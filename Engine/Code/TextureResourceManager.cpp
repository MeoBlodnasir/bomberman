
#include "TextureResourceManager.hpp"

#include "OpenGL.hpp"
#include "Output.hpp"

#include <SFML/Graphics/Image.hpp>

namespace ft
{
	ErrorCode	TextureResourceManager::Load_Impl(const TextureResourceInfos& oInfos, SPtr<TextureResource>& xOutResource)
	{
		TextureResource* pResource = nullptr;

		// Chargement de l'image en mémoire
		sf::Image* pImage = new sf::Image;

		if (!pImage->loadFromFile(oInfos.oFilePath.GetFullPath()))
		{
			FT_CERR << "Erreur dans le chargement de la texture: " << oInfos.oFilePath << std::endl;
			FT_TODO("Charger une texture par défaut");
			return FT_FAIL;
		}

		// Chargement de la texture dans la mémoire vidéo
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

		pResource = new TextureResource;
		pResource->m_oResourceInfos	= oInfos;
		pResource->m_pImage			= pImage;
		pResource->m_iHandle		= iHandle;

		xOutResource = pResource;

		return FT_OK;
	}

	ErrorCode	TextureResourceManager::Unload_Impl(TextureResource* pResource)
	{
		FT_ASSERT(pResource != nullptr)

		// Libérer la mémoire vidéo
		if (pResource->m_iHandle != 0)
		{
			FT_GL_ASSERT( glDeleteTextures(1, &pResource->m_iHandle) );
			pResource->m_iHandle = 0;
		}

		// Libérer l'image en mémoire
		FT_SAFE_DELETE(pResource->m_pImage);

		return FT_OK;
	}
}
