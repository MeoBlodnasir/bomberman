
#include "TestsSceneNode.hpp"

#include <Core/Output.hpp>
#include <Math/MathStructOutput.hpp>
#include <Graphics/SceneNode.hpp>

//#define FT_TESTS_LOCAL_VERBOSE

namespace ft
{
	namespace TestsSceneNode
	{
		void	PrintGraph(Node::const_iterator itNode)
		{
			itNode.Restart();
#if defined(FT_TESTS_LOCAL_VERBOSE)
			FT_COUT << "------" << std::endl;
			while (*itNode != nullptr)
			{
				FT_COUT << (*itNode)->GetName() << std::endl;
				FT_COUT << "World:\n" << (*itNode)->GetWorldTransform() << std::endl;
				FT_COUT << "Local:\n" << (*itNode)->GetLocalTransform() << std::endl;
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

			Node::const_iterator	itNode;
			Node::Desc				oDesc;

			SPtr<Node> xRoot, x1, x2, x3, x11, x12, x21, x31, x311, x3111;

			xRoot = new Node();
			oDesc.pParent = nullptr;
			xRoot->Create(&oDesc);
			xRoot->SetName("Root");
			xRoot->Update();

			x1 = new Node();
			oDesc.pParent = xRoot;
			x1->Create(&oDesc);
			x1->SetName("1");
			x1->Update();

			x2 = new Node();
			oDesc.pParent = xRoot;
			x2->Create(&oDesc);
			x2->SetName("2");
			x2->Update();

			x3 = new Node();
			oDesc.pParent = xRoot;
			x3->Create(&oDesc);
			x3->SetName("3");
			x3->Update();

			x11 = new Node();
			oDesc.pParent = x1;
			x11->Create(&oDesc);
			x11->SetName("11");
			x11->Update();

			x12 = new Node();
			oDesc.pParent = x1;
			x12->Create(&oDesc);
			x12->SetName("12");
			x12->Update();

			x21 = new Node();
			oDesc.pParent = x2;
			x21->Create(&oDesc);
			x21->SetName("21");
			x21->Update();

			x31 = new Node();
			oDesc.pParent = x3;
			x31->Create(&oDesc);
			x31->SetName("31");
			x31->Update();

			x311 = new Node();
			oDesc.pParent = x31;
			x311->Create(&oDesc);
			x311->SetName("311");
			x311->Update();

			x3111 = new Node();
			oDesc.pParent = x311;
			x3111->Create(&oDesc);
			x3111->SetName("3111");
			x3111->Update();

			itNode.Reset(xRoot);
			PrintGraph(itNode);

			x31->SetLocalTransform(glm::rotate(x31->GetLocalTransform(), glm::radians(90.f), Vector3(0.f, 0.f, 1.f)));
			SceneNode::UpdateHierarchy(x31);
			PrintGraph(itNode);

			FT_UTEST_END;
		}
	}
}
