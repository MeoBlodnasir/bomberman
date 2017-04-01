#pragma once

#include "Types.hpp"
#include "Core.hpp"
#include "ErrorCode.hpp"

namespace ft
{
	class Handled
	{
	public:

		Handled() : m_iHandle(0)						{}
		virtual ~Handled()								{}

		inline virtual	ErrorCode	Destroy()			{ m_iHandle = 0; return FT_OK; }

		inline			bool		IsHandled() const	{ return m_iHandle != 0; }
		inline			uint32		GetHandle() const	{ return m_iHandle; }

	protected:

		uint32	m_iHandle;

	private:

		Handled(const Handled&) FT_DELETED;
		Handled& operator = (const Handled&) FT_DELETED;
	};
}
