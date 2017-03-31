#pragma once

#include "Handled.hpp"
#include "Resource.hpp"
#include "Path.hpp"
#include "Vector2.hpp"

namespace ft
{
	// fw
	class TextureResource;

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


// fw
namespace sf
{
	class Image;
}

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;

	struct TextureResourceInfos
	{
		Path	oFilePath;			// Chemin du fichier
		uint32	iTextureTarget;		// Utilisation par le driver vidéo
									//		si le champ est défini et valide, la texture est allouée, puis l'image libérée
									//		sinon, l'image uniquement est allouée
		TextureResourceInfos()
			: iTextureTarget((uint32)-1)
		{}
	};

	class TextureResource : public Handled, public Resource<TextureResourceInfos>
	{
	public:

		virtual ~TextureResource();

		virtual bool				IsLoadedAndValid() const override;
				bool				IsImageLoadedAndValid() const;
				bool				IsTextureLoadedAndValid() const;

		inline	const Path&			GetFilePath() const			{ return m_oResourceInfos.oFilePath; }
		inline	uint32				GetTextureTarget() const	{ return m_oResourceInfos.iTextureTarget; }
		inline	const sf::Image*	GetImage() const			{ return m_pImage; }
		inline	const Vector2&		GetSize() const				{ return m_vImageSize; }

	protected:

		TextureResource();

		TextureResourceInfos	m_oResourceInfos;
		sf::Image*				m_pImage;
		Vector2					m_vImageSize;

	private:
		friend SpecificResourceManager<TextureResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const TextureResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <> struct hash<ft::TextureResourceInfos>
	{
		size_t	operator ()	(const ft::TextureResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};
}
