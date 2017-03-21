#pragma once

#include <string>

namespace ft
{
	class Namable
	{
	public:

		Namable() : m_sName()												{}
		Namable(const Namable& oNamable) : m_sName(oNamable.m_sName)		{}
		Namable(const std::string& sName) : m_sName(sName)					{}
		Namable(const char* csName) : m_sName(csName)						{}
		virtual ~Namable()													{}

		inline Namable&				operator = (const Namable& oNamable)	{ m_sName = oNamable.m_sName; return *this; }

		inline void					SetName(const std::string& sName)		{ m_sName = sName; }
		inline const std::string&	GetName()								{ return m_sName; }

	protected:

		std::string		m_sName;
	};
}
