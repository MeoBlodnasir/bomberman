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
		};

		uint32	All();
		uint32	Basics();
	}
}
