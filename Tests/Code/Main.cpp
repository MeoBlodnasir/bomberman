
#if defined(_WIN32)
#	include <crtdbg.h>
#endif

#include "TestsSPtr.hpp"
#include "TestsPath.hpp"
#include "TestsShader.hpp"
#include "TestsHierarchyNode.hpp"
#include "TestsSceneNode.hpp"

int main()
{
#if defined(_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	using namespace ft;
	
	uint32 iRet = 0;
	iRet += TestsStrongPointer::All();
	iRet += TestsPath::All();
	iRet += TestsShader::All();
	iRet += TestsHierarchyNode::All();
	iRet += TestsSceneNode::All();

	return 0;
}