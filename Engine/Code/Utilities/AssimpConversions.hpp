#pragma once

#include "Math/Matrix44.hpp"
#include "Graphics/Color.hpp"

#include <assimp\matrix4x4.h>
#include <assimp\types.h>

#include <string.h> // memcpy

namespace ft
{
	// Vérification, car la copie de l'un vers l'autre se fait par memcpy,
	// assumant que les deux utilisent la même quantité d'espace mémoire.
	// Mais il faut la transposer avant:
	// aiMatrix4x4	=> row-major
	// ft::Matrix44	=> column-major
	FT_STATIC_ASSERT(sizeof(ft::Matrix44) == sizeof(aiMatrix4x4));

	inline void		FromAssimp(Matrix44& mMat, const aiMatrix4x4& mAiMatrix)
	{
		::memcpy((void*)&mMat, (void*)&mAiMatrix, sizeof(Matrix44));
		mMat = glm::transpose(mMat);
	}

	inline Color3	FromAssimp(const aiColor3D& oAiColor3D)
	{
		Color3 oColor3;

		oColor3.r = oAiColor3D.r;
		oColor3.g = oAiColor3D.g;
		oColor3.b = oAiColor3D.b;
		return oColor3;
	}
}
