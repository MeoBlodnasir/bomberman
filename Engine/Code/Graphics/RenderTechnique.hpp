#pragma once

#include "Core/ErrorCode.hpp"
#include "Core/StrongPointer.hpp"
#include "Graphics/Material.hpp"

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
