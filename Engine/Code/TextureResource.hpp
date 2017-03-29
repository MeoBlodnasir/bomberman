#pragma once

#include "Resource.hpp"
#include "Path.hpp"
#include "ErrorCode.hpp"

// fw
namespace sf
{
	class Image;
}

namespace ft
{
	struct TextureResourceInfos
	{
		Path	oFilePath;
		uint32	iTextureTarget;
	};
}

namespace ft
{
	class TextureResource : public Resource<TextureResourceInfos>
	{
	public:

		virtual ~TextureResource();

		virtual bool				IsLoadedAndValid() const override;

		inline	const Path&			GetFilePath() const			{ return m_oFilePath; }
		inline	const sf::Image*	GetImage() const			{ return m_pImage; }
		inline	uint32				GetHandle() const			{ return m_iHandle; }
		inline	uint32				GetTextureTarget() const	{ return m_iTarget; }

	protected:

		friend class TextureResourceManager;
		
		TextureResource();

		Path		m_oFilePath;
		sf::Image*	m_pImage;
		uint32		m_iHandle;
		uint32		m_iTarget;
	};
}

// Nécessaire pour TextureResourceManager
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
