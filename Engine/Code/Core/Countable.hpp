#pragma once

#include "Core/Core.hpp"
#include "Core/Types.hpp"

namespace ft
{
	// Référence dénombrable
	class Countable
	{
	protected:

		Countable() : m_iReferencesCount(0)							{}
		virtual ~Countable()										{}

	public:

		Countable(const Countable&) : m_iReferencesCount(0)			{}

		inline virtual	Countable&	operator = (const Countable&)	{ m_iReferencesCount = 0; return *this; }

		inline virtual	void		AddReference()					{ ++m_iReferencesCount; }
		inline virtual	void		ReleaseReference()				{ --m_iReferencesCount; FT_ASSERT((int32)m_iReferencesCount >= 0); }
		inline			uint32		GetReferenceCount() const		{ return m_iReferencesCount; }

	private:

		uint32	m_iReferencesCount;
	};
}
