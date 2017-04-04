
#include "Graphics/Mesh.hpp"

#include "Graphics/OpenGL.hpp"
#include "Graphics/VertexLocation.h"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderTechnique.hpp"
#include "Graphics/ShaderProgram.hpp"

namespace ft
{
	Mesh::Mesh()
		: m_xResource(nullptr)
	{
	}

	Mesh::~Mesh()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Mesh::Create(const SPtr<MeshResource>& xResource)
	{
		FT_ASSERT(xResource != nullptr);

		FT_TEST_RETURN(xResource->IsLoadedAndValid(), FT_FAIL);
		m_xResource = xResource;

		if (xResource->m_xMaterialResource != nullptr)
		{
			FT_TEST(m_oRenderTechnique.oMaterial.Create(xResource->m_xMaterialResource) == FT_OK);
		}
		m_oRenderTechnique.xShaderProgram = new ShaderProgram;
		FT_TEST(m_oRenderTechnique.xShaderProgram->Create(xResource->m_xShaderProgramResource) == FT_OK);

		return FT_OK;
	}

	ErrorCode	Mesh::Destroy()
	{
		m_xResource = nullptr;
		return FT_OK;
	}

	void	Mesh::Render(const RenderContext& oRenderContext, const Matrix44& mModel) const
	{
		m_oRenderTechnique.xShaderProgram->Use();
		oRenderContext.SendToShader(m_oRenderTechnique.xShaderProgram);
		m_oRenderTechnique.xShaderProgram->SetUniform("mModel", mModel);
		m_oRenderTechnique.oMaterial.SendToShader(m_oRenderTechnique.xShaderProgram);
		Draw();
	}

	void	Mesh::Draw() const
	{
		FT_ASSERT(m_xResource != nullptr);
		
		FT_GL_ASSERT( glBindVertexArray(m_xResource->GetHandle()) );
		if (m_xResource->m_iEbo != 0)
		{
			FT_GL_ASSERT( glDrawElements(m_xResource->m_iPrimitiveType, m_xResource->m_iVertexToDrawCount, GL_UNSIGNED_INT, 0) );
		}
		else
		{
			FT_GL_ASSERT( glDrawArrays(m_xResource->m_iPrimitiveType, 0, m_xResource->m_iVertexToDrawCount) );
		}

		FT_GL_ASSERT( glBindVertexArray(0) );
	}
}
