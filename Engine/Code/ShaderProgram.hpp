#pragma once

#include "ErrorCode.hpp"
#include "StrongPointer.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"

namespace ft
{
	// fw
	class ShaderResource;
	class Texture;

	class ShaderProgram : public CountableSPtr
	{
	public:

		ShaderProgram();
		virtual ~ShaderProgram();

		ErrorCode	Create(const SPtr<ShaderResource>& xResource);
		ErrorCode	Destroy();

		void		Use() const;

		// Wrappers glUniform*()
		void		SetUniform(const char* csName, float32 f) const;
		void		SetUniform(const char* csName, int32 i) const;
		void		SetUniform(const char* csName, const float32* p, uint32 iCount = 1) const;
		void		SetUniform(const char* csName, const int32* p, uint32 iCount = 1) const;
		void		SetUniform(const char* csName, const Vector2& v) const;
		void		SetUniform(const char* csName, const Vector3& v) const;
		void		SetUniform(const char* csName, const Vector4& v) const;
		void		SetUniform(const char* csName, const Matrix33& m) const;
		void		SetUniform(const char* csName, const Matrix44& m) const;
		void		SetUniform(const char* csName, const SPtr<Texture>& xTexture, uint32 iTextureUnit) const;

	private:

		SPtr<ShaderResource>	m_xResource;

		ShaderProgram(const ShaderProgram&) FT_DELETED;
		ShaderProgram& operator = (const ShaderProgram&) FT_DELETED;
	};
}
