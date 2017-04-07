#pragma once

#include "Core/Types.hpp"
#include "Core/ErrorCode.hpp"
#include "Core/StrongPointer.hpp"
#include "Core/HierarchyNodeIterator.hpp"

namespace ft
{
	// S'utilise en héritant d'un HierarchyNode templaté de son propre type
	// Exemple: class SceneNode : public HierarchyNode<SceneNode> {};

	template <typename TNode>
	class HierarchyNode : public IHierarchyNode, public CountableSPtr
	{
	public:

		typedef TNode	NodeType;

		typedef HierarchyNodeIterator< 		 NodeType >	iterator;
		typedef HierarchyNodeIterator< const NodeType >	const_iterator;

		struct Desc
		{
			NodeType*	pParent;
			Desc() : pParent(nullptr) {}
		};

		HierarchyNode<TNode>();
		virtual	~HierarchyNode<TNode>();

		// Interface IHierarchyNode
		virtual		  IHierarchyNode*	GetParent()			override		{ return m_pParent; }
		virtual const IHierarchyNode*	GetParent() const	override		{ return m_pParent; }
		virtual		  IHierarchyNode*	GetChild()			override		{ return m_xChild; }
		virtual const IHierarchyNode*	GetChild() const	override		{ return m_xChild; }
		virtual		  IHierarchyNode*	GetSibling()		override		{ return m_xSibling; }
		virtual const IHierarchyNode*	GetSibling() const	override		{ return m_xSibling; }

		virtual ErrorCode				Create(const Desc* pDesc);
		virtual ErrorCode				Destroy();

				TNode*					GetRootNode();
				uint32					GetHierarchyCount() const;
			
		virtual	void					SetParent(TNode* pParent);
		virtual	void					UnlinkFromParent();

	protected:

		TNode*			m_pParent;
		SPtr<TNode>		m_xChild;
		SPtr<TNode>		m_xSibling;

	private:

		HierarchyNode<TNode>(const HierarchyNode<TNode>&) FT_DELETED;
		HierarchyNode<TNode>& operator = (const HierarchyNode<TNode>&) FT_DELETED;
	};
}

#include "Core/HierarchyNode.tpp"
