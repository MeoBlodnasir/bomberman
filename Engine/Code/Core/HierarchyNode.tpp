
namespace ft
{
	template <typename TNode>
	HierarchyNode<TNode>::HierarchyNode()
		: m_pParent(nullptr)
		, m_xChild(nullptr)
		, m_xSibling(nullptr)
	{
	}

	template <typename TNode>
	HierarchyNode<TNode>::~HierarchyNode()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	template <typename TNode>
	ErrorCode	HierarchyNode<TNode>::Create(const Desc* pDesc)
	{
		FT_TEST_RETURN(pDesc != nullptr, FT_FAIL);
		SetParent(pDesc->pParent);
		return FT_OK;
	}

	template <typename TNode>
	ErrorCode	HierarchyNode<TNode>::Destroy()
	{
		// Détacher les noeuds enfants
		TNode* pChild = m_xChild;
		TNode* pChildSibling = nullptr;
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

	template <typename TNode>
	TNode*		HierarchyNode<TNode>::GetRootNode()
	{
		TNode* pRootNode = this;

		while (pRootNode->m_pParent != nullptr)
			pRootNode = pRootNode->m_pParent;

		return pRootNode;
	}

	template <typename TNode>
	uint32		HierarchyNode<TNode>::GetHierarchyCount() const
	{
		uint32 iHierarchyCount = 1;
		const_iterator it(m_xChild.Ptr());

		while (it != nullptr)
		{
			++iHierarchyCount;
			it.Next();
		}

		return iHierarchyCount;
	}

	template <typename TNode>
	void		HierarchyNode<TNode>::SetParent(TNode* pParent)
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
				pParent->m_xChild = dynamic_cast<TNode*>(this);
			}
			ReleaseReference();
		}
	}

	template <typename TNode>
	void		HierarchyNode<TNode>::UnlinkFromParent()
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
				TNode* pTemp = m_pParent->m_xChild;
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
