#pragma once

#include "ResourceManager.hpp"
#include "TextureResource.hpp"

namespace ft
{
	class TextureResourceManager : public ResourceManager<TextureResource>
	{
	protected:

		virtual ErrorCode	Load_Impl(const TextureResourceInfos& oInfos, SPtr<TextureResource>& xOutResource) override;
		virtual ErrorCode	Unload_Impl(TextureResource* pResource) override;
	};
}
