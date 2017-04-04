
#include "Core/File.hpp"

#include "Core/Path.hpp"
#include "Core/Output.hpp"

#include <fstream>
#include <sstream>

namespace ft
{
	ErrorCode	LoadFileContent(std::string* pOutContent, const Path& oFilePath)
	{
		std::ifstream		oIFStream;
		std::stringstream	oSStream;

		oIFStream.open(oFilePath.GetFullPath());
		if (oIFStream.rdstate() & std::ifstream::failbit)
		{
			FT_CERR << "Echec dans l'ouverture du fichier: " << oFilePath << std::endl;
			return FT_FAIL;
		}
		oSStream.clear();
		oSStream << oIFStream.rdbuf();
		oIFStream.close();

		*pOutContent = oSStream.str();

		return FT_OK;
	}
}
