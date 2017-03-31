
#include "Shader.hpp"

#include "OpenGL.hpp"
#include "ShaderFile.hpp"
#include "Output.hpp"

namespace ft
{
	Shader::Shader()
		: m_xResource(nullptr)
	{
	}

	Shader::~Shader()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Shader::Create(const SPtr<ShaderResource>& xResource)
	{
		FT_ASSERT(xResource != nullptr);

		FT_TEST_RETURN(xResource->IsLoadedAndValid(), FT_FAIL);
		m_xResource = xResource;

		return FT_OK;
	}

	ErrorCode	Shader::Destroy()
	{
		m_xResource = nullptr;
		return FT_OK;
	}

	ShaderResource::ShaderResource()
	{
	}

	ShaderResource::~ShaderResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	ShaderResource::IsLoadedAndValid() const
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_iHandle != 0;
	}

	ErrorCode	ShaderResource::Load(ResourceManager& /*oResourceManager*/, const ShaderResourceInfos& oInfos)
	{
		GLenum		eGLShaderType = GetGLShaderType(oInfos.eShaderType);
		GLuint		iHandle;
		std::string	sShaderSource;
		const char*	csShaderSource;
		{
			FT_TEST_RETURN(ShaderFile::ExtractContent(&sShaderSource, oInfos.oFilePath) == FT_OK, FT_FAIL);
			csShaderSource = sShaderSource.c_str();
		}

		FT_GL_ASSERT( iHandle = glCreateShader(eGLShaderType) );
		FT_GL_ASSERT( glShaderSource(iHandle, 1, &csShaderSource, NULL) );
		FT_GL_ASSERT( glCompileShader(iHandle) );
		{
			GLint	iSuccess;
			GLchar	csInfoLog[512];
			glGetShaderiv(iHandle, GL_COMPILE_STATUS, &iSuccess);
			if (!iSuccess)
			{
				glGetShaderInfoLog(iHandle, 512, NULL, csInfoLog);
				FT_CERR << "Compilation Shader " << oInfos.oFilePath << " echouee : " << csInfoLog << std::endl;
				FT_CERR << "Source compilee:\n" << csShaderSource << std::endl;
				return FT_FAIL;
			}
		}

		m_oResourceInfos = oInfos;
		m_iHandle = iHandle;

		return FT_OK;
	}

	ErrorCode	ShaderResource::Unload()
	{
		FT_GL_ASSERT( glDeleteShader(m_iHandle) );
		m_iHandle = 0;

		return FT_OK;
	}
}
