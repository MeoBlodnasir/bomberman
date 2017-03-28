#pragma once

#include "Vector2.hpp"

#include <SFML/System/Vector2.hpp>

namespace ft
{
	template <typename T>
	inline sf::Vector2<T>	ToSFML(const ft::Vector2& v)
	{
		return sf::Vector2f(static_cast<T>(v.x), static_cast<T>(v.y));
	}

	template <typename T>
	inline ft::Vector2		FromSFML(const sf::Vector2<T>& v)
	{
		return ft::Vector2(v.x, v.y);
	}
}
