
#include "TestsHierarchyNode.hpp"

#include <Core/Output.hpp>

//#define FT_TESTS_LOCAL_VERBOSE

namespace ft
{
	namespace TestsHierarchyNode
	{
		void	PrintGraph(Node::const_iterator itNode)
		{
			itNode.Restart();
#if defined(FT_TESTS_LOCAL_VERBOSE)
			FT_COUT << "------" << std::endl;
			while (*itNode != nullptr)
			{
				FT_COUT << (*itNode)->GetName() << std::endl;
				itNode.Next();
			}
			FT_COUT << "------" << std::endl;
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

			{
				Node::const_iterator	itNode;
				Node::Desc				oDesc;

				SPtr<Node> xRoot, x1, x2, x3, x11, x12, x21, x31, x311, x3111;

				xRoot = new Node();
				oDesc.pParent = nullptr;
				xRoot->Create(&oDesc);
				xRoot->SetName("Root");
				FT_UTEST(xRoot->GetParent() == nullptr);

				itNode.Reset(xRoot);
				FT_UTEST(*itNode == xRoot);
				PrintGraph(itNode);

				x1 = new Node();
				oDesc.pParent = xRoot;
				x1->Create(&oDesc);
				x1->SetName("1");
				FT_UTEST(x1->GetParent() == xRoot);
				PrintGraph(itNode);

				x2 = new Node();
				oDesc.pParent = xRoot;
				x2->Create(&oDesc);
				x2->SetName("2");
				FT_UTEST(x2->GetParent() == xRoot);
				PrintGraph(itNode);

				x3 = new Node();
				oDesc.pParent = xRoot;
				x3->Create(&oDesc);
				x3->SetName("3");
				FT_UTEST(x3->GetParent() == xRoot);
				PrintGraph(itNode);

				x11 = new Node();
				oDesc.pParent = x1;
				x11->Create(&oDesc);
				x11->SetName("11");
				FT_UTEST(x11->GetParent() == x1);
				PrintGraph(itNode);

				x12 = new Node();
				oDesc.pParent = x1;
				x12->Create(&oDesc);
				x12->SetName("12");
				FT_UTEST(x12->GetParent() == x1);
				PrintGraph(itNode);

				x21 = new Node();
				oDesc.pParent = x2;
				x21->Create(&oDesc);
				x21->SetName("21");
				FT_UTEST(x21->GetParent() == x2);
				PrintGraph(itNode);

				x31 = new Node();
				oDesc.pParent = x3;
				x31->Create(&oDesc);
				x31->SetName("31");
				FT_UTEST(x31->GetParent() == x3);
				PrintGraph(itNode);

				x311 = new Node();
				oDesc.pParent = x31;
				x311->Create(&oDesc);
				x311->SetName("311");
				FT_UTEST(x311->GetParent() == x31);
				PrintGraph(itNode);

				x3111 = new Node();
				oDesc.pParent = x311;
				x3111->Create(&oDesc);
				x3111->SetName("3111");
				FT_UTEST(x3111->GetParent() == x311);
				PrintGraph(itNode);
			}

			FT_UTEST_END;
		}
	}
}
