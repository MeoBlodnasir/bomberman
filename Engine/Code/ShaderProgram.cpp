
#include "ShaderProgram.hpp"

#include "Output.hpp"
#include "OpenGL.hpp"
#include "Texture.hpp"

namespace ft
{
	ShaderProgram::ShaderProgram()
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	ShaderProgram::Create(const Shader* pVertexShader, const Shader* pFragmentShader /*= nullptr*/)
	{
		FT_ASSERT(pVertexShader != nullptr);
		FT_ASSERT(pVertexShader->GetShaderType() == E_VERTEX_SHADER);
		FT_ASSERT(pVertexShader->IsHandled());

		GLuint iShaderProgramHandle = glCreateProgram();

		FT_GL_ASSERT( glAttachShader(iShaderProgramHandle, pVertexShader->GetHandle()) );

		if (pFragmentShader != nullptr)
		{
			FT_ASSERT(pFragmentShader->GetShaderType() == E_FRAGMENT_SHADER);
			FT_ASSERT(pFragmentShader->IsHandled());
			FT_GL_ASSERT( glAttachShader(iShaderProgramHandle, pFragmentShader->GetHandle()) );
		}

		FT_GL_ASSERT( glLinkProgram(iShaderProgramHandle) );
		{
			GLint	iSuccess;
			GLchar	csInfoLog[512];
			glGetProgramiv(iShaderProgramHandle, GL_LINK_STATUS, &iSuccess);
			if (!iSuccess)
			{
				glGetProgramInfoLog(iShaderProgramHandle, 512, NULL, csInfoLog);
				FT_CERR << "Lien ShaderProgram échoué : " << csInfoLog << std::endl;
				return FT_FAIL;
			}
		}

		m_aShaders[E_VERTEX_SHADER]	  = pVertexShader;
		m_aShaders[E_FRAGMENT_SHADER] = pFragmentShader;
		m_iHandle = iShaderProgramHandle;
		return FT_OK;
	}

	ErrorCode	ShaderProgram::Destroy()
	{
		for (uint32 i = E_VERTEX_SHADER; i < E_SHADER_TYPE_COUNT; ++i)
		{
			if (m_aShaders[i] != nullptr)
			{
				FT_GL_ASSERT( glDetachShader(m_iHandle, m_aShaders[i]->GetHandle()) );
			}
		}
		FT_GL_ASSERT( glDeleteProgram(m_iHandle) );

		return FT_OK;
	}

	void	ShaderProgram::Use() const
	{
		FT_ASSERT(IsHandled());
		FT_GL_ASSERT( glUseProgram(m_iHandle) );
	}

	void	ShaderProgram::SetUniform(const char* csName, float32 f) const
	{
		FT_GL_ASSERT( glUniform1f(glGetUniformLocation(m_iHandle, csName), f) );
	}

	void	ShaderProgram::SetUniform(const char* csName, int32 i) const
	{
		FT_GL_ASSERT( glUniform1i(glGetUniformLocation(m_iHandle, csName), i) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const float32* p, uint32 iCount /*= 1*/) const
	{
		FT_GL_ASSERT( glUniform1fv(glGetUniformLocation(m_iHandle, csName), iCount, p) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const int32* p, uint32 iCount /*= 1*/) const
	{
		FT_GL_ASSERT( glUniform1iv(glGetUniformLocation(m_iHandle, csName), iCount, p) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector2& v) const
	{
		FT_GL_ASSERT( glUniform2fv(glGetUniformLocation(m_iHandle, csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector3& v) const
	{
		FT_GL_ASSERT( glUniform3fv(glGetUniformLocation(m_iHandle, csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Vector4& v) const
	{
		FT_GL_ASSERT( glUniform4fv(glGetUniformLocation(m_iHandle, csName), 1, (GLfloat*)&v) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Matrix33& m) const
	{
		FT_GL_ASSERT( glUniformMatrix3fv(glGetUniformLocation(m_iHandle, csName), 1, GL_FALSE, (const GLfloat*)&m) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const Matrix44& m) const
	{
		FT_GL_ASSERT( glUniformMatrix4fv(glGetUniformLocation(m_iHandle, csName), 1, GL_FALSE, (const GLfloat*)&m) );
	}

	void	ShaderProgram::SetUniform(const char* csName, const SPtr<Texture>& xTexture, uint32 iTextureUnit) const
	{
		if (xTexture != nullptr) // assert suffisant?
			xTexture->Bind(iTextureUnit);

		FT_GL_ASSERT( glUniform1i(glGetUniformLocation(m_iHandle, csName), iTextureUnit) );
	}
}
