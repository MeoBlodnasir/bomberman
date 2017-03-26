#pragma once

#include "Types.hpp"
#include "ErrorCode.hpp"
#include "StrongPointer.hpp"
#include "HierarchyNodeIterator.hpp"

namespace ft
{
	class HierarchyNode : public IHierarchyNode, public CountableSPtr
	{
	public:

		typedef HierarchyNodeIterator<		HierarchyNode>	iterator;
		typedef HierarchyNodeIterator<const	HierarchyNode>	const_iterator;

		struct Desc
		{
			HierarchyNode*	pParent;

			Desc() : pParent(nullptr) {}
		};

		HierarchyNode();
		virtual	~HierarchyNode();

		// Interface IHierarchyNode
		virtual			IHierarchyNode*	GetParent()			override { return m_pParent; }
		virtual const	IHierarchyNode*	GetParent() const	override { return m_pParent; }
		virtual			IHierarchyNode*	GetChild()			override { return m_xChild; }
		virtual const	IHierarchyNode*	GetChild() const	override { return m_xChild; }
		virtual			IHierarchyNode*	GetSibling()		override { return m_xSibling; }
		virtual const	IHierarchyNode*	GetSibling() const	override { return m_xSibling; }

		virtual ErrorCode		Create(const Desc* pDesc); // Pointeur?
		virtual ErrorCode		Destroy();

				HierarchyNode*	GetRootNode() const				{ return m_pParent == nullptr ? const_cast<HierarchyNode*>(this) : m_pParent->GetRootNode(); }
				uint32			GetHierarchyCount();
			
		virtual	void			SetAsChildOf(HierarchyNode* pParent);
		virtual	void			UnlinkFromParent();

	protected:

		HierarchyNode*		m_pParent;
		SPtr<HierarchyNode>	m_xChild;
		SPtr<HierarchyNode>	m_xSibling;

	private:

		HierarchyNode(const HierarchyNode&) FT_DELETED;
		HierarchyNode& operator = (const HierarchyNode&) FT_DELETED;
	};

	typedef HierarchyNode::iterator			HNodeIt;
	typedef HierarchyNode::const_iterator	HNodeConstIt;
}
