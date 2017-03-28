
#include "TextureResourceManager.hpp"

#include "OpenGL.hpp"
#include "Output.hpp"

#include <SFML/Graphics/Image.hpp>

namespace ft
{
	ErrorCode	TextureResourceManager::Load(const TextureResourceInfos& oInfos, SPtr<TextureResource>& xOutResource)
	{
		Hash::Type iHashed = Hash::Compute(oInfos.oFilePath.GetFullPath());

		if (m_oResources.count(iHashed) == 1)
		{
			xOutResource = m_oResources[iHashed];
		}
		else
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
			pResource->m_oFilePath = oInfos.oFilePath;
			pResource->m_pImage = pImage;
			pResource->m_iHandle = iHandle;
			pResource->m_iTarget = eTextureTarget;

			m_oResources[iHashed] = pResource;
			xOutResource = pResource;
		}

		return FT_OK;
	}

	ErrorCode	TextureResourceManager::Unload(const TextureResourceInfos& oInfos)
	{
		Hash::Type	iHashed = Hash::Compute(oInfos.oFilePath.GetFullPath());

		return UnloadByHash(iHashed);
	}

	ErrorCode	TextureResourceManager::UnloadByHash(Hash::Type iHash)
	{
		ErrorCode eRet = FT_OK;

		if (m_oResources.count(iHash) == 1)
		{
			TextureResource* pResource = m_oResources[iHash].Ptr();
			if (pResource != nullptr)
			{
				// Libérer la mémoire vidéo
				if (pResource->m_iHandle != 0)
				{
					FT_GL_ASSERT( glDeleteTextures(1, &pResource->m_iHandle) );
					pResource->m_iHandle = 0;
				}

				// Libérer l'image en mémoire
				FT_SAFE_DELETE(pResource->m_pImage);
			}

			// Test s'il s'agit bien de la dernière référence à cette ressource
			eRet = pResource->GetReferenceCount() == 1 ? FT_OK : FT_FAIL;
			// Suppression de la référence à la ressource
			FT_TEST(m_oResources.erase(iHash) == 1);
		}

		return eRet;
	}

}
