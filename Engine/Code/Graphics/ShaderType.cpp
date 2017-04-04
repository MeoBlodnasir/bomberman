
#include "Graphics/ShaderType.hpp"

#include "Graphics/OpenGL.hpp"

namespace ft
{
	GLenum		GetGLShaderType(EShaderType eShaderType)
	{
		switch (eShaderType)
		{
		case E_FRAGMENT_SHADER:	return GL_FRAGMENT_SHADER;
		case E_VERTEX_SHADER:	return GL_VERTEX_SHADER;
		default:				return (GLenum)-1;
		}
	}
}
