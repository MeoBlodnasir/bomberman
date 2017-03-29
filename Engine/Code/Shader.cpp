
#include "Shader.hpp"

#include "ShaderResource.hpp"
#include "OpenGL.hpp"

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

	ErrorCode	Shader::Create(const SPtr<ShaderFileResource>& xResource)
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

	EShaderType		Shader::GetShaderType() const
	{
		FT_ASSERT(m_xResource != nullptr);
		return m_xResource->GetShaderType();
	}
}
