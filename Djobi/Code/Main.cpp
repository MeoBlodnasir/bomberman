
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

int TestsRender();
int TestsGameplay();

int		main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//long	iBreakAlloc = -1;
	//_CrtSetBreakAlloc(iBreakAlloc);
#endif

	return TestsRender();
	//return TestsGameplay();
}
