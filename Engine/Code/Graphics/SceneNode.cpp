
#include "Graphics/SceneNode.hpp"

namespace ft
{
	SceneNode::SceneNode()
		: m_iFlags(E_NONE)
	{
	}

	SceneNode::~SceneNode()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	SceneNode::Create(const Desc* pDesc)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		SetParent(pDesc->pParent, false);
		SetLocalTransform(Matrix44(1));
		Update();
		return FT_OK;
	}

	ErrorCode	SceneNode::Destroy()
	{
		return HierarchyNode::Destroy();
	}

	void	SceneNode::Update()
	{
		// Priorité donnée à la transformation en espace Local
		if (m_iFlags & E_OUTDATED_WORLD_TRANSFORM)
		{
			UpdateWorldTransform();
		}
		else if (m_iFlags & E_OUTDATED_LOCAL_TRANSFORM)
		{
			UpdateLocalTransform();
		}
	}

	void	SceneNode::UpdateHierarchy(SceneNode* pNode)
	{
		for (SceneNodeIt it(pNode); it != nullptr; ++it)
			it->Update();
	}

	void	SceneNode::SetLocalTransform(const Matrix44& m)
	{
		m_mLocal = m;
		if (m_pParent == nullptr)
			m_mWorld = m;
		else
			SetFlag(E_OUTDATED_WORLD_TRANSFORM);

		PropagateFlag(E_OUTDATED_WORLD_TRANSFORM, true);
	}

	void	SceneNode::SetWorldTransform(const Matrix44& m)
	{
		m_mWorld = m;
		if (m_pParent == nullptr)
			m_mLocal = m;
		else
			SetFlag(E_OUTDATED_LOCAL_TRANSFORM);

		PropagateFlag(E_OUTDATED_WORLD_TRANSFORM, true);
	}

	void	SceneNode::UpdateLocalTransform()
	{
		FT_ASSERT(!(m_iFlags & E_OUTDATED_WORLD_TRANSFORM)); // Priorité donnée à la transformation en espace Local

		if (m_pParent == nullptr)
			m_mLocal = m_mWorld;
		else
			m_mLocal = glm::inverse(m_pParent->m_mWorld) * m_mWorld;

		UnsetFlag(E_OUTDATED_LOCAL_TRANSFORM);
	}

	void	SceneNode::UpdateWorldTransform()
	{
		if (m_pParent == nullptr)
			m_mWorld = m_mLocal;
		else
			m_mWorld = m_pParent->m_mWorld * m_mLocal;

		UnsetFlag(E_OUTDATED_WORLD_TRANSFORM);
	}

	void	SceneNode::SetParent(SceneNode* pParent, bool bPreserveWorldTransform)
	{
		FT_ASSERT(pParent != this);

		if (pParent != m_pParent)
		{
			HierarchyNode::SetParent(pParent);
			SetFlag(bPreserveWorldTransform ? E_OUTDATED_LOCAL_TRANSFORM : E_OUTDATED_WORLD_TRANSFORM);
			PropagateFlag(E_OUTDATED_WORLD_TRANSFORM, true);
		}
	}

	void	SceneNode::SetFlag(uint32 iFlag, bool bPropagate /*= false*/)
	{
		m_iFlags |= iFlag;
		if (bPropagate)
			PropagateFlag(iFlag, true);
	}

	void	SceneNode::UnsetFlag(uint32 iFlag, bool bPropagate /*= false*/)
	{
		m_iFlags &= ~iFlag;
		if (bPropagate)
			PropagateFlag(iFlag, false);
	}

	void	SceneNode::PropagateFlag(uint32 iFlag, bool bValue)
	{
		SceneNode* pChild = m_xChild.Ptr();

		while (pChild != nullptr)
		{
			if (bValue)
				pChild->SetFlag(iFlag, true);
			else
				pChild->UnsetFlag(iFlag, true);

			pChild = pChild->m_xSibling.Ptr();
		}
	}
}
