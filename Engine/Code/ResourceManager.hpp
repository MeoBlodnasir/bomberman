#pragma once

#include "ErrorCode.hpp"
#include "StrongPointer.hpp"
#include "Hash.hpp"

#include <map>
#include <type_traits>

namespace ft
{
	// L'Id de la Resource est une classe qui permet de r�cup�rer une r�f�rence
	// sur cette-ci via un ResourceManager correspondant.
	// En g�n�ral std::string, ft:Path, ou int.
	template <typename TId>
	struct Resource : public CountableSPtr
	{
		typedef	TId		Id;
	};

	template <typename TResourceType>
	class ResourceManager
	{
	public:

		// V�rification � la compilation que le type pass� en argument de template est une Resource
		FT_STATIC_ASSERT((std::is_base_of< Resource<typename TResourceType::Id>, typename TResourceType >::value));
		
		typedef	typename TResourceType::Id	Id;
		typedef	typename TResourceType		Type;

		// Charge et alloue la ressource si elle ne l'est pas encore puis garde une correspondance dans la map,
		// sinon r�cup�re la ressource dans la map,
		// renvoie la ressource par le StrongPointer donn� en param�tre.
		// FT_OK si tout s'est bien pass�,
		// FT_FAIL si la ressource n'a pas pu �tre charg�e
		virtual ErrorCode	Load(const Id&, SPtr<Type>&) = 0;

		// Supprime la r�f�rence sur la ressource correspondante.
		// FT_OK si la ressource est d�sallou�e,
		// FT_FAIL si une r�f�rence existe encore sur cette ressource, hors ResourceManager
		virtual ErrorCode	Unload(const Id&) = 0;

		// Supprime les r�f�rences sur toutes les ressources.
		// FT_OK si tout est d�sallou�.
		// FT_FAIL si au moins une r�f�rence existe encore sur une ressource, hors ResourceManager
		virtual ErrorCode	UnloadAll() = 0;

	protected:

		std::map<Hash, SPtr<Type>>		m_oResources;
	};
}
