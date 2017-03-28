#pragma once

#include "StrongPointer.hpp"

namespace ft
{
	// L'Id de la Resource est une classe qui permet de récupérer une référence
	// sur cette-ci via un ResourceManager correspondant.
	// En général std::string, ft:Path, ou int.
	template <typename TId>
	class Resource : public CountableSPtr
	{
	public:

		typedef	TId		Id;

		virtual ~Resource()	{}

		// S'assure que la ressource est exploitable
		virtual bool	IsLoadedAndValid() const = 0;

	protected:

		Resource()	{}

	private:

		Resource(const Resource&) FT_DELETED;
		Resource& operator = (const Resource&) FT_DELETED;
	};
}
