#pragma once

#include "Core/Core.hpp"

namespace ft
{
	class Game
	{
	public:

		Game();
		~Game();

	private:

		Game(const Game&) FT_DELETED;
		Game&	operator = (const Game&) FT_DELETED;
	};
}
