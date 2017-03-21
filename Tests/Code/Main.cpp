
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include "TestsSmartPtr.hpp"

int main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	using namespace ft;
	
	uint32 iRet = 0;
	iRet += Tests::SmartPtr::All();

	return 0;
}