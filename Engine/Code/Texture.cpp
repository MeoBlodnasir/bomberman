
#include "Texture.hpp"

#include "Output.hpp"

#include <SFML/Graphics/Image.hpp>

namespace ft
{
	Texture::Texture()
		: m_vSize(0.f, 0.f)
		, m_eTextureTarget(0)
	{
	}

	Texture::~Texture()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Texture::Create(GLenum eTextureTarget, const Path& oSourceFilePath)
	{
		sf::Image	oSfImage;
		GLuint		iHandle;

		if (m_iHandle != 0)
		{
			FT_TEST_RETURN(Destroy() == FT_OK, FT_FAIL);
		}

		if (!oSfImage.loadFromFile(oSourceFilePath.GetFullPath()))
		{
			FT_CERR << "Erreur dans le chargement de la texture: " << oSourceFilePath << std::endl;
			FT_TODO("Charger une texture par défaut");
			return FT_FAIL;
		}

		FT_GL_ASSERT( glGenTextures(1, &iHandle) );
		FT_GL_ASSERT( glBindTexture(eTextureTarget, iHandle) );

		FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT) );
		FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT) );
		FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
		FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR) );

		FT_GL_ASSERT( glTexImage2D(
			eTextureTarget,				// Type de texture
			0,							// Niveau mip-map
			GL_RGBA,					// Composants de la texture interne
			oSfImage.getSize().x,		// Dimensions
			oSfImage.getSize().y,		// - 
			0,							// Bords (0 = pas de bord, 1 = bords)
			GL_RGBA,					// Composants de la donnée externe
			GL_UNSIGNED_BYTE,			// Type et taille de donnée
			oSfImage.getPixelsPtr()) );	// Pointeur vers les données

		FT_GL_ASSERT( glGenerateMipmap(eTextureTarget) );
		FT_GL_ASSERT( glBindTexture(eTextureTarget, 0) );

		m_iHandle = iHandle;
		m_eTextureTarget = eTextureTarget;
		m_vSize = Vector2((float32)oSfImage.getSize().x, (float32)oSfImage.getSize().y);

		return FT_OK;
	}

	ErrorCode	Texture::Destroy()
	{
		FT_GL_ASSERT( glDeleteTextures(1, &m_iHandle) );
		m_vSize = Vector2(0.f, 0.f);
		m_eTextureTarget = 0;
		return Handled::Destroy();
	}

	void		Texture::Bind(uint32 iTextureUnit) const
	{
		FT_GL_ASSERT( glActiveTexture(GL_TEXTURE0 + iTextureUnit) );
		FT_GL_ASSERT( glBindTexture(m_eTextureTarget, m_iHandle) );
	}
}
