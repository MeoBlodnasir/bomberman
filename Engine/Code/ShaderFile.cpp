
#include "ShaderFile.hpp"

#include "Core.hpp"
#include "Path.hpp"
#include "File.hpp"

#include <unordered_set>

namespace ft
{
	namespace ShaderFile
	{
		static
		ErrorCode	EmulateInclude(std::string* pOutContent,
									std::unordered_set<std::string>* pOutFileSet,
									const std::string& sSourceFileDirPath,
									const std::string& sSource)
		{
			FT_ASSERT(pOutContent != NULL);
			FT_ASSERT(pOutFileSet != NULL);

			static const char s_csInclude[]	= "#include";
			static const char s_iQuote = '"';
			static const char s_iEol = '\n';

			uint32 iFileSetSize;

			std::string::size_type iPosInclude = 0;
			std::string::size_type iPosEol = 0;
			std::string::size_type iPosFileBegin = 0;
			std::string::size_type iPosFileEnd = 0;

			std::string	sIncludedSourceBeforeInclude;
			std::string	sIncludedSource;
			Path		oIncludedFilePath;

			*pOutContent = sSource;

			while (iPosInclude != std::string::npos)
			{
				// Récupération de la première occurence de "#include"
				iPosInclude = pOutContent->find(s_csInclude, iPosInclude);
				if (iPosInclude != std::string::npos)
				{
					// Vérification qu'elle est bien en début de ligne, sinon on ignore
					if (iPosInclude == 0 || (*pOutContent)[iPosInclude - 1] == s_iEol)
					{
						// Récupération du nom du fichier
						iPosEol = pOutContent->find_first_of(s_iEol, iPosInclude);
						iPosFileBegin = pOutContent->find_first_of(s_iQuote, iPosInclude);
						FT_TEST_RETURN(iPosFileBegin < iPosEol, FT_FAIL);
						++iPosFileBegin;
						iPosFileEnd = pOutContent->find_first_of(s_iQuote, iPosFileBegin);
						FT_TEST_RETURN(iPosFileEnd < iPosEol, FT_FAIL);

						if (oIncludedFilePath.Set(sSourceFileDirPath + pOutContent->substr(iPosFileBegin, iPosFileEnd - iPosFileBegin)))
						{
							sIncludedSource.clear();
							iFileSetSize = pOutFileSet->size();
							pOutFileSet->insert(oIncludedFilePath.GetFullPath());
							if (pOutFileSet->size() > iFileSetSize)
							{
								FT_TEST_RETURN(LoadFileContent(&sIncludedSourceBeforeInclude, oIncludedFilePath) == FT_OK, FT_FAIL);
								FT_TEST_RETURN(EmulateInclude(&sIncludedSource, pOutFileSet, oIncludedFilePath.GetDirPath(), sIncludedSourceBeforeInclude) == FT_OK, FT_FAIL);
							}
							// Remplace par le contenu si applicable, sinon remplace par du vide
							pOutContent->replace(iPosInclude, iPosEol - iPosInclude, sIncludedSource);
						}
					}
					++iPosInclude;
				}
			}

			return FT_OK;
		}

		ErrorCode	ExtractContent(std::string* pOutContent, const Path& oFilePath)
		{
			FT_ASSERT(pOutContent != NULL);

			std::unordered_set<std::string> oFileSet;
			std::string						sSource;

			FT_TEST_RETURN(LoadFileContent(pOutContent, oFilePath) == FT_OK, FT_FAIL);
			oFileSet.insert(oFilePath.GetFullPath());
			sSource = *pOutContent;

			FT_TEST_RETURN(EmulateInclude(pOutContent, &oFileSet, oFilePath.GetDirPath(), sSource) == FT_OK, FT_FAIL);

			return FT_OK;
		}
	}
}
