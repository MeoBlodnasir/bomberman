
#include "Mesh.hpp"

#include "OpenGL.hpp"
#include "VertexLocation.h"

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

		return FT_OK;
	}

	ErrorCode	Mesh::Destroy()
	{
		m_xResource = nullptr;
		return FT_OK;
	}

	void	Mesh::Draw() const
	{
		if (m_xResource != nullptr)
		{
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
}
