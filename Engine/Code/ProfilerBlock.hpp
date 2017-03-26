#pragma once

#include "Types.hpp"
#include "Core.hpp"

#include <string>
#include <vector>

// la macro TICKS_PER_SECOND à vérifier et définir pour toutes les architectures
// certainement qu'il serait mieux de la ranger autre part

#if defined(_WIN32)
#	include <windows.h>
FT_STATIC_ASSERT(sizeof(LARGE_INTEGER) == sizeof(ft::uint64));

#	define TICKS_PER_SECOND		1000000
#else

#	define TICKS_PER_SECOND		1000000
#endif

namespace ft
{
	class ProfilerBlock
	{
	public:

		ProfilerBlock(const std::string& sName);
		virtual ~ProfilerBlock();

		void	Start();
		void	Step();

	protected:

		std::string			m_sName;

#if defined(_WIN32)
		LARGE_INTEGER		m_liFrequency;
		LARGE_INTEGER		m_liStartingTime;
		LARGE_INTEGER		m_liEndingTime;
#else

#endif

	private:

		ProfilerBlock() FT_DELETED;
		ProfilerBlock(const ProfilerBlock&) FT_DELETED;
		ProfilerBlock& operator = (const ProfilerBlock&) FT_DELETED;
	};

	class ProfilerBlockPrint : public ProfilerBlock
	{
	public:
		ProfilerBlockPrint(const std::string& sName);
		virtual ~ProfilerBlockPrint();

		void	Print(const std::string& sMessage = std::string());
	};
}
