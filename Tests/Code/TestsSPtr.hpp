#pragma once

#include "Tests.hpp"

#include <StrongPointer.hpp>

namespace ft
{
	namespace Tests
	{
		namespace StrongPointer
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
}
