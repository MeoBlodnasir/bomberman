
#include "Graphics/Model.hpp"

#include "Graphics/Renderer.hpp"
#include "Graphics/RenderTechnique.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Mesh.hpp"

namespace ft
{
	Model::Model()
		: m_xModelResource(nullptr)
	{
	}

	Model::~Model()
	{
	}

	ErrorCode	Model::Create(const Desc* pDesc, const SPtr<ModelResource>& xResource)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		FT_TEST_RETURN(xResource != nullptr && xResource->IsLoadedAndValid(), FT_FAIL);

		// Chargement de la ressource
		std::vector<InternalNode*>	oInternalNodes;
		InternalNode::Desc			oInternalNodeDesc;
		SPtr<Mesh>					xMesh = nullptr;

		oInternalNodes.reserve(xResource->m_oNodes.size());
		for (const ModelResource::Node& oResourceNode : xResource->m_oNodes)
		{
			// Hi�rarchie
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

		FT_TEST_RETURN(SceneNode::Create(dynamic_cast<const SceneNode::Desc*>(pDesc)) == FT_OK, FT_FAIL);
		
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

	void	Model::Update()
	{
		SceneNode::Update();
		
		if (m_xRootNode != nullptr)
		{
			m_xRootNode->SetWorldTransform(GetWorldTransform());
			UpdateHierarchy(m_xRootNode.Ptr());
		}
	}

	void	Model::InternalNode::Render(const RenderContext& oRenderContext) const 
	{
		for (const SPtr<Mesh>& xMesh : m_oMeshes)
			xMesh->Render(oRenderContext, GetWorldTransform());
	}

	void	Model::Render(const RenderContext& oRenderContext) const 
	{
		SceneNodeConstIt itSceneNode(m_xRootNode);
		while (itSceneNode != nullptr)
		{
			itSceneNode->Render(oRenderContext);
			itSceneNode.Next();
		}
	}
}
