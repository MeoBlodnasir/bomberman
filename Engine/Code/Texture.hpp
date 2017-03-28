#pragma once

#include "TextureResource.hpp"
#include "Vector2.hpp"

namespace ft
{
	class Texture : public CountableSPtr
	{
	public:

				Texture();
		virtual ~Texture();

		ErrorCode		Create(const SPtr<TextureResource>& xResource);
		ErrorCode		Destroy();

		const Vector2&	GetSize() const;

		void			Bind(uint32 iTextureUnit) const;

	protected:

		SPtr<TextureResource>	m_xResource;

	private:

		Texture(const Texture&) FT_DELETED;
		Texture& operator = (const Texture&) FT_DELETED;
	};
}
