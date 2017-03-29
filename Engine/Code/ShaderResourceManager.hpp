#pragma once

#include "SpecificResourceManager.hpp"
#include "ShaderResource.hpp"

namespace ft
{
	// G�re les fichiers de shader compil�s individuellement
	class ShaderFileResourceManager : public SpecificResourceManager<ShaderFileResource>
	{
	public:

		ShaderFileResourceManager(ResourceManager& oOwner)
			: SpecificResourceManager<ShaderFileResource>(oOwner)
		{}

	protected:

		virtual ErrorCode	Load_Impl(const ShaderFileResourceInfos& oInfos, SPtr<ShaderFileResource>& xOutResource) override;
		virtual ErrorCode	Unload_Impl(ShaderFileResource* pResource) override;
	};

	// G�re les programmes li�s
	class ShaderResourceManager : public SpecificResourceManager<ShaderResource>
	{
	public:

		ShaderResourceManager(ResourceManager& oOwner)
			: SpecificResourceManager<ShaderResource>(oOwner)
		{}

	protected:

		virtual ErrorCode	Load_Impl(const ShaderResourceInfos& oInfos, SPtr<ShaderResource>& xOutResource) override;
		virtual ErrorCode	Unload_Impl(ShaderResource* pResource) override;
	};
}
