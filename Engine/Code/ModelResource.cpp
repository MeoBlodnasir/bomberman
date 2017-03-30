
#include "ModelResource.hpp"

#include "Path.hpp"

//#include "ProfilerBlock.hpp"
#include "Output.hpp"// tests

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
#if defined (__FT_DEBUG__)
									aiProcess_FindInvalidData		|
									aiProcess_ValidateDataStructure	|
#endif
									aiProcess_Triangulate			|
									aiProcess_JoinIdenticalVertices	|
									aiProcess_GenNormals			|
									aiProcess_GenUVCoords			|
									aiProcess_CalcTangentSpace		|
									aiProcess_SortByPType			|
									aiProcess_FlipUVs				|
									aiProcess_RemoveRedundantMaterials
								);

		// Vérifications de la validité
		FT_ASSERT(pScene != nullptr);
		FT_ASSERT(!(pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
		FT_ASSERT(pScene->mRootNode != nullptr);
		FT_ASSERT(pScene->mNumMeshes > 0);
		if (pScene == nullptr || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || pScene->mRootNode == nullptr || pScene->mNumMeshes == 0)
			return FT_FAIL;

		// Pas bloquant, mais autant le faire pour voir
		FT_ASSERT(!(pScene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING))
		if (pScene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING)
		{
			FT_CERR << "Assimp WARNING: " << oAssimpImporter.GetErrorString() << std::endl;
		}

		// Tests
		{
			FT_COUT << oModelFilePath.GetFullPath() << std::endl;
			FT_COUT << pScene->mNumMaterials << " materiaux" << std::endl;
			for (uint32 i = 0, iCount = pScene->mNumMaterials; i < iCount; ++i)
			{
				const aiMaterial* pMat = pScene->mMaterials[i];
				aiString	oString;
				aiColor3D	oColor;
				float		fFloat;
				FT_COUT << "\tMat " << i << " : " << std::endl;
				if (pMat->Get(AI_MATKEY_NAME, oString) == AI_SUCCESS)
					FT_COUT << "\t\tName : " << oString.C_Str() << std::endl;
				if (pMat->Get(AI_MATKEY_COLOR_DIFFUSE, oColor) == AI_SUCCESS)
					FT_COUT << "\t\tDiffuse Color :  " << oColor.r << ',' << oColor.g << ',' << oColor.b << std::endl;
				if (pMat->Get(AI_MATKEY_COLOR_AMBIENT, oColor) == AI_SUCCESS)
					FT_COUT << "\t\tAmbient Color :  " << oColor.r << ',' << oColor.g << ',' << oColor.b << std::endl;
				if (pMat->Get(AI_MATKEY_COLOR_SPECULAR, oColor) == AI_SUCCESS)
					FT_COUT << "\t\tSpecular Color : " << oColor.r << ',' << oColor.g << ',' << oColor.b << std::endl;
				if (pMat->Get(AI_MATKEY_COLOR_EMISSIVE, oColor) == AI_SUCCESS)
					FT_COUT << "\t\tEmissive Color : " << oColor.r << ',' << oColor.g << ',' << oColor.b << std::endl;
				if (pMat->Get(AI_MATKEY_SHININESS, fFloat) == AI_SUCCESS)
					FT_COUT << "\t\tShininess : " << fFloat << std::endl;
				if (pMat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), oString) == AI_SUCCESS)
					FT_COUT << "\t\tTexture Diffuse :  " << oString.C_Str() << std::endl;
				if (pMat->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT, 0), oString) == AI_SUCCESS)
					FT_COUT << "\t\tTexture Ambient :  " << oString.C_Str() << std::endl;
				if (pMat->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), oString) == AI_SUCCESS)
					FT_COUT << "\t\tTexture Specular : " << oString.C_Str() << std::endl;
				if (pMat->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), oString) == AI_SUCCESS)
					FT_COUT << "\t\tTexture Emissive : " << oString.C_Str() << std::endl;
				if (pMat->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), oString) == AI_SUCCESS)
					FT_COUT << "\t\tTexture Normals :  " << oString.C_Str() << std::endl;
			}
		}
		//

		const aiNode* pNode = pScene->mRootNode;
		while (pNode->mMetaData == nullptr && pNode->mNumMeshes == 0)
		{
			if (pNode->mNumChildren > 0)
				pNode = pNode->mChildren[0];
			else
				return FT_FAIL;
		}

		// Récupération des maillages
		SPtr<MeshData>	xMeshResource = nullptr;
		oMeshResources.clear();
		oMeshResources.reserve(pScene->mNumMeshes);
		for (uint32 i = 0, iCount = pScene->mNumMeshes; i < iCount; ++i)
		{
			xMeshResource = new MeshData;
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
