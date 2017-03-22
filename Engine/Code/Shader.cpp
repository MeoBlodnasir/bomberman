
#include "Shader.hpp"

#include "Output.hpp"
#include "OpenGL.hpp"
#include "ShaderFile.hpp"

namespace ft
{
	Shader::Shader()
		: m_eShaderType(E_INVALID_SHADER_TYPE)
	{
	}

	Shader::~Shader()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Shader::Create(EShaderType eShaderType, const Path& oSourceFilePath)
	{
		GLenum		eGLShaderType = GetGLShaderType(eShaderType);
		GLuint		iShaderHandle;
		std::string	sShaderSource;
		const char*	csShaderSource;
		{
			FT_TEST_RETURN(ShaderFile::ExtractContent(&sShaderSource, oSourceFilePath) == FT_OK, FT_FAIL);
			csShaderSource = sShaderSource.c_str();
		}

		FT_GL_ASSERT( iShaderHandle = glCreateShader(eGLShaderType) );
		FT_GL_ASSERT( glShaderSource(iShaderHandle, 1, &csShaderSource, NULL) );
		FT_GL_ASSERT( glCompileShader(iShaderHandle) );
		{
			GLint	iSuccess;
			GLchar	csInfoLog[512];
			glGetShaderiv(iShaderHandle, GL_COMPILE_STATUS, &iSuccess);
			if (!iSuccess)
			{
				glGetShaderInfoLog(iShaderHandle, 512, NULL, csInfoLog);
				FT_COUT << "Compilation Shader " << oSourceFilePath << " échouée : " << csInfoLog << std::cout;
				return FT_FAIL;
			}
		}

		m_oSourceFilePath = oSourceFilePath;
		m_eShaderType	  = eShaderType;
		m_iHandle		  = iShaderHandle;
		return FT_OK;
	}

	ErrorCode	Shader::Destroy()
	{
		FT_GL_ASSERT( glDeleteShader(m_iHandle) );
		m_eShaderType = E_INVALID_SHADER_TYPE;
		return Handled::Destroy();
	}

	uint32		Shader::GetGLShaderType(EShaderType eShaderType)
	{
		switch (eShaderType)
		{
		case E_VERTEX_SHADER:	return GL_VERTEX_SHADER;
		case E_FRAGMENT_SHADER:	return GL_FRAGMENT_SHADER;
		default:				return (uint32)-1;
		}
	}
}
