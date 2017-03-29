#pragma once

#include "SpecificResourceManager.hpp"
#include "TextureResource.hpp"

namespace ft
{
	class TextureResourceManager : public SpecificResourceManager<TextureResource>
	{
	public:

		TextureResourceManager(ResourceManager& oOwner) : SpecificResourceManager<TextureResource>(oOwner) {}

	protected:

		virtual ErrorCode	Load_Impl(const TextureResourceInfos& oInfos, SPtr<TextureResource>& xOutResource) override;
		virtual ErrorCode	Unload_Impl(TextureResource* pResource) override;
	};
}
