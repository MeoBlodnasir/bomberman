#pragma once

namespace ft
{
	template <class TNode>
	HierarchyNodeIterator<TNode>::HierarchyNodeIterator()
		: m_pStartingNode(nullptr)
		, m_pNode(nullptr)
	{
	}

	template <class TNode>
	HierarchyNodeIterator<TNode>::HierarchyNodeIterator(TNode* pNode)
		: m_pStartingNode(pNode)
		, m_pNode(pNode)
	{
	}

	template <class TNode>
	HierarchyNodeIterator<TNode>::HierarchyNodeIterator(const HierarchyNodeIterator<TNode>& oIt)
		: m_pStartingNode(oIt.m_pStartingNode)
		, m_pNode(oIt.m_pNode)
	{
	}

	template <class TNode>
	template <class TOther>
	HierarchyNodeIterator<TNode>::HierarchyNodeIterator(const HierarchyNodeIterator<TOther>& oIt)
		: m_pStartingNode(oIt.m_pStartingNode)
		, m_pNode(oIt.m_pNode)
	{
	}

	template <class TNode>
	HierarchyNodeIterator<TNode>::~HierarchyNodeIterator()
	{
		m_pStartingNode	= nullptr;
		m_pNode			= nullptr;
	}

	template <class TNode>
	HierarchyNodeIterator<TNode>&
	HierarchyNodeIterator<TNode>::operator = (const HierarchyNodeIterator<TNode>& oIt)
	{
		m_pStartingNode	= oIt.m_pStartingNode;
		m_pNode			= oIt.m_pNode;
		return *this;
	}

	template <class TNode>
	template <class TOther>
	HierarchyNodeIterator<TNode>&
	HierarchyNodeIterator<TNode>::operator = (const HierarchyNodeIterator<TOther>& oIt)
	{
		m_pStartingNode	= oIt.m_pStartingNode;
		m_pNode			= oIt.m_pNode;
		return *this;
	}

	template <class TNode>
	void
	HierarchyNodeIterator<TNode>::Reset(TNode* pNode)
	{
		m_pStartingNode	= pNode;
		m_pNode			= m_pStartingNode;
	}

	template <class TNode>
	void
	HierarchyNodeIterator<TNode>::Next() // Parcours préfixe
	{
		FT_ASSERT(IsValid());
		if (m_pNode != nullptr)
		{
			// Descente dans la hiérarchie
			const TNode* pNext = m_pNode->GetChild();
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
						const TNode* pParent = m_pNode->GetParent();
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
