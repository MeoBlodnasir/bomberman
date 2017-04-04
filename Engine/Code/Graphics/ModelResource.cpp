
#include "Graphics/ModelResource.hpp"

//#include "Core/ProfilerBlock.hpp"
#include "Core/Output.hpp"// tests
#include "Core/Path.hpp"
#include "Resource/ResourceManager.hpp"
#include "External/AssimpConversions.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <stack>

namespace ft
{
	// Structure purement utilitaire pour éviter de la récursivité
	struct ParentNodesInfos
	{
		const aiNode*	pAiNode;
		uint32			iIndex;
		uint32			iChildToRead;

		ParentNodesInfos(const aiNode* _pAiNode, uint32 _iIndex) : pAiNode(_pAiNode) , iIndex(_iIndex) , iChildToRead(0) {}
	};

	ModelResource::ModelResource()
	{
	}

	ModelResource::~ModelResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	ModelResource::IsLoadedAndValid() const 
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_oNodes.size() > 0;
	}

	ErrorCode	ModelResource::Load(ResourceManager& oResourceManager, const ModelResourceInfos& oInfos)
	{
		//ProfilerBlockPrint oProfilerBlock(std::string("ModelResource::LoadFromFile : ") + oModelFilePath.GetFullPath());

		Assimp::Importer oAssimpImporter;

		// Chargement de la scène
		const aiScene* pScene = oAssimpImporter.ReadFile(oInfos.oFilePath.GetFullPath(),
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
		FT_ASSERT(!(pScene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING));
		if (pScene->mFlags & AI_SCENE_FLAGS_VALIDATION_WARNING)
		{
			FT_CERR << "Assimp WARNING: " << oAssimpImporter.GetErrorString() << std::endl;
		}

		// Récupération du premier noeud contenant un maillage
		const aiNode* itAiNode = pScene->mRootNode;
		while (itAiNode->mMetaData == nullptr && itAiNode->mNumMeshes == 0)
		{
			if (itAiNode->mNumChildren > 0)
				itAiNode = itAiNode->mChildren[0];
			else
				return FT_FAIL;
		}

		// Récupération des maillages
		m_oMeshResources.clear();
		m_oMeshResources.reserve(pScene->mNumMeshes);
		MeshResourceInfos oMeshResourceInfos;
		SPtr<MeshResource> xMeshResource = nullptr;
		oMeshResourceInfos.eSource = E_ASSIMP_MESH;
		oMeshResourceInfos.oLocalPath = Path(oInfos.oFilePath.GetDirPath());
		for (uint32 i = 0, iCount = pScene->mNumMeshes; i < iCount; ++i)
		{
			oMeshResourceInfos.pAiMesh = pScene->mMeshes[i];
			oMeshResourceInfos.pAiMaterial = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex];
			FT_TEST(oResourceManager.GetMeshResourceManager()->Load(oMeshResourceInfos, xMeshResource) == FT_OK);
			m_oMeshResources.push_back(xMeshResource);
		}

		// Récupération de la hiérarchie et des transformations
		// Parcours de la hiérarchie de noeuds de Assimp
		// A chaque enfant, on empile dans la stack, quand les enfants sont traités, on dépile
		// (pseudo-récursivité)
		m_oNodes.clear();
		std::stack<ParentNodesInfos> oParentStack;
		oParentStack.push(ParentNodesInfos(itAiNode->mParent, m_oNodes.size() - 1));
		while (!oParentStack.empty())
		{
			ParentNodesInfos& oParentNodesInfos = oParentStack.top();
			FT_ASSERT(oParentNodesInfos.pAiNode != nullptr);

			if (oParentNodesInfos.iChildToRead < oParentNodesInfos.pAiNode->mNumChildren)
			{
				itAiNode = oParentNodesInfos.pAiNode->mChildren[oParentNodesInfos.iChildToRead];

				m_oNodes.push_back(Node());
				m_oNodes.back().iParentIndex = oParentNodesInfos.iIndex;
				FT_ASSERT(m_oNodes.back().iParentIndex < (int32)m_oNodes.size() - 1);

				FromAssimp(m_oNodes.back().mLocalTransform, itAiNode->mTransformation);

				for (uint32 i = 0, iCount = itAiNode->mNumMeshes; i < iCount; ++i)
					m_oNodes.back().oMeshIndice.push_back(itAiNode->mMeshes[i]);

				// largeur
				++oParentNodesInfos.iChildToRead;

				// descente
				oParentStack.push(ParentNodesInfos(itAiNode, m_oNodes.size() - 1));
			}
			else
				// remontée
				oParentStack.pop();
		}

		m_oResourceInfos = oInfos;

		return FT_OK;
	}

	ErrorCode	ModelResource::Unload()
	{
		m_oNodes.clear();
		m_oMeshResources.clear();

		return FT_OK;
	}
}
