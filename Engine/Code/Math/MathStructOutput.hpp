#pragma once

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"
#include "Math/Vector4.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Matrix33.hpp"
#include "Math/Matrix44.hpp"

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
