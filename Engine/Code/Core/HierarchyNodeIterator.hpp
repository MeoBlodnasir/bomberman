#pragma once

#include "Core/Core.hpp"
#include "Core/IHierarchyNode.hpp"

namespace ft
{
	template <class TNode>
	class HierarchyNodeIterator
	{
	public:

		typedef HierarchyNodeIterator<TNode>	IteratorType;

		HierarchyNodeIterator();
		HierarchyNodeIterator(TNode* pNode);
		HierarchyNodeIterator(const IteratorType& oIt);
		template <class TOther>
		HierarchyNodeIterator(const HierarchyNodeIterator<TOther>& oIt);
		virtual ~HierarchyNodeIterator();

				IteratorType&	operator = (const HierarchyNodeIterator<TNode>& oIt);
		template <class TOther>
				IteratorType&	operator = (const HierarchyNodeIterator<TOther>& oIt);

		inline					operator TNode* () const			{ return (TNode*)m_pNode; }
		inline	TNode*			operator * () const					{ return (TNode*)m_pNode; }
		inline	TNode*			operator -> () const				{ return (TNode*)m_pNode; }
		inline	IteratorType&	operator ++ ()						{ Next(); return *this; }

		inline	void			Restart()							{ Reset((TNode*)m_pStartingNode); }
		inline	bool			IsValid() const						{ return m_pNode == nullptr || m_pStartingNode != nullptr; }

		inline	void			Reset(TNode* pNode);

		virtual void			Next();

	protected:

		const IHierarchyNode*	m_pStartingNode;
		const IHierarchyNode*	m_pNode;
	};
}

// Implementation
#include "HierarchyNodeIterator.tpp"

