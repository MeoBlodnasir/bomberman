
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

	ErrorCode	Model::Create(const Model::Desc* pDesc, const SPtr<ModelResource>& xResource)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		FT_TEST_RETURN(xResource != nullptr && xResource->IsLoadedAndValid(), FT_FAIL);

		FT_TEST_RETURN(SceneNode::Create(pDesc) == FT_OK, FT_FAIL);

		// Chargement de la ressource
		std::vector<InternalNode*>	oInternalNodes;
		InternalNode::Desc			oInternalNodeDesc;
		SPtr<Mesh>					xMesh = nullptr;

		oInternalNodes.reserve(xResource->m_oNodes.size());
		for (const ModelResource::Node& oResourceNode : xResource->m_oNodes)
		{
			// Hiérarchie
			oInternalNodes.push_back(new InternalNode);
			oInternalNodeDesc.pParent = oResourceNode.iParentIndex >= 0 ? oInternalNodes[oResourceNode.iParentIndex] : nullptr;
			FT_TEST(oInternalNodes.back()->Create(&oInternalNodeDesc) == FT_OK);

			// Maillages
			for (uint32 iMeshIndex : oResourceNode.oMeshIndice)
			{
				xMesh = new Mesh;
				FT_TEST(xMesh->Create(xResource->m_oMeshResources[iMeshIndex]) == FT_OK);
				oInternalNodes.back()->m_oMeshes.push_back(xMesh);
			}

			// Transform
			oInternalNodes.back()->SetLocalTransform(oResourceNode.mLocalTransform);
		}

		m_xRootNode = oInternalNodes.front();
		m_xRootNode->SetWorldTransform(GetWorldTransform());
		SceneNode::UpdateHierarchy(m_xRootNode.Ptr());

		m_xModelResource = xResource;

		return FT_OK;
	}

	ErrorCode	Model::Destroy()
	{
		m_xModelResource = nullptr;
		m_xRootNode = nullptr;
		return SceneNode::Destroy();
	}
}
