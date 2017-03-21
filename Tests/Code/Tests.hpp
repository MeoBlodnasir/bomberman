#pragma once

#include <Core.hpp>
#include <Types.hpp>

// Configuration //////////////////////////////////////////////////////////////

//#define FT_ENABLE_BREAK_AT_ASSERT
#if defined(FT_ENABLE_BREAK_AT_ASSERT)
#	define FT_UTEST_ASSERT		FT_ASSERT
#else
#	define FT_UTEST_ASSERT
#endif

///////////////////////////////////////////////////////////////////////////////

namespace ft
{
	namespace Tests
	{
		void	TestFailLog(const char* csFilePath, const int iLine, const char* csFunction);
		void	TestEndLog(const char* csFunction, const uint32 iTestCount, const uint32 iSuccessful);
	}
}

#define FT_UTEST_INIT		ft::uint32 iTestCount__ = 0, iSuccessfulTests__ = 0

#define FT_UTEST(expr)				\
{									\
	bool bUTest__ = (expr);			\
	++iTestCount__;					\
	FT_UTEST_ASSERT(bUTest__);		\
	if (bUTest__)					\
	{								\
		++iSuccessfulTests__;		\
	}								\
	else							\
	{								\
		ft::Tests::TestFailLog(__FILE__, __LINE__, __FUNCTION__);\
	}								\
}

#define FT_UTEST_RETURN(expr, ret)	\
{									\
	bool bUTest__ = (expr);			\
	++iTestCount__;					\
	FT_UTEST_ASSERT(bUTest__);		\
	if (bUTest__)					\
	{								\
		++iSuccessfulTests__;		\
	}								\
	else							\
	{								\
		ft::Tests::TestFailLog(__FILE__, __LINE__, __FUNCTION__);\
		return (ret);				\
	}								\
}

#define FT_UTEST_END				\
{									\
	ft::Tests::TestEndLog(__FUNCTION__, iTestCount__, iSuccessfulTests__);\
	return (iTestCount__ - iSuccessfulTests__);\
}
