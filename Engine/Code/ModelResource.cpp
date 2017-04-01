
#include "ModelResource.hpp"

#include "Path.hpp"
#include "ResourceManager.hpp"

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

		// Tests matériaux
		{
			FT_COUT << oInfos.oFilePath.GetFullPath() << std::endl;
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
		for (uint32 i = 0, iCount = pScene->mNumMeshes; i < iCount; ++i)
		{
			oMeshResourceInfos.pAiMesh = pScene->mMeshes[i];
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

				::memcpy((void*)&m_oNodes.back().mLocalTransform, (void*)&itAiNode->mTransformation, sizeof(Matrix44));
				m_oNodes.back().mLocalTransform = glm::transpose(m_oNodes.back().mLocalTransform);

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
