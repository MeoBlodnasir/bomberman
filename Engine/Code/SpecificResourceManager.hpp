#pragma once

#include "ErrorCode.hpp"
#include "Hash.hpp"
#include "Resource.hpp"
#include "ResourceLoader.hpp"

#include <map>

#if defined (__FT_DEBUG__)
#	include <type_traits> // std::is_base_of
#endif

// Classe de gestion d'un type de ressource sp�cifique

namespace ft
{
	// fw
	class ResourceManager;

	template <typename TResourceType>
	class SpecificResourceManager
	{
	public:

		typedef	typename TResourceType				ResourceType;
		typedef	typename TResourceType::InfosType	ResourceInfosType;

	private:

		// V�rification � la compilation que le type pass� en argument de template est une Resource
		FT_STATIC_ASSERT((std::is_base_of< Resource<ResourceInfosType>, ResourceType >::value));

	public:

		SpecificResourceManager<TResourceType>(ResourceManager& oOwner)
			: m_oOwner(oOwner)
			, m_oResources()
		{
		}

		virtual ~SpecificResourceManager<TResourceType>()
		{
			FT_TEST(UnloadAll() == FT_OK);
		}

		// Charge et alloue la ressource si elle ne l'est pas encore puis garde une correspondance dans la map,
		// sinon r�cup�re la ressource dans la map,
		// renvoie la ressource par le StrongPointer donn� en param�tre.
		// FT_OK si tout s'est bien pass�,
		// FT_FAIL si la ressource n'a pas pu �tre charg�e (pas de modification de xOutResource).
		ErrorCode	Load(const ResourceInfosType& oInfos, SPtr<ResourceType>& xOutResource)
		{
			ErrorCode	eRet	= FT_OK;
			Hash::Type	iHashed	= Hash::Compute(oInfos);

			if (m_oResources.count(iHashed) == 1)
				xOutResource = m_oResources[iHashed];
			else
			{
				SPtr<ResourceType> xResource = new ResourceType;
				if ((eRet = xResource->Load(m_oOwner, oInfos)) == FT_OK)
				{
					m_oResources[iHashed] = xResource;
					xOutResource = xResource;
				}
			}

			return eRet;
		}

		// R�cup�re la ressource dans la map,
		// renvoie la ressource par le StrongPointer donn� en param�tre.
		// FT_OK si tout s'est bien pass�,
		// FT_FAIL si la ressource n'existe pas (pas de modification de xOutResource).
		ErrorCode	Get(const ResourceInfosType& oInfos, SPtr<ResourceType>& xOutResource)
		{
			ErrorCode	eRet	= FT_OK;
			Hash::Type	iHashed	= Hash::Compute(oInfos);

			if (m_oResources.count(iHashed) == 1)
				xOutResource = m_oResources[iHashed];
			else
				eRet = FT_FAIL;

			return eRet;
		}

		// Voir UnloadByHash
		ErrorCode	Unload(const ResourceInfosType& oInfos)
		{
			return UnloadByHash(Hash::Compute(oInfos));
		}

		// Supprime les r�f�rences sur toutes les ressources.
		// FT_OK si tout est d�sallou�.
		// FT_FAIL si au moins une r�f�rence existe encore sur une ressource, hors ResourceManager.
		ErrorCode	UnloadAll()
		{
			ErrorCode eRet = FT_OK;

			while (!m_oResources.empty())
				if (UnloadByHash(m_oResources.begin()->first) == FT_FAIL)
					eRet = FT_FAIL;

			return eRet;
		}

	private:

		// Supprime la r�f�rence sur la ressource correspondante.
		// FT_OK si la ressource est d�sallou�e,
		// FT_FAIL si une r�f�rence existe encore sur cette ressource, hors ResourceManager.
		ErrorCode	UnloadByHash(Hash::Type iHash)
		{
			ErrorCode eRet = FT_OK;

			if (m_oResources.count(iHash) == 1)
			{
				ResourceType* pResource = m_oResources.at(iHash).Ptr();
				if (pResource != nullptr)
				{
					eRet = pResource->Unload();
					// Test qu'il s'agit bien de la derni�re r�f�rence � cette ressource
					if (pResource->GetReferenceCount() > 1)
						eRet = FT_FAIL;
				}
				// Suppression de la r�f�rence � la ressource
				FT_TEST(m_oResources.erase(iHash) == 1);
			}

			return eRet;
		}

	protected:

		ResourceManager&	m_oOwner;

	private:

		std::map< Hash::Type, SPtr<ResourceType> >	m_oResources;

		SpecificResourceManager<TResourceType>() FT_DELETED;
		SpecificResourceManager<TResourceType>(const SpecificResourceManager<TResourceType>&) FT_DELETED;
		SpecificResourceManager<TResourceType>& operator = (const SpecificResourceManager<TResourceType>&) FT_DELETED;
	};
}
