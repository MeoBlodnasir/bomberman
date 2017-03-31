
#include "ShaderResourceManager.hpp"

#include "ResourceManager.hpp"
#include "OpenGL.hpp"
#include "ShaderFile.hpp"
#include "Output.hpp"

namespace ft
{
	ErrorCode	ShaderFileResourceManager::Load_Impl(const ShaderFileResourceInfos& oInfos, SPtr<ShaderFileResource>& xOutResource)
	{
		ShaderFileResource*	pResource = nullptr;

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
				FT_CERR << "Sources a copiler:\n" << csShaderSource << std::endl;
				return FT_FAIL;
			}
		}

		pResource = new ShaderFileResource;
		pResource->m_oResourceInfos	= oInfos;
		pResource->m_iHandle		= iHandle;

		xOutResource = pResource;

		return FT_OK;
	}

	ErrorCode	ShaderFileResourceManager::Unload_Impl(ShaderFileResource* pResource)
	{
		FT_ASSERT(pResource != nullptr);

		FT_GL_ASSERT( glDeleteShader(pResource->m_iHandle) );

		return FT_OK;
	}

	ErrorCode	ShaderResourceManager::Load_Impl(const ShaderResourceInfos& oInfos, SPtr<ShaderResource>& xOutResource)
	{
		ShaderResource*				pResource = nullptr;
		SPtr<ShaderFileResource>	xVertexShaderResource = nullptr;
		SPtr<ShaderFileResource>	xFragmentShaderResource = nullptr;
		ShaderFileResourceInfos		oShaderFileResourceInfos;
		GLuint						iHandle;

		// Chargement des shaders individuels
		if (oInfos.iShaderTypesFlags & E_VERTEX_SHADER_FLAG)
		{
			oShaderFileResourceInfos.eShaderType = E_VERTEX_SHADER;
			oShaderFileResourceInfos.oFilePath = oInfos.oVertexShaderFilePath;
			FT_TEST_RETURN(m_oOwner.GetShaderFileResManager()->Load(oShaderFileResourceInfos, xVertexShaderResource) == FT_OK, FT_FAIL);
		}

		if (oInfos.iShaderTypesFlags & E_FRAGMENT_SHADER_FLAG)
		{
			oShaderFileResourceInfos.eShaderType = E_FRAGMENT_SHADER;
			oShaderFileResourceInfos.oFilePath = oInfos.oFragmentShaderFilePath;
			FT_TEST_RETURN(m_oOwner.GetShaderFileResManager()->Load(oShaderFileResourceInfos, xFragmentShaderResource) == FT_OK, FT_FAIL);
		}

		// Lien entre les shaders
		iHandle = glCreateProgram();
		if (oInfos.iShaderTypesFlags & E_VERTEX_SHADER_FLAG)
		{
			FT_GL_ASSERT( glAttachShader(iHandle, xVertexShaderResource->GetHandle()) );
		}
		if (oInfos.iShaderTypesFlags & E_FRAGMENT_SHADER_FLAG)
		{
			FT_GL_ASSERT( glAttachShader(iHandle, xFragmentShaderResource->GetHandle()) );
		}
		FT_GL_ASSERT( glLinkProgram(iHandle) );
		{
			GLint	iSuccess;
			GLchar	csInfoLog[512];
			glGetProgramiv(iHandle, GL_LINK_STATUS, &iSuccess);
			if (!iSuccess)
			{
				glGetProgramInfoLog(iHandle, 512, NULL, csInfoLog);
				FT_CERR << "Lien ShaderProgram échoué : " << csInfoLog << std::endl;
				return FT_FAIL;
			}
		}

		// Détachement des shaders
		if (oInfos.iShaderTypesFlags & E_VERTEX_SHADER_FLAG)
		{
			FT_GL_ASSERT( glDetachShader(iHandle, xVertexShaderResource->GetHandle()) );
		}
		if (oInfos.iShaderTypesFlags & E_FRAGMENT_SHADER_FLAG)
		{
			FT_GL_ASSERT( glDetachShader(iHandle, xFragmentShaderResource->GetHandle()) );
		}

		pResource = new ShaderResource;
		pResource->m_oResourceInfos	= oInfos;
		pResource->m_iHandle		= iHandle;

		xOutResource = pResource;

		return FT_OK;
	}

	ErrorCode	ShaderResourceManager::Unload_Impl(ShaderResource* pResource)
	{
		FT_GL_ASSERT( glDeleteProgram(pResource->m_iHandle) );

		return FT_OK;
	}
}
