
#include "ShaderProgram.hpp"

#include "Shader.hpp"
#include "OpenGL.hpp"
#include "Texture.hpp"
#include "ResourceManager.hpp"
#include "SpecificResourceManager.hpp"
#include "Output.hpp"

namespace ft
{
	ShaderProgram::ShaderProgram()
		: m_xResource(nullptr)
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	ShaderProgram::Create(const SPtr<ShaderProgramResource>& xResource)
	{
		FT_ASSERT(xResource != nullptr);

		FT_TEST_RETURN(xResource->IsLoadedAndValid(), FT_FAIL);
		m_xResource = xResource;

		return FT_OK;
	}

	ErrorCode	ShaderProgram::Destroy()
	{
		m_xResource = nullptr;
		return FT_OK;
	}

	void	ShaderProgram::Use() const
	{
		FT_ASSERT(m_xResource->IsLoadedAndValid());
		FT_GL_ASSERT( glUseProgram(m_xResource->GetHandle()) );
	}

	void	ShaderProgram::SetUniform(const char* csName, float32 f) const
	{
		FT_GL_ASSERT( glUniform1f(glGetUniformLocation(m_xResource->GetHandle(), csName), f) );
	}

	void	ShaderProgram::SetUniform(const char* csName, int32 i) const
	{
		FT_GL_ASSERT( glUniform1i(glGetUniformLocation(m_xResource->GetHandle(), csName), i) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const float32* p, uint32 iCount /*= 1*/) const
	{
		FT_GL_ASSERT( glUniform1fv(glGetUniformLocation(m_xResource->GetHandle(), csName), iCount, p) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const int32* p, uint32 iCount /*= 1*/) const
	{
		FT_GL_ASSERT( glUniform1iv(glGetUniformLocation(m_xResource->GetHandle(), csName), iCount, p) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector2& v) const
	{
		FT_GL_ASSERT( glUniform2fv(glGetUniformLocation(m_xResource->GetHandle(), csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector3& v) const
	{
		FT_GL_ASSERT( glUniform3fv(glGetUniformLocation(m_xResource->GetHandle(), csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector4& v) const
	{
		FT_GL_ASSERT( glUniform4fv(glGetUniformLocation(m_xResource->GetHandle(), csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Matrix33& m) const
	{
		FT_GL_ASSERT( glUniformMatrix3fv(glGetUniformLocation(m_xResource->GetHandle(), csName), 1, GL_FALSE, (const GLfloat*)&m) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Matrix44& m) const
	{
		FT_GL_ASSERT( glUniformMatrix4fv(glGetUniformLocation(m_xResource->GetHandle(), csName), 1, GL_FALSE, (const GLfloat*)&m) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const SPtr<Texture>& xTexture, uint32 iTextureUnit) const
	{
		FT_ASSERT(xTexture != nullptr);
		xTexture->Bind(iTextureUnit);

		FT_GL_ASSERT( glUniform1i(glGetUniformLocation(m_xResource->GetHandle(), csName), iTextureUnit) );
	}

	ShaderProgramResource::ShaderProgramResource()
	{
	}

	ShaderProgramResource::~ShaderProgramResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	ShaderProgramResource::IsLoadedAndValid() const
	{
		return m_iHandle != 0;
	}

	ErrorCode	ShaderProgramResource::Load(ResourceManager& oResourceManager, const ShaderProgramResourceInfos& oInfos)
	{
		SPtr<ShaderResource>	xVertexShaderResource = nullptr;
		SPtr<ShaderResource>	xFragmentShaderResource = nullptr;
		ShaderResourceInfos		oShaderResourceInfos;
		GLuint					iHandle;

		// Chargement des shaders individuels
		if (oInfos.iShaderTypesFlags & E_VERTEX_SHADER_FLAG)
		{
			oShaderResourceInfos.eShaderType = E_VERTEX_SHADER;
			oShaderResourceInfos.oFilePath = oInfos.oVertexShaderFilePath;
			FT_TEST_RETURN(oResourceManager.GetShaderResourceManager()->Load(oShaderResourceInfos, xVertexShaderResource) == FT_OK, FT_FAIL);
		}

		if (oInfos.iShaderTypesFlags & E_FRAGMENT_SHADER_FLAG)
		{
			oShaderResourceInfos.eShaderType = E_FRAGMENT_SHADER;
			oShaderResourceInfos.oFilePath = oInfos.oFragmentShaderFilePath;
			FT_TEST_RETURN(oResourceManager.GetShaderResourceManager()->Load(oShaderResourceInfos, xFragmentShaderResource) == FT_OK, FT_FAIL);
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

		m_oResourceInfos = oInfos;
		m_iHandle = iHandle;

		return FT_OK;
	}

	ErrorCode	ShaderProgramResource::Unload()
	{
		FT_GL_ASSERT( glDeleteProgram(m_iHandle) );
		m_iHandle = 0;

		return FT_OK;
	}
}
