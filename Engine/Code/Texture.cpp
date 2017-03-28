
#include "Texture.hpp"

#include "OpenGL.hpp"
#include "SfmlConversions.hpp"

#include <SFML/Graphics/Image.hpp>

namespace ft
{
	Texture::Texture()
		: m_xResource(nullptr)
	{
	}

	Texture::~Texture()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Texture::Create(const SPtr<TextureResource>& xResource)
	{
		FT_ASSERT(xResource != nullptr);

		FT_TEST_RETURN(xResource->IsLoadedAndValid(), FT_FAIL);
		m_xResource = xResource;

		return FT_OK;
	}

	ErrorCode	Texture::Destroy()
	{
		m_xResource = nullptr;
		return FT_OK;
	}

	const Vector2&	Texture::GetSize() const
	{
		FT_ASSERT(m_xResource != nullptr && m_xResource->IsLoadedAndValid());
		return FromSFML(m_xResource->GetImage()->getSize());
	}

	void	Texture::Bind(uint32 iTextureUnit) const
	{
		FT_ASSERT(m_xResource != nullptr && m_xResource->IsLoadedAndValid());
		FT_GL_ASSERT( glActiveTexture(GL_TEXTURE0 + iTextureUnit) );
		FT_GL_ASSERT( glBindTexture(m_xResource->GetTextureTarget(), m_xResource->GetHandle()) );
	}
}
