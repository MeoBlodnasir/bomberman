#pragma once

namespace ft
{
	template <class THierarchyNode, class TNode>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::HierarchyNodeIteratorTemplate()
		: m_pStartingNode(nullptr)
		, m_pNode(nullptr)
	{
	}

	template <class THierarchyNode, class TNode>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::HierarchyNodeIteratorTemplate(TNode* pNode)
		: m_pStartingNode(dynamic_cast<IHierarchyNode*>(pNode))
		, m_pNode(m_pStartingNode)
	{
	}

	template <class THierarchyNode, class TNode>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::HierarchyNodeIteratorTemplate(const HierarchyNodeIteratorTemplate<THierarchyNode, TNode>& oIt)
		: m_pStartingNode(oIt.m_pStartingNode)
		, m_pNode(oIt.m_pNode)
	{
	}

	template <class THierarchyNode, class TNode>
	template <class TOther>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::HierarchyNodeIteratorTemplate(const HierarchyNodeIteratorTemplate<THierarchyNode, TOther>& oIt)
		: m_pStartingNode(oIt.m_pStartingNode)
		, m_pNode(oIt.m_pNode)
	{
	}

	template <class THierarchyNode, class TNode>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::~HierarchyNodeIteratorTemplate()
	{
		m_pStartingNode	= nullptr;
		m_pNode			= nullptr;
	}

	template <class THierarchyNode, class TNode>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>&
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::operator = (const HierarchyNodeIteratorTemplate<THierarchyNode, TNode>& oIt)
	{
		m_pStartingNode	= oIt.m_pStartingNode;
		m_pNode			= oIt.m_pNode;
		return *this;
	}

	template <class THierarchyNode, class TNode>
	template <class TOther>
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>&
		HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::operator = (const HierarchyNodeIteratorTemplate<THierarchyNode, TOther>& oIt)
	{
		m_pStartingNode	= oIt.m_pStartingNode;
		m_pNode			= oIt.m_pNode;
		return *this;
	}

	template <class THierarchyNode, class TNode>
	void
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::Reset(TNode* pNode)
	{
		m_pStartingNode	= dynamic_cast<IHierarchyNode*>(pNode);
		m_pNode			= m_pStartingNode;
	}

	template <class THierarchyNode, class TNode>
	void
	HierarchyNodeIteratorTemplate<THierarchyNode, TNode>::Next() // Parcours préfixe
	{
		FT_ASSERT(IsValid());
		if (m_pNode != nullptr)
		{
			// Descente dans la hiérarchie
			THierarchyNode* pNext = m_pNode->GetChild();
			if (pNext == nullptr)
			{
				// Parcours horizontal
				if (m_pNode == m_pStartingNode)
					m_pNode = nullptr;
				else
				{
					pNext = m_pNode->GetSibling();
					if (pNext == nullptr)
					{
						// Remontée
						THierarchyNode* pParent = m_pNode->GetParent();
						while (pParent != nullptr && pParent != m_pStartingNode)
						{
							if (pParent->GetSibling() != nullptr)
							{
								pNext = pParent->GetSibling();
								break;
							}
							pParent = pParent->GetParent();
						}
					}
				}
			}
			m_pNode = pNext;
		}
		FT_ASSERT(IsValid());
	}
}
