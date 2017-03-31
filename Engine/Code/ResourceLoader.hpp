#pragma once

#include "StrongPointer.hpp"
#include "ErrorCode.hpp"

// Fonctions de chargement de ressource.
// Elles doivent être implémentées pour tout type de ressource.

namespace ft
{
	namespace ResourceLoader
	{
		// TSource: Source des données de la ressource: pointeur sur données déjà chargées en mémoire, chemin vers un fichier, etc...
		template <typename TSource, typename TResource>
		ErrorCode	Load(const TSource& oSource, SPtr<TResource> xOutResource)
		{
			FT_NOT_IMPLEMENTED("ResourceLoader non implémenté");
			return FT_FAIL;
		}

		template <typename TResource>
		ErrorCode	Unload(TResource* pResource)
		{
			FT_NOT_IMPLEMENTED("ResourceLoader non implémenté");
			return FT_FAIL;
		}
	};
}
