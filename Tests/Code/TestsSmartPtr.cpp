
#include "TestsSmartPtr.hpp"

#include <Output.hpp>

namespace ft
{
	namespace Tests
	{
		namespace SmartPtr
		{
			uint32	All()
			{
				uint32 iRet = 0;
				iRet += SharedPtr();

				return iRet;
			}

			uint32	SharedPtr()
			{
				FT_UTEST_INIT;

				Sptr<int32> xSPtr01 = nullptr;

				xSPtr01.reset(new int32);
				FT_UTEST(xSPtr01.use_count() == 1);

				{
					Sptr<int32> xSPtr02 = nullptr;
					FT_UTEST(xSPtr01 != xSPtr02);
					FT_UTEST(xSPtr01.get() != xSPtr02.get());
					xSPtr02 = xSPtr01;
					FT_UTEST(xSPtr01.use_count() == 2);
					FT_UTEST(xSPtr02.use_count() == 2);
					FT_UTEST(xSPtr01 == xSPtr02);
					FT_UTEST(xSPtr01.get() == xSPtr02.get());
				}
				FT_UTEST(xSPtr01.use_count() == 1);

				{
					int32* p01 = xSPtr01.get();
					Sptr<int32> xSPtr02(new int32);
					FT_UTEST(xSPtr01.use_count() == 1);
					FT_UTEST(xSPtr02.use_count() == 1);
					FT_UTEST(xSPtr01 != xSPtr02);
					FT_UTEST(xSPtr01.get() != xSPtr02.get());
					xSPtr02 = xSPtr01;
					FT_UTEST(xSPtr01.use_count() == 2);
					FT_UTEST(xSPtr02.use_count() == 2);
					FT_UTEST(xSPtr01.get() == p01);
					FT_UTEST(xSPtr02.get() == p01);
					FT_UTEST(xSPtr01 == xSPtr02);
					FT_UTEST(xSPtr01.get() == xSPtr02.get());
				}
				FT_UTEST(xSPtr01.use_count() == 1);

				xSPtr01.reset();
				FT_UTEST(xSPtr01.get() == nullptr);

				FT_UTEST_END;
			}
		}
	}
}
