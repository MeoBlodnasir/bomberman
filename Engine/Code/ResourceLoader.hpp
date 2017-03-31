#pragma once

#include "StrongPointer.hpp"
#include "ErrorCode.hpp"

// Fonctions de chargement de ressource.
// Elles doivent �tre impl�ment�es pour tout type de ressource.

namespace ft
{
	namespace ResourceLoader
	{
		// TSource: Source des donn�es de la ressource: pointeur sur donn�es d�j� charg�es en m�moire, chemin vers un fichier, etc...
		template <typename TSource, typename TResource>
		ErrorCode	Load(const TSource& oSource, SPtr<TResource> xOutResource)
		{
			FT_NOT_IMPLEMENTED("ResourceLoader non impl�ment�");
			return FT_FAIL;
		}

		template <typename TResource>
		ErrorCode	Unload(TResource* pResource)
		{
			FT_NOT_IMPLEMENTED("ResourceLoader non impl�ment�");
			return FT_FAIL;
		}
	};
}
