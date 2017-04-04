
#include "TestsSPtr.hpp"

#include <Core/Output.hpp>

//#define FT_TESTS_LOCAL_VERBOSE

namespace ft
{
	namespace TestsStrongPointer
	{
		CountableSPtrPrint::CountableSPtrPrint()
		{
#if defined(FT_TESTS_LOCAL_VERBOSE)
			FT_COUT << "ctor  CountableSPtrPrint()" << std::endl;
#endif
		}

		CountableSPtrPrint::~CountableSPtrPrint()
		{
#if defined(FT_TESTS_LOCAL_VERBOSE)
			FT_COUT << "dtor ~CountableSPtrPrint()" << std::endl;
#endif
		}

		uint32	All()
		{
			uint32 iRet = 0;
			iRet += Basics();

			return iRet;
		}

		uint32	Basics()
		{
			FT_UTEST_INIT;

			//DjoSPtr<NotCountableSPtr> xSPtr00 = new NotCountableSPtr; // Ne doit pas fonctionner

			SPtr<CountableSPtrPrint> xSPtr01 = nullptr;

			xSPtr01 = new CountableSPtrPrint;
			FT_UTEST(xSPtr01->GetReferenceCount() == 1);

			{
				SPtr<CountableSPtrPrint> xSPtr02 = nullptr;
				FT_UTEST(xSPtr01 != xSPtr02);
				FT_UTEST(xSPtr01.Ptr() != xSPtr02.Ptr());
				xSPtr02 = xSPtr01;
				FT_UTEST(xSPtr01->GetReferenceCount() == 2);
				FT_UTEST(xSPtr02->GetReferenceCount() == 2);
				FT_UTEST(xSPtr01 == xSPtr02);
				FT_UTEST(xSPtr01.Ptr() == xSPtr02.Ptr());
			}
			FT_UTEST(xSPtr01->GetReferenceCount() == 1);

			{
				CountableSPtrPrint* p01 = xSPtr01.Ptr();
				SPtr<CountableSPtrPrint> xSPtr02 = new CountableSPtrPrint;
				FT_UTEST(xSPtr01->GetReferenceCount() == 1);
				FT_UTEST(xSPtr02->GetReferenceCount() == 1);
				FT_UTEST(xSPtr01 != xSPtr02);
				FT_UTEST(xSPtr01.Ptr() != xSPtr02.Ptr());
				xSPtr02 = xSPtr01;
				FT_UTEST(xSPtr01->GetReferenceCount() == 2);
				FT_UTEST(xSPtr02->GetReferenceCount() == 2);
				FT_UTEST(xSPtr01 == p01);
				FT_UTEST(xSPtr02 == p01);
				FT_UTEST(xSPtr01 == xSPtr02);
				FT_UTEST(xSPtr01.Ptr() == xSPtr02.Ptr());
			}
			FT_UTEST(xSPtr01->GetReferenceCount() == 1);

			xSPtr01.Release();
			FT_UTEST(xSPtr01.Ptr() == nullptr);

			FT_UTEST_END;
		}
	}
}
