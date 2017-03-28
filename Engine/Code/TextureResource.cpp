
#include "TextureResource.hpp"

namespace ft
{
	TextureResource::TextureResource()
		: m_pImage(nullptr)
		, m_iHandle(0)
		, m_iTarget(0)
	{
	}

	TextureResource::~TextureResource()
	{
		m_pImage = nullptr;
	}

	bool	TextureResource::IsLoadedAndValid() const 
	{
		return !m_oFilePath.IsEmpty() && m_pImage != nullptr && m_iHandle != 0;
	}
}
