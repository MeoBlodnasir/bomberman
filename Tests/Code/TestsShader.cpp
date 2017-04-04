
#include "TestsShader.hpp"

#include <Core/File.hpp>
#include <Core/Output.hpp>
#include <Core/Path.hpp>
#include <Graphics/ShaderFile.hpp>

namespace ft
{
	namespace TestsShader
	{
		uint32	All()
		{
			uint32 iRet = 0;
			iRet += EmulateInclude();

			return iRet;
		}

		uint32	EmulateInclude()
		{
			FT_UTEST_INIT;

			// Fichier vide
			Path oShader00("./Tests/Assets/TestsShader/EmulateInclude_00.txt");
			// Fichier avec uniquement un include de fichier vide (donc vide après include)
			Path oShader01("./Tests/Assets/TestsShader/EmulateInclude_01.txt");
			// Fichier avec un include de "Fichier.txt"
			Path oShader02("./Tests/Assets/TestsShader/EmulateInclude_02.txt");
			Path oShader02Res("./Tests/Assets/TestsShader/EmulateInclude_02_resultat.txt");
			// Fichier avec un include pas en début de ligne (pas pris en compte)
			Path oShader03("./Tests/Assets/TestsShader/EmulateInclude_03.txt");
			// Fichier avec des includes chainés dans des dossiers différents
			Path oShader04("./Tests/Assets/TestsShader/EmulateInclude_04.txt");
			Path oShader04Res("./Tests/Assets/TestsShader/EmulateInclude_04_resultat.txt");
			// Fichier avec des includes bouclés
			Path oShader07("./Tests/Assets/TestsShader/EmulateInclude_07.txt");
			Path oShader07Res("./Tests/Assets/TestsShader/EmulateInclude_07_resultat.txt");

			std::string	s1;
			std::string s2;

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader00) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader00) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader01) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader00) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader02) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader02Res) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader03) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader03) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader04) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader04Res) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST(ShaderFile::ExtractContent(&s1, oShader07) == FT_OK);
			FT_UTEST(LoadFileContent(&s2, oShader07Res) == FT_OK);
			FT_UTEST(s1 == s2);

			FT_UTEST_END;
		}
	}
}
