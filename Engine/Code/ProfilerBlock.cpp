
#include "ProfilerBlock.hpp"

#include "Output.hpp"

#if defined(_WIN32)
#	include <profileapi.h>
#else

#endif

namespace ft
{
	ProfilerBlock::ProfilerBlock(const std::string& sName)
		: m_sName(sName)
	{
		Start();
	}

	ProfilerBlock::~ProfilerBlock()
	{
	}

	void	ProfilerBlock::Start()
	{
#if defined(_WIN32)
		::QueryPerformanceFrequency(&m_liFrequency); 
		::QueryPerformanceCounter(&m_liStartingTime);
#else

#endif
	}

	void	ProfilerBlock::Step()
	{
#if defined(_WIN32)
		::QueryPerformanceCounter(&m_liEndingTime);
#else

#endif
	}


	ProfilerBlockPrint::ProfilerBlockPrint(const std::string& sName)
		: ProfilerBlock(sName)
	{
	}

	ProfilerBlockPrint::~ProfilerBlockPrint()
	{
		Print();
	}

	void	ProfilerBlockPrint::Print(const std::string& sMessage /*= std::string*/)
	{
		Step();

		uint64	iElapsedTimeMs;
#if defined(_WIN32)
		iElapsedTimeMs = ((m_liEndingTime.QuadPart - m_liStartingTime.QuadPart) * (TICKS_PER_SECOND / 1000)) / m_liFrequency.QuadPart;
#else
		iElapsedTimeMs = 0;
#endif
		FT_COUT << "TimerInfos : " << m_sName << " " << sMessage;
		FT_COUT << "\n t : " << iElapsedTimeMs << "ms" << std::endl;
	}
}
