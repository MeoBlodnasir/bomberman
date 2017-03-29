#pragma once

#include "Types.hpp"

namespace ft
{
	enum EShaderType
	{
		E_INVALID_SHADER_TYPE = -1,

		E_VERTEX_SHADER = 0,
		E_FRAGMENT_SHADER,

		E_SHADER_TYPE_COUNT
	};

#define SHADER_TYPE_FLAG(shaderType)		(1 << shaderType)

	enum EShaderTypeFlag
	{
		E_VERTEX_SHADER_FLAG	= SHADER_TYPE_FLAG(E_VERTEX_SHADER),
		E_FRAGMENT_SHADER_FLAG	= SHADER_TYPE_FLAG(E_FRAGMENT_SHADER)
	};

	uint32	GetGLShaderType(EShaderType eShaderType);
}
