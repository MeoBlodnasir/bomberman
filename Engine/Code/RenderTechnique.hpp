#pragma once

#include "StrongPointer.hpp"
#include "ErrorCode.hpp"
#include "Material.hpp"

namespace ft
{
	// fw
	class ShaderProgram;

	class RenderTechnique
	{
	public:

		SPtr<ShaderProgram>		xShaderProgram;
		Material				oMaterial;

		RenderTechnique();
		~RenderTechnique();
	};
}
