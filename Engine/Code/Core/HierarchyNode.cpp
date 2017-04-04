
#include "Core/HierarchyNode.hpp"

namespace ft
{
	HierarchyNode::HierarchyNode()
		: m_pParent(nullptr)
		, m_xChild(nullptr)
		, m_xSibling(nullptr)
	{
	}

	HierarchyNode::~HierarchyNode()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	HierarchyNode::Create(const Desc* pDesc)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		SetAsChildOf(pDesc->pParent);
		return FT_OK;
	}

	ErrorCode	HierarchyNode::Destroy()
	{
		// Détacher les noeuds enfants
		HierarchyNode* pChild = m_xChild;
		HierarchyNode* pChildSibling = nullptr;
		while (pChild != nullptr)
		{
			pChildSibling = pChild->m_xSibling;
			pChild->UnlinkFromParent();
			pChild = pChildSibling;
		}

		// Détacher du parent
		UnlinkFromParent();

		return FT_OK;
	}

	uint32		HierarchyNode::GetHierarchyCount()
	{
		FT_TODO("HierarchyNode::GetHierarchyCount() : Faire avec HierarchyNodeIterator const");

		uint32 iChildCount = 1;
		HierarchyNode* pChild = m_xChild;

		while (pChild != nullptr)
		{
			iChildCount += pChild->GetHierarchyCount();
			pChild = pChild->m_xSibling;
		}

		return iChildCount;
	}

	void		HierarchyNode::SetAsChildOf(HierarchyNode* pParent)
	{
		FT_ASSERT(pParent != this);
		if (pParent != m_pParent)
		{
			AddReference();
			UnlinkFromParent();
			if (pParent != nullptr)
			{
				FT_ASSERT(m_pParent == nullptr);
				FT_ASSERT(m_xSibling == nullptr);
				m_pParent = pParent;
				m_xSibling = pParent->m_xChild;
				pParent->m_xChild = this;
			}
			ReleaseReference();
		}
	}

	void		HierarchyNode::UnlinkFromParent()
	{
		// Détacher du parent
		if (m_pParent != nullptr)
		{
			AddReference();

			if (m_pParent->m_xChild == this)
			{
				// Si le noeud courant est enfant direct du parent, le remplacer par le noeud frère direct
				m_pParent->m_xChild = m_xSibling;
			}
			else
			{
				// Récupérer le noeud dont le frère direct est le noeud courant
				HierarchyNode* pTemp = m_pParent->m_xChild;
				FT_ASSERT(pTemp != nullptr);
				while (pTemp->m_xSibling != this)
				{
					pTemp = pTemp->m_xSibling;
					FT_ASSERT(pTemp != nullptr);
				}
				pTemp->m_xSibling = m_xSibling;
			}
			m_pParent = nullptr;
			m_xSibling = nullptr;

			ReleaseReference();
		}
		else
		{
			FT_ASSERT(m_xSibling == nullptr);
		}
	}
}
