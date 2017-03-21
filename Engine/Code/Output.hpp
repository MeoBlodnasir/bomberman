#pragma once

#include <iostream>

#if !defined (_MSC_VER)

#	define FT_COUT	std::cout
#	define FT_CERR	std::cerr

#else

#	include <windows.h>	// ::OutputDebugString()

namespace ft
{
	class VisualConsoleOutputBuffer : public std::streambuf
	{
	protected:

		virtual int	overflow(int c) override
		{
			static char sz[2] = {0};
			sz[0] = (char)c;
			::OutputDebugString(sz);
			return c;
		}

		virtual std::streamsize	sputn(const char *s, std::streamsize n)
		{
			::OutputDebugString(s);
			return n;
		}
	};

	class VisualConsoleOutput : public std::ostream
	{
	public:

		VisualConsoleOutputBuffer m_oBuffer;

		VisualConsoleOutput() : std::ostream(&m_oBuffer) {}
	};

	extern VisualConsoleOutput VSCOut;
}

#	define FT_COUT	ft::VSCOut
#	define FT_CERR	ft::VSCOut

#endif
