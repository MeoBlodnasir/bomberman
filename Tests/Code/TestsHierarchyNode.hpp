#pragma once

#include "Tests.hpp"

#include <HierarchyNode.hpp>
#include <Namable.hpp>

namespace ft
{
	namespace Tests
	{
		namespace Hierarchy
		{
			struct Node : public HierarchyNode, public Namable
			{
				typedef HierarchyNodeIteratorTemplate<	    IHierarchyNode,	Node> iterator;
				typedef HierarchyNodeIteratorTemplate<const IHierarchyNode, Node> const_iterator;
			};

			uint32	All();
			uint32	Basics();
		}
	}
}
