#pragma once

#include "Core/ErrorCode.hpp"
#include "Core/StrongPointer.hpp"
#include "Core/Hash.hpp"

namespace ft
{
	// fw
	class ResourceManager;

	template <typename TResourceInfos>
	class Resource : public CountableSPtr
	{
	public:

		typedef typename TResourceInfos		InfosType;

		Resource()			{}
		virtual ~Resource()	{}

	private:

		// Fonctions uniquement utilisables par un SpecificResourceManager
		virtual ErrorCode	Load(ResourceManager& oResourceManager, const InfosType& oInfos) = 0;
		virtual ErrorCode	Unload() = 0;

	public:

		// S'assure que la ressource est exploitable
		virtual bool	IsLoadedAndValid() const = 0;

	private:

		Resource(const Resource&) FT_DELETED;
		Resource& operator = (const Resource&) FT_DELETED;
	};
}
