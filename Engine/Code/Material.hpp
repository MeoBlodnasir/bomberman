#pragma once

#include "StrongPointer.hpp"

namespace ft
{
	class Material : public CountableSPtr
	{
	public:

		Material();
		virtual ~Material();
	};
}
