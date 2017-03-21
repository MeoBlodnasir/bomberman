
#include "Core.hpp"

#include "Output.hpp"

namespace Djo
{
	void	PrintAssertInfos(const char* csExpression, const char* csFilePath, const int iLine, const char* csFunction)
	{
		FT_CERR << "############## ASSERTION ECHOUEE ##############\n";
		FT_CERR << "Fichier:  " << csFilePath << " (" << iLine << ")\n";
		FT_CERR << "Fonction: " << csFunction << "\n\n";
		FT_CERR << csExpression << "\n\n";
		FT_CERR << "###############################################" << std::endl;
	}
}
