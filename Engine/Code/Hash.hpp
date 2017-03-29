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
		static inline Type	Compute(const T& tObj)
		{
			return static_cast<Type>(std::hash<T>()(tObj));
		}
	};
}
