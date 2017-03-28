#pragma once

#include "Types.hpp"

#include <functional> // std::hash

namespace ft
{
	class Hash
	{
	public:

		typedef uint32	Type;

		template <typename T>
		static Type		Compute(const T& tObj)
		{
			std::hash<T> oHasher;
			return static_cast<Type>(oHasher(tObj));
		}
	};
}
