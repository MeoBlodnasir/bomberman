#pragma once

#include "Handled.hpp"
#include "Path.hpp"
#include "StrongPointer.hpp"

namespace ft
{
	enum EShaderType
	{
		E_INVALID_SHADER_TYPE = -1,

		E_VERTEX_SHADER = 0,
		E_FRAGMENT_SHADER,

		E_SHADER_TYPE_COUNT
	};

	class Shader : public Handled, public CountableSPtr
	{
	public:

				Shader();
		virtual ~Shader();

				ErrorCode		Create(EShaderType eShaderType, const Path& oSourceFilePath);
		virtual ErrorCode		Destroy() override;

		inline	EShaderType		GetShaderType() const						{ return m_eShaderType; }

		static	uint32			GetGLShaderType(EShaderType eShaderType);

	protected:

		Path			m_oSourceFilePath;
		EShaderType		m_eShaderType;

	private:

		Shader(const Shader&) FT_DELETED;
		Shader& operator = (const Shader&) FT_DELETED;
	};
}
