#pragma once

#include "Core.hpp"
#include "ErrorCode.hpp"
#include "Resource.hpp"

#include <map>

#if defined (__FT_DEBUG__)
#	include <type_traits>	// std::is_base_of
#endif

#define ENGINE_DEFAULT_RESOURCE_NAME	"__EngineDefault"

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;
	class ShaderResource;
	class TextureResource;
	class ShaderProgramResource;
	class MaterialResource;
	class MeshResource;
	class ModelResource;


	// Classe générale qui gère tous les SpecificResourceManager

	typedef SpecificResourceManager<ShaderResource>			ShaderResourceManager;
	typedef SpecificResourceManager<TextureResource>		TextureResourceManager;
	typedef SpecificResourceManager<ShaderProgramResource>	ShaderProgramResourceManager;
	typedef SpecificResourceManager<MaterialResource>		MaterialResourceManager;
	typedef SpecificResourceManager<MeshResource>			MeshResourceManager;
	typedef SpecificResourceManager<ModelResource>			ModelResourceManager;

	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		ErrorCode	Create();
		ErrorCode	Destroy();

		ShaderResourceManager*			GetShaderResourceManager() const		{ FT_ASSERT(m_pShaderResourceManager != nullptr); return m_pShaderResourceManager; }
		TextureResourceManager*			GetTextureResourceManager() const		{ FT_ASSERT(m_pTextureResourceManager != nullptr); return m_pTextureResourceManager; }
		ShaderProgramResourceManager*	GetShaderProgramResourceManager() const	{ FT_ASSERT(m_pShaderProgramResourceManager != nullptr); return m_pShaderProgramResourceManager; }
		MaterialResourceManager*		GetMaterialResourceManager() const		{ FT_ASSERT(m_pMaterialResourceManager != nullptr); return m_pMaterialResourceManager; }
		MeshResourceManager*			GetMeshResourceManager() const			{ FT_ASSERT(m_pMeshResourceManager != nullptr); return m_pMeshResourceManager; }
		ModelResourceManager*			GetModelResourceManager() const			{ FT_ASSERT(m_pModelResourceManager != nullptr); return m_pModelResourceManager; }

		ErrorCode	LoadDefaultResources();

	private:

		ShaderResourceManager*			m_pShaderResourceManager;
		TextureResourceManager*			m_pTextureResourceManager;
		ShaderProgramResourceManager*	m_pShaderProgramResourceManager;
		MaterialResourceManager*		m_pMaterialResourceManager;
		MeshResourceManager*			m_pMeshResourceManager;
		ModelResourceManager*			m_pModelResourceManager;

		ResourceManager(const ResourceManager&) FT_DELETED;
		ResourceManager& operator = (const ResourceManager&) FT_DELETED;
	};


	// Manager d'un type de ressource spécifique

	template <typename TResourceType>
	class SpecificResourceManager
	{
	public:

		typedef	typename TResourceType				ResourceType;
		typedef	typename TResourceType::InfosType	ResourceInfosType;

	private:

		// Vérification à la compilation que le type passé en argument de template est une Resource
		FT_STATIC_ASSERT((std::is_base_of< Resource<ResourceInfosType>, ResourceType >::value));

	public:

		SpecificResourceManager<TResourceType>(ResourceManager& oOwner)
			: m_oOwner(oOwner)
			, m_xDefault(nullptr)
		{
		}

		virtual ~SpecificResourceManager<TResourceType>()
		{
			FT_TEST(UnloadAll() == FT_OK);
		}

		// Charge et alloue la ressource si elle ne l'est pas encore puis garde une correspondance dans la map,
		// sinon récupère la ressource dans la map,
		// renvoie la ressource par le StrongPointer donné en paramètre.
		// FT_OK si tout s'est bien passé,
		// FT_FAIL si la ressource n'a pas pu être chargée (pas de modification de xOutResource).
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

		// Récupère la ressource dans la map,
		// renvoie la ressource par le StrongPointer donné en paramètre.
		// FT_OK si tout s'est bien passé,
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

		// Récupère la ressource par défaut,
		// FT_OK si tout s'est bien passé,
		// FT_FAIL si la ressource n'existe pas (pas de modification de xOutResource).
		ErrorCode	GetDefault(SPtr<ResourceType>& xOutResource)
		{
			ErrorCode eRet = FT_FAIL;

			if (m_xDefault != nullptr)
			{
				xOutResource == m_xDefault;
				eRet = FT_OK;
			}

			return eRet;
		}

		// Voir UnloadByHash
		ErrorCode	Unload(const ResourceInfosType& oInfos)
		{
			return UnloadByHash(Hash::Compute(oInfos));
		}

		// Supprime les références sur toutes les ressources.
		// FT_OK si tout est désalloué.
		// FT_FAIL si au moins une référence existe encore sur une ressource, hors ResourceManager.
		ErrorCode	UnloadAll()
		{
			ErrorCode eRet = FT_OK;

			m_xDefault = nullptr;

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
					eRet = pResource->Unload();
					// Test qu'il s'agit bien de la dernière référence à cette ressource
					if (pResource->GetReferenceCount() > 1)
						eRet = FT_FAIL;
				}
				// Suppression de la référence à la ressource
				FT_TEST(m_oResources.erase(iHash) == 1);
			}

			return eRet;
		}

		// Charge la ressource correspondante et l'assigne à la ressource par défaut
		// FT_OK si tout s'est bien passé,
		// FT_FAIL si la ressource n'a pas pu être chargée (pas de modification de m_xDefault).
		friend class ResourceManager;
		ErrorCode	SetAsDefault(const ResourceInfosType& oInfos)
		{
			return Load(oInfos, m_xDefault);
		}

	protected:

		ResourceManager&	m_oOwner;
		SPtr<ResourceType>	m_xDefault;

	private:

		std::map< Hash::Type, SPtr<ResourceType> >	m_oResources;

		SpecificResourceManager<TResourceType>() FT_DELETED;
		SpecificResourceManager<TResourceType>(const SpecificResourceManager<TResourceType>&) FT_DELETED;
		SpecificResourceManager<TResourceType>& operator = (const SpecificResourceManager<TResourceType>&) FT_DELETED;
	};
}
