#pragma once

#include "Tests.hpp"

#include <Core/Namable.hpp>
#include <Graphics/SceneNode.hpp>

namespace ft
{
	namespace TestsSceneNode
	{
		struct Node : public SceneNode, public Namable
		{
			typedef HierarchyNodeIterator<		Node>	iterator;
			typedef HierarchyNodeIterator<const Node>	const_iterator;
		};

		uint32	All();
		uint32	Basics();
	}
}
