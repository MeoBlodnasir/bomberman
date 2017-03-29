
#include "TextureResource.hpp"

namespace ft
{
	TextureResource::TextureResource()
		: m_pImage(nullptr)
		, m_oResourceInfos()
	{
	}

	TextureResource::~TextureResource()
	{
		m_pImage = nullptr;
	}

	bool	TextureResource::IsLoadedAndValid() const 
	{
		return !m_oResourceInfos.oFilePath.IsEmpty() && m_pImage != nullptr && m_iHandle != 0;
	}
}
