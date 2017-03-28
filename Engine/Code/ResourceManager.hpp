#pragma once

#include "ErrorCode.hpp"
#include "Resource.hpp"
#include "Hash.hpp"

#include <map>
#include <type_traits>

namespace ft
{
	template <typename TResourceType>
	class ResourceManager
	{
	public:

		// Vérification à la compilation que le type passé en argument de template est une Resource
		FT_STATIC_ASSERT((std::is_base_of< Resource<typename TResourceType::Id>, typename TResourceType >::value));
		
		typedef	typename TResourceType::Id	Id;
		typedef	typename TResourceType		Type;

		ResourceManager<TResourceType>()					{}

		// Le destructeur vérifie qu'on a bien appelé UnloadAll() avant de détruire le ResourceManager
		virtual ~ResourceManager<TResourceType>()			{ FT_ASSERT(m_oResources.empty()); }

		// Charge et alloue la ressource si elle ne l'est pas encore puis garde une correspondance dans la map,
		// sinon récupère la ressource dans la map,
		// renvoie la ressource par le StrongPointer donné en paramètre.
		// FT_OK si tout s'est bien passé,
		// FT_FAIL si la ressource n'a pas pu être chargée.
		virtual ErrorCode	Load(const Id&, SPtr<Type>&) = 0;

		// Supprime la référence sur la ressource correspondante.
		// FT_OK si la ressource est désallouée,
		// FT_FAIL si une référence existe encore sur cette ressource, hors ResourceManager.
		virtual ErrorCode	Unload(const Id&) = 0;

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

	protected:

		std::map< Hash::Type, SPtr<Type> >	m_oResources;

		// Wrapper
		virtual	ErrorCode	UnloadByHash(Hash::Type iHash) = 0;
	};
}
