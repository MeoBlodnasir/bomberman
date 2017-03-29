#pragma once

#include "ErrorCode.hpp"
#include "Resource.hpp"
#include "Handled.hpp"
#include "Path.hpp"

// fw
namespace sf
{
	class Image;
}

// Séparer TextureResource de ImageResource?
// Notammeent pour pouvoir libérer la mémoire quand on n'a besoin que de la mémoire vidéo.

namespace ft
{
	struct TextureResourceInfos
	{
		Path	oFilePath;
		uint32	iTextureTarget;

		TextureResourceInfos()
			: oFilePath()
			, iTextureTarget(0)
		{}
	};

	class TextureResource : public Handled, public Resource<TextureResourceInfos>
	{
	public:

		virtual ~TextureResource();

		virtual bool				IsLoadedAndValid() const override;

		inline	const Path&			GetFilePath() const			{ return m_oResourceInfos.oFilePath; }
		inline	uint32				GetTextureTarget() const	{ return m_oResourceInfos.iTextureTarget; }
		inline	const sf::Image*	GetImage() const			{ return m_pImage; }

	protected:

		friend class TextureResourceManager;
		
		TextureResource();

		TextureResourceInfos	m_oResourceInfos;
		sf::Image*				m_pImage;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <>
	struct hash<ft::TextureResourceInfos>
	{
		size_t	operator ()	(const ft::TextureResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};
}
