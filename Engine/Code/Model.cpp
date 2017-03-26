
#include "Model.hpp"

namespace ft
{
	Model::Model()
		: m_xModelResource(nullptr)
	{
	}

	Model::~Model()
	{
	}

	ErrorCode	Model::Create(const Model::Desc* pDesc, const SPtr<ModelResource>& xModelResource)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		FT_TEST_RETURN(SceneNode::Create(pDesc) == FT_OK, FT_FAIL);

		// Chargement de la ressource
		std::vector<ModelNode*>	oNodes;
		ModelNode::Desc			oModelNodeDesc;
		SPtr<Mesh>				xMesh = nullptr;

		oNodes.reserve(xModelResource->oNodes.size());
		for (const ModelNodeResource* pNodeResource : xModelResource->oNodes)
		{
			// Hiérarchie
			oNodes.push_back(new ModelNode);
			oModelNodeDesc.pParent = pNodeResource->iParentIndex >= 0 ? oNodes[pNodeResource->iParentIndex] : nullptr;
			FT_TEST(oNodes.back()->Create(&oModelNodeDesc) == FT_OK);

			// Maillages
			for (uint32 iMeshIndex : pNodeResource->oMeshIndice)
			{
				xMesh = new Mesh;
				FT_TEST(xMesh->Create(xModelResource->oMeshResources[iMeshIndex]) == FT_OK);
				oNodes.back()->m_oMeshes.push_back(xMesh);
			}

			// Transform
			oNodes.back()->SetLocalTransform(pNodeResource->mLocalTransform);
		}

		m_xRootNode = oNodes.front();
		m_xRootNode->SetWorldTransform(GetWorldTransform());
		SceneNode::UpdateHierarchy(m_xRootNode.Ptr());

		m_xModelResource = xModelResource;

		return FT_OK;
	}

	ErrorCode	Model::Destroy()
	{
		m_xModelResource = nullptr;
		m_xRootNode = nullptr;
		return SceneNode::Destroy();
	}

	bool	Model::IsValid() const
	{
		return true;
	}

	void	Model::Draw() const
	{
	}
}
