
#include "ModelResource.hpp"

#include "Path.hpp"

//#include "ProfilerBlock.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <stack>

// Vérification, car la copie de l'un vers l'autre se fait par memcpy,
// assumant que les deux utilisent la même quantité d'espace mémoire.
// Mais il faut la transposer avant:
// aiMatrix4x4	=> row-major
// ft::Matrix44	=> column-major
FT_STATIC_ASSERT(sizeof(ft::Matrix44) == sizeof(aiMatrix4x4));

namespace ft
{
	// Structure purement utilitaire
	struct ParentNodesInfos
	{
		const aiNode*		pAiNode;
		ModelNodeResource*	pModelNode;
		uint32				iIndex;
		uint32				iChildToRead;

		ParentNodesInfos()
			: pAiNode(nullptr)
			, pModelNode(nullptr)
			, iIndex(0)
			, iChildToRead(0)
		{}
		ParentNodesInfos(const aiNode* _pAiNode, ModelNodeResource* _pModelNode, uint32 _iIndex)
			: pAiNode(_pAiNode)
			, pModelNode(_pModelNode)
			, iIndex(_iIndex)
			, iChildToRead(0)
		{}
	};

	
	ErrorCode	ModelResource::LoadFromFile(const Path& oModelFilePath)
	{
		//ProfilerBlockPrint oProfilerBlock(std::string("ModelResource::LoadFromFile : ") + oModelFilePath.GetFullPath());

		Assimp::Importer oAssimpImporter;

		// Chargement de la scène
		const aiScene* pScene = oAssimpImporter.ReadFile(oModelFilePath.GetFullPath(),
								aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

		// Vérifications de la validité
		FT_ASSERT(pScene != nullptr);
		FT_ASSERT(!(pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
		FT_ASSERT(pScene->mRootNode != nullptr);
		FT_ASSERT(pScene->mNumMeshes > 0);
		if (pScene == nullptr || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || pScene->mRootNode == nullptr || pScene->mNumMeshes == 0)
			return FT_FAIL;

		const aiNode* pNode = pScene->mRootNode;
		while (pNode->mMetaData == nullptr && pNode->mNumMeshes == 0)
		{
			if (pNode->mNumChildren > 0)
				pNode = pNode->mChildren[0];
			else
				return FT_FAIL;
		}

		// Récupération des maillages
		SPtr<MeshResource>	xMeshResource = nullptr;
		oMeshResources.clear();
		oMeshResources.reserve(pScene->mNumMeshes);
		for (uint32 i = 0, iCount = pScene->mNumMeshes; i < iCount; ++i)
		{
			xMeshResource = new MeshResource;
			xMeshResource->MakeFromAssimpMesh(pScene->mMeshes[i]);
			oMeshResources.push_back(xMeshResource);
		}

		// Récupération de la hiérarchie et des transformations
		// Création du noeud racine
		ModelNodeResource::Desc oModelNodeResDesc;
		xRootNode = new ModelNodeResource;
		oModelNodeResDesc.pParent = nullptr;
		FT_TEST(xRootNode->Create(&oModelNodeResDesc) == FT_OK);
		xRootNode->SetName(oModelFilePath.GetName());
		xRootNode->iParentIndex = -1;
		oNodes.clear();
		oNodes.push_back(xRootNode.Ptr());
		// Parcours de la hiérarchie de noeuds de Assimp
		// A chaque enfant, on empile dans la stack, quand les enfants sont traités, on dépile
		std::stack<ParentNodesInfos> oParentStack;
		oParentStack.push(ParentNodesInfos(pNode->mParent, xRootNode.Ptr(), oNodes.size() - 1));

		SPtr<ModelNodeResource>	xModelNode = nullptr;
		while (!oParentStack.empty())
		{
			ParentNodesInfos& oParentNodesInfos = oParentStack.top();
			FT_ASSERT(oParentNodesInfos.pAiNode != nullptr);
			
			if (oParentNodesInfos.iChildToRead < oParentNodesInfos.pAiNode->mNumChildren)
			{
				pNode = oParentNodesInfos.pAiNode->mChildren[oParentNodesInfos.iChildToRead];

				xModelNode = new ModelNodeResource;
				oModelNodeResDesc.pParent = dynamic_cast<HierarchyNode*>(oParentNodesInfos.pModelNode);
				FT_ASSERT(oModelNodeResDesc.pParent != nullptr);
				FT_TEST(xModelNode->Create(&oModelNodeResDesc) == FT_OK);
				xModelNode->SetName(pNode->mName.C_Str());

				::memcpy((void*)&xModelNode->mLocalTransform, (void*)&pNode->mTransformation, sizeof(Matrix44));
				xModelNode->mLocalTransform = glm::transpose(xModelNode->mLocalTransform);

				for (uint32 i = 0, iCount = pNode->mNumMeshes; i < iCount; ++i)
					xModelNode->oMeshIndice.push_back(pNode->mMeshes[i]);

				xModelNode->iParentIndex = oParentNodesInfos.iIndex;

				oNodes.push_back(xModelNode.Ptr());

				// largeur
				++oParentNodesInfos.iChildToRead;

				// descente
				oParentStack.push(ParentNodesInfos(pNode, xModelNode.Ptr(), oNodes.size() - 1));
			}
			else
				// remontée
				oParentStack.pop();
		}

		return FT_OK;
	}
}
