#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"

#include <iosfwd>

namespace ft
{
	std::ostream&	operator << (std::ostream& oOs, const Vector2& v);
	std::ostream&	operator << (std::ostream& oOs, const Vector3& v);
	std::ostream&	operator << (std::ostream& oOs, const Vector4& v);
	std::ostream&	operator << (std::ostream& oOs, const Quaternion& q);
	std::ostream&	operator << (std::ostream& oOs, const Matrix33& m);
	std::ostream&	operator << (std::ostream& oOs, const Matrix44& m);
}
