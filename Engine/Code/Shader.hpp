#pragma once

#include "Handled.hpp"
#include "Path.hpp"
#include "StrongPointer.hpp"
#include "ShaderType.hpp"

namespace ft
{
	// fw
	class ShaderFileResource;

	class Shader : public CountableSPtr
	{
	public:

				Shader();
		virtual ~Shader();

		ErrorCode		Create(const SPtr<ShaderFileResource>& xResource);
		ErrorCode		Destroy();

		EShaderType		GetShaderType() const;

	protected:

		SPtr<ShaderFileResource>	m_xResource;

	private:

		Shader(const Shader&) FT_DELETED;
		Shader& operator = (const Shader&) FT_DELETED;
	};
}
