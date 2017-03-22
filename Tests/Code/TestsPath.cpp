
#include "TestsPath.hpp"

#include <Path.hpp>
#include <Output.hpp>

namespace ft
{
	namespace Tests
	{
		namespace Path
		{
			uint32	All()
			{
				uint32 iRet = 0;
				iRet += Basics();

				return iRet;
			}

			uint32	Basics()
			{
				FT_UTEST_INIT;

				ft::Path oPath0;
				ft::Path oPath1;
				ft::Path oPath2;

				oPath1.Set("./toto.lol");
				oPath2.Set("./toto/");

				FT_UTEST(oPath0.GetFullPath().size() == 0);
				//FT_UTEST(oPath1.GetFullPath() == ".\\toto.lol");
				//FT_UTEST(oPath2.GetFullPath() == ".\\toto\\");

				FT_UTEST(oPath0.GetFullName() == "");
				FT_UTEST(oPath1.GetFullName() == "toto.lol");
				FT_UTEST(oPath2.GetFullName() == "toto");

				FT_UTEST(oPath0.GetDirPath() == "");
				//FT_UTEST(oPath1.GetDirPath() == ".\\");
				//FT_UTEST(oPath2.GetDirPath() == ".\\");

				FT_UTEST(oPath0.GetName() == "");
				FT_UTEST(oPath1.GetName() == "toto");
				FT_UTEST(oPath2.GetName() == "toto");

				FT_UTEST(oPath0.GetExtension() == "");
				FT_UTEST(oPath1.GetExtension() == ".lol");
				FT_UTEST(oPath2.GetExtension() == "");

				FT_UTEST(oPath0.IsDirectory() == false);
				FT_UTEST(oPath1.IsDirectory() == false);
				FT_UTEST(oPath2.IsDirectory() == false);

				FT_UTEST(oPath0.DoesExist() == false);
				FT_UTEST(oPath1.DoesExist() == false);
				FT_UTEST(oPath2.DoesExist() == false);

				FT_UTEST(oPath0.IsValid() == false);
				FT_UTEST(oPath1.IsValid() == false);
				FT_UTEST(oPath2.IsValid() == false);

				FT_UTEST(oPath0.IsEmpty() == true);
				FT_UTEST(oPath1.IsEmpty() == false);
				FT_UTEST(oPath2.IsEmpty() == false);
				oPath0.Set("Tests/Assets/TestsPath/Fichier.txt");
				oPath1.Set("Tests/Assets/TestsPath/Dossier/");
				oPath2.Set("Tests/Assets/TestsPath/Dossier/Fichier.txt");
				

				//FT_UTEST(oPath0.GetFullPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/Fichier.txt");
				//FT_UTEST(oPath1.GetFullPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/Dossier/");
				//FT_UTEST(oPath2.GetFullPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/Dossier/Fichier.txt");

				//FT_UTEST(oPath0.GetFullPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\Fichier.txt");
				//FT_UTEST(oPath1.GetFullPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\Dossier\\");
				//FT_UTEST(oPath2.GetFullPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\Dossier\\Fichier.txt");

				FT_UTEST(oPath0.GetFullName() == "Fichier.txt");
				FT_UTEST(oPath1.GetFullName() == "Dossier");
				FT_UTEST(oPath2.GetFullName() == "Fichier.txt");

				//FT_UTEST(oPath0.GetDirPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/");
				//FT_UTEST(oPath1.GetDirPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/");
				//FT_UTEST(oPath2.GetDirPath() == "/home/gonzo/projects/bomberman/Tests/Assets/TestsPath/Dossier/");

				//FT_UTEST(oPath0.GetDirPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\");
				//FT_UTEST(oPath1.GetDirPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\");
				//FT_UTEST(oPath2.GetDirPath() == "D:\\42\\Bomberman\\git\\Tests\\Assets\\TestsPath\\Dossier\\");

				FT_UTEST(oPath0.GetName() == "Fichier");
				FT_UTEST(oPath1.GetName() == "Dossier");
				FT_UTEST(oPath2.GetName() == "Fichier");

				FT_UTEST(oPath0.GetExtension() == ".txt");
				FT_UTEST(oPath1.GetExtension() == "");
				FT_UTEST(oPath2.GetExtension() == ".txt");

				FT_UTEST(oPath0.IsDirectory() == false);
				FT_UTEST(oPath1.IsDirectory() == true);
				FT_UTEST(oPath2.IsDirectory() == false);

				FT_UTEST(oPath0.DoesExist() == true);
				FT_UTEST(oPath1.DoesExist() == true);
				FT_UTEST(oPath2.DoesExist() == true);

				FT_UTEST(oPath0.IsValid() == true);
				FT_UTEST(oPath1.IsValid() == true);
				FT_UTEST(oPath2.IsValid() == true);

				FT_UTEST(oPath0.IsEmpty() == false);
				FT_UTEST(oPath1.IsEmpty() == false);
				FT_UTEST(oPath2.IsEmpty() == false);
				// tester: 
				//void				Normalize();
				//void				Add(const std::string& s);
				FT_UTEST_END;
			}
		}
	}
}
