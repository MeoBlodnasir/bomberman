#pragma once

#include "Core.hpp"
#include "IHierarchyNode.hpp"

// Faire une version const ?

namespace ft
{
	template <class THierarchyNode, class TNode>
	class HierarchyNodeIteratorTemplate
	{
	public:

		typedef HierarchyNodeIteratorTemplate<THierarchyNode, TNode>	IteratorType;

		HierarchyNodeIteratorTemplate();
		HierarchyNodeIteratorTemplate(TNode* pNode);
		HierarchyNodeIteratorTemplate(const IteratorType& oIt);
		template <class TOther>
		HierarchyNodeIteratorTemplate(const HierarchyNodeIteratorTemplate<THierarchyNode, TOther>& oIt);
		virtual ~HierarchyNodeIteratorTemplate();

		IteratorType& operator = (const HierarchyNodeIteratorTemplate<THierarchyNode, TNode>& oIt);
		template <class TOther>
		IteratorType& operator = (const HierarchyNodeIteratorTemplate<THierarchyNode, TOther>& oIt);

		inline					operator TNode* () const			{ return dynamic_cast<TNode*>(m_pNode); }
		inline TNode*			operator * () const					{ return dynamic_cast<TNode*>(m_pNode); }
		inline TNode*			operator -> () const				{ return dynamic_cast<TNode*>(m_pNode); }
		inline IteratorType&	operator ++ ()						{ Next(); return *this; }

		inline	void	Restart()									{ Reset(dynamic_cast<TNode*>(m_pStartingNode)); }
		inline	bool	IsValid() const								{ return m_pNode == nullptr || m_pStartingNode != nullptr; }

		inline	void	Reset(TNode* pNode);

		virtual void	Next();

	protected:

		THierarchyNode*	m_pStartingNode;
		THierarchyNode*	m_pNode;
	};
}

// Implementation
#include "HierarchyNodeIterator.tpp"

