#pragma once

#include "Tests.hpp"

#include <Core/HierarchyNode.hpp>
#include <Core/Namable.hpp>

namespace ft
{
	namespace TestsHierarchyNode
	{
		struct Node : public HierarchyNode<Node>, public Namable
		{
		};

		uint32	All();
		uint32	Basics();
	}
}
