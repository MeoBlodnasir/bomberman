
#include "Texture.hpp"

#include "OpenGL.hpp"
#include "Output.hpp"
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
		FT_ASSERT(m_xResource != nullptr);
		FT_ASSERT(m_xResource->IsLoadedAndValid());

		return m_xResource->GetSize();
	}

	void	Texture::Bind(uint32 iTextureUnit) const
	{
		FT_ASSERT(m_xResource != nullptr);
		FT_ASSERT(m_xResource->IsTextureLoadedAndValid());
		FT_GL_ASSERT( glActiveTexture(GL_TEXTURE0 + iTextureUnit) );
		FT_GL_ASSERT( glBindTexture(m_xResource->GetTextureTarget(), m_xResource->GetHandle()) );
	}

	TextureResource::TextureResource()
		: m_pImage(nullptr)
		, m_vImageSize(0.f, 0.f)
	{
	}

	TextureResource::~TextureResource()
	{
		FT_ASSERT(!IsLoadedAndValid());
	}

	bool	TextureResource::IsLoadedAndValid() const
	{
		return IsImageLoadedAndValid() || IsTextureLoadedAndValid();
	}

	bool	TextureResource::IsImageLoadedAndValid() const
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_pImage != nullptr && m_vImageSize.x > 0.f && m_vImageSize.y > 0.f;
	}

	bool	TextureResource::IsTextureLoadedAndValid() const
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_iHandle != 0 && m_vImageSize.x > 0.f && m_vImageSize.y > 0.f;
	}

	ErrorCode	TextureResource::Load(ResourceManager& /*oResourceManager*/, const TextureResourceInfos& oInfos)
	{
		// Chargement de l'image en mémoire
		sf::Image* pImage = new sf::Image;

		if (!pImage->loadFromFile(oInfos.oFilePath.GetFullPath()))
		{
			FT_CERR << "Erreur dans le chargement de la texture: " << oInfos.oFilePath << std::endl;
			return FT_FAIL;
		}

		m_pImage = pImage;
		m_vImageSize = FromSFML(pImage->getSize());

		// Chargement de la texture dans la mémoire vidéo uniquement si la cible est définie
		if (oInfos.iTextureTarget != (uint32)-1)
		{
			GLuint	iHandle;
			GLenum	eTextureTarget = oInfos.iTextureTarget;

			FT_GL_ASSERT( glGenTextures(1, &iHandle) );
			FT_GL_ASSERT( glBindTexture(eTextureTarget, iHandle) );

			FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT) );
			FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT) );
			FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
			FT_GL_ASSERT( glTexParameteri(eTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR) );

			FT_GL_ASSERT( glTexImage2D(
				eTextureTarget,								// Type de texture
				0,											// Niveau mip-map
				GL_RGBA,									// Composants de la texture interne
				pImage->getSize().x, pImage->getSize().y,	// Dimensions
				0,											// Bords (0 = pas de bord, 1 = bords)
				GL_RGBA,									// Composants de la donnée externe
				GL_UNSIGNED_BYTE,							// Type et taille de donnée
				pImage->getPixelsPtr()) );					// Pointeur vers les données

			FT_GL_ASSERT( glGenerateMipmap(eTextureTarget) );
			FT_GL_ASSERT( glBindTexture(eTextureTarget, 0) );

			FT_TODO("Tester les retours OpenGL pour sortir de la fonction avec FT_FAIL au besoin");

			m_iHandle = iHandle;
			m_pImage = nullptr;

			// Libérer l'image
			FT_DELETE(pImage);
		}

		m_oResourceInfos = oInfos;

		return FT_OK;
	}

	ErrorCode	TextureResource::Unload()
	{
		// Libérer la texture
		if (m_iHandle != 0)
		{
			FT_GL_ASSERT( glDeleteTextures(1, &m_iHandle) );
			m_iHandle = 0;
		}

		// Libérer l'image
		FT_SAFE_DELETE(m_pImage);

		return FT_OK;
	}
}
