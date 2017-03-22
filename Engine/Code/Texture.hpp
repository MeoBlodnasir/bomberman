#pragma once

#include "Handled.hpp"
#include "StrongPointer.hpp"
#include "Path.hpp"
#include "Vector2.hpp"
#include "OpenGL.hpp"

namespace ft
{
	class Texture : public Handled, public CountableSPtr
	{
	public:

				Texture();
		virtual ~Texture();

				ErrorCode		Create(GLenum eTextureTarget, const Path& oSourceFilePath);
		virtual ErrorCode		Destroy();

		inline	const Vector2&	GetSize() const					{ return m_vSize; }
		inline	uint32			GetWidth() const				{ return (uint32)m_vSize.x; }
		inline	uint32			GetHeight() const				{ return (uint32)m_vSize.y; }

				void			Bind(uint32 iTextureUnit) const;

	protected:

		Path		m_oSourceFilePath;
		Vector2		m_vSize;
		GLenum		m_eTextureTarget;

	private:

		Texture(const Texture&) FT_DELETED;
		Texture& operator = (const Texture&) FT_DELETED;
	};
}
