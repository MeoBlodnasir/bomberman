#pragma once

#include "ErrorCode.hpp"
#include "Resource.hpp"
#include "Hash.hpp"

#include <map>

#if defined (__FT_DEBUG__)
#	include <type_traits> // std::is_base_of
#endif

namespace ft
{
	template <typename TResourceType>
	class ResourceManager
	{
	private:

		// Vérification à la compilation que le type passé en argument de template est une Resource
		FT_STATIC_ASSERT((std::is_base_of< Resource<typename TResourceType::Id>, typename TResourceType >::value));
		
	public:

		typedef	typename TResourceType::Id	ResourceId;
		typedef	typename TResourceType		ResourceType;


		ResourceManager<TResourceType>()
			: m_oResources()
		{
		}

		// Le destructeur vérifie qu'on a bien appelé UnloadAll() avant de détruire le ResourceManager
		virtual ~ResourceManager<TResourceType>()
		{
			FT_ASSERT(m_oResources.empty());
		}

		// Charge et alloue la ressource si elle ne l'est pas encore puis garde une correspondance dans la map,
		// sinon récupère la ressource dans la map,
		// renvoie la ressource par le StrongPointer donné en paramètre.
		// FT_OK si tout s'est bien passé,
		// FT_FAIL si la ressource n'a pas pu être chargée (pas de modification de xOutResource).
		ErrorCode	Load(const ResourceId& oId, SPtr<ResourceType>& xOutResource)
		{
			ErrorCode	eRet	= FT_OK;
			Hash::Type	iHashed	= Hash::Compute(oId);

			if (m_oResources.count(iHashed) == 1)
				xOutResource = m_oResources[iHashed];
			else
			{
				if ((eRet = Load_Impl(oId, xOutResource)) == FT_OK)
					m_oResources[iHashed] = xOutResource;
			}

			return eRet;
		}

		// Voir UnloadByHash
		ErrorCode	Unload(const ResourceId& oId)
		{
			return UnloadByHash(Hash::Compute(oId));
		}

		// Supprime les références sur toutes les ressources.
		// FT_OK si tout est désalloué.
		// FT_FAIL si au moins une référence existe encore sur une ressource, hors ResourceManager.
		ErrorCode	UnloadAll()
		{
			ErrorCode eRet = FT_OK;

			while (!m_oResources.empty())
				if (UnloadByHash(m_oResources.begin()->first) == FT_FAIL)
					eRet = FT_FAIL;

			return eRet;
		}

	private:

		// Supprime la référence sur la ressource correspondante.
		// FT_OK si la ressource est désallouée,
		// FT_FAIL si une référence existe encore sur cette ressource, hors ResourceManager.
		ErrorCode	UnloadByHash(Hash::Type iHash)
		{
			ErrorCode eRet = FT_OK;

			if (m_oResources.count(iHash) == 1)
			{
				ResourceType* pResource = m_oResources.at(iHash).Ptr();
				if (pResource != nullptr)
				{
					eRet = Unload_Impl(pResource);
					// Test qu'il s'agit bien de la dernière référence à cette ressource
					if (pResource->GetReferenceCount() > 1)
						eRet = FT_FAIL;
				}
				// Suppression de la référence à la ressource
				FT_TEST(m_oResources.erase(iHash) == 1);
			}

			return eRet;
		}

	protected:

		// Interface à implémenter par les classes dérivées
		virtual ErrorCode	Load_Impl(const ResourceId&, SPtr<ResourceType>&)	= 0;
		virtual ErrorCode	Unload_Impl(ResourceType* pResource)				= 0;

	private:

		std::map< Hash::Type, SPtr<ResourceType> >		m_oResources;

		ResourceManager<TResourceType>(const ResourceManager<TResourceType>&) FT_DELETED;
		ResourceManager<TResourceType>& operator = (const ResourceManager<TResourceType>&) FT_DELETED;
	};
}
