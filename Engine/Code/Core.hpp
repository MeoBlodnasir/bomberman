#pragma once

#if defined (_WIN32)
#	include <intrin.h>	// __debugbreak()
#endif

// Implementation C++ (à voir)
#define FT_DELETED
//#define FT_DELETED						= delete

// Outils macro
#define FT_STRINGIFY2(s)				#s
#define FT_STRINGIFY(s)					FT_STRINGIFY2(s)

// Break
#if defined (_WIN32)
#	define FT_BREAK_CPU()				__debugbreak()
#else
#	define FT_BREAK_CPU()				asm volatile ("int $3")
#endif

// Todo
#if defined (_WIN32)
#	define FT_TODO(s)					__pragma(message(__FILE__ "(" FT_STRINGIFY(__LINE__) ") : A FAIRE: " s))
#else
#	define FT_TODO(s)
#endif

namespace ft
{
	void	PrintAssertInfos(const char* csExpression, const char* csFilePath, const int iLine, const char* csFunction);
}

#if defined (__FT_DEBUG__)

#	define FT_FAILED_ASSERTION(expr)		\
	{										\
		ft::PrintAssertInfos(#expr, __FILE__, __LINE__, __FUNCTION__);	\
		FT_BREAK_CPU();					\
	}

// tester: #define FT_ASSERT(expr)			(void)(!!(expr) || FT_FAILED_ASSERTION(expr))
#	define FT_ASSERT(expr)					\
	{										\
		if (!(expr))						\
		{									\
			FT_FAILED_ASSERTION(expr);		\
		}									\
	}

#	define FT_STATIC_ASSERT(expr)		typedef char __FT_S_ASSERT__[(expr) ? 1 : -1]

#else

#	define FT_FAILED_ASSERTION(expr)
#	define FT_ASSERT(expr)
#	define FT_STATIC_ASSERT(expr)

#endif

#define FT_TEST(expr)						\
{											\
	if (!(expr))							\
	{										\
		FT_FAILED_ASSERTION(expr);			\
	}										\
}

#define FT_TEST_RETURN(expr, ret)			\
{											\
	if (!(expr))							\
	{										\
		FT_FAILED_ASSERTION(expr);			\
		return ret;							\
	}										\
}

#define FT_NOT_IMPLEMENTED(mess)	{ FT_TODO(mess); FT_FAILED_ASSERTION(mess); }

#define FT_DELETE(ptr)				{ delete ptr; ptr = nullptr; }
#define FT_DELETE_ARRAY(ptr)		{ delete [] ptr; ptr = nullptr; }
#define FT_SAFE_DELETE(ptr)			{ if (ptr != nullptr) { FT_DELETE(ptr); } }
#define FT_SAFE_DELETE_ARRAY(ptr)	{ if (ptr != nullptr) { FT_DELETE_ARRAY(ptr); } }
