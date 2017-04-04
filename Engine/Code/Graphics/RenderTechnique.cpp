
#include "Graphics/RenderTechnique.hpp"

#include "Graphics/ShaderProgram.hpp"

namespace ft
{
	RenderTechnique::RenderTechnique()
		: xShaderProgram(nullptr)
	{
	}

	RenderTechnique::~RenderTechnique()
	{
		xShaderProgram = nullptr;
	}
}
