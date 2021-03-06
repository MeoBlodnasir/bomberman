#pragma once

#include "Tests.hpp"

#include <Core/StrongPointer.hpp>

namespace ft
{
	namespace TestsStrongPointer
	{
		struct CountableSPtrPrint : public CountableSPtr
		{
			uint32	m_iSomeVariable;

			CountableSPtrPrint();
			~CountableSPtrPrint();
		};

		struct NotCountableSPtr
		{
			uint32	m_iSomeVariable;
		};

		uint32	All();
		uint32	Basics();
	}
}
