#pragma once

#include "Core/ErrorCode.hpp"

#include <string>

namespace ft
{
	// fw
	class Path;

	ErrorCode	LoadFileContent(std::string* pOutContent, const Path& oFilePath);
}
