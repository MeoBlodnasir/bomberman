#pragma once

#include "Types.hpp"

#include <functional> // std::hash

namespace ft
{
	namespace Hash
	{
		typedef uint32	Type;

		template <typename T>
		Type	Compute(const T& tObj)
		{
			return static_cast<Type>(std::hash<T>()(tObj));
		}
	}
}
