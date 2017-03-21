
#include "MathStructOutput.hpp"

#include <iostream>
#include <iomanip>

namespace ft
{
	std::ostream&	operator << (std::ostream& oOs, const Vector2& v)
	{
		oOs << '[' << (v.x) << ", " << (v.y) << ']';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Vector3& v)
	{
		oOs << '[' << (v.x) << ", " << (v.y) << ", " << (v.z) << ']';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Vector4& v)
	{
		oOs << '[' << (v.x) << ", " << (v.y) << ", " << (v.z) << ", " << (v.w) << ']';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Matrix33& m)
	{
		oOs << '[' << (m[0][0]) << ", " << (m[0][1]) << ", " << (m[0][2]) << "]\n";
		oOs << '[' << (m[1][0]) << ", " << (m[1][1]) << ", " << (m[1][2]) << "]\n";
		oOs << '[' << (m[2][0]) << ", " << (m[2][1]) << ", " << (m[2][2]) << ']';
		return oOs;
	}

	std::ostream&	operator << (std::ostream& oOs, const Matrix44& m)
	{
		oOs << '[' << (m[0][0]) << ", " << (m[0][1]) << ", " << (m[0][2]) << ", " << (m[0][3]) << "]\n";
		oOs << '[' << (m[1][0]) << ", " << (m[1][1]) << ", " << (m[1][2]) << ", " << (m[1][3]) << "]\n";
		oOs << '[' << (m[2][0]) << ", " << (m[2][1]) << ", " << (m[2][2]) << ", " << (m[2][3]) << "]\n";
		oOs << '[' << (m[3][0]) << ", " << (m[3][1]) << ", " << (m[3][2]) << ", " << (m[3][3]) << ']';
		return oOs;
	}
}
