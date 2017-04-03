
#include "RenderTechnique.hpp"

#include "ShaderProgram.hpp"

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
