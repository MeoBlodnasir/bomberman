#pragma once

#include "ResourceManager.hpp"
#include "TextureResource.hpp"

namespace ft
{
	class TextureResourceManager : public ResourceManager<TextureResource>
	{
	public:

		virtual ErrorCode	Load(const TextureResourceInfos& oInfos, SPtr<TextureResource>& xOutResource) override;
		virtual ErrorCode	Unload(const TextureResourceInfos& oInfos) override;

	protected:

		virtual	ErrorCode	UnloadByHash(Hash::Type iHash) override;
	};
}
