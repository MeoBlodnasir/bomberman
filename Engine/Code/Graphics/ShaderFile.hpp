#pragma once

#include "Core/ErrorCode.hpp"

#include <string>

namespace ft
{
	// fw
	class Path;

	namespace ShaderFile
	{
		ErrorCode	ExtractContent(std::string* pOutContent, const Path& oFilePath);
	}
}
