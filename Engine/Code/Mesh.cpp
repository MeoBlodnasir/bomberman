
#include "Mesh.hpp"

#include "OpenGL.hpp"
#include "VertexLocation.h"

namespace ft
{
	Mesh::Mesh()
		: m_xMeshResource(nullptr)
		, m_iVbo(0)
		, m_iEbo(0)
	{
	}

	Mesh::~Mesh()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Mesh::Create(const SPtr<MeshResource>& xMeshResource)
	{
		if (	xMeshResource == nullptr
			||  xMeshResource->m_oVerticeData.size() == 0
			|| !xMeshResource->m_oVertexDescription.IsValid()
			||  xMeshResource->m_oVerticeData.size() % (xMeshResource->m_oVertexDescription.GetVertexSize() / sizeof(GLfloat)) != 0)
			return FT_FAIL;

		if (m_iHandle != 0)
		{
			FT_TEST_RETURN(Destroy() == FT_OK, FT_FAIL);
		}

		FT_GL_ASSERT( glGenVertexArrays(1, &m_iHandle) );
		FT_GL_ASSERT( glBindVertexArray(m_iHandle) );

		if (xMeshResource->m_oIndice.size() > 0)
		{
			FT_GL_ASSERT( glGenBuffers(1, &m_iEbo) );
			FT_GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEbo) );
			FT_GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, xMeshResource->m_oIndice.size() * sizeof(GLuint), (GLuint*)xMeshResource->m_oIndice.data(), GL_STATIC_DRAW) );
		}

		FT_GL_ASSERT( glGenBuffers(1, &m_iVbo) );
		FT_GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_iVbo) );
		FT_GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, xMeshResource->m_oVerticeData.size() * sizeof(GLfloat), (GLfloat*)xMeshResource->m_oVerticeData.data(), GL_STATIC_DRAW) );

		uint32 iProperties = xMeshResource->m_oVertexDescription.GetProperties();
		uint32 iVertexSize = xMeshResource->m_oVertexDescription.GetVertexSize();
		uint32 iPropertyElementCount = 0;
		uint64 iCurrentStride = 0;
		for (uint32 i = 0; i < FT_VERTEX_LOCATION_COUNT; ++i)
		{
			if (iProperties & (1 << i))
			{
				FT_ASSERT(iCurrentStride == xMeshResource->m_oVertexDescription.GetPropertyStride((EVertexProperty)(1 << i)));
				iPropertyElementCount = VertexDescription::s_iElementCount[i];
				FT_GL_ASSERT( glVertexAttribPointer(i, iPropertyElementCount, GL_FLOAT, GL_FALSE, iVertexSize, (GLvoid*)iCurrentStride) );
				FT_GL_ASSERT( glEnableVertexAttribArray(i) );
				iCurrentStride += iPropertyElementCount * sizeof(GLfloat);
			}
		}

		FT_GL_ASSERT( glBindVertexArray(0) );

		m_xMeshResource = xMeshResource;

		return IsValid() ? FT_OK : FT_FAIL;
	}

	ErrorCode	Mesh::Destroy()
	{
		FT_GL_ASSERT( glDeleteBuffers(1, &m_iVbo) );
		FT_GL_ASSERT( glDeleteBuffers(1, &m_iEbo) );
		FT_GL_ASSERT( glDeleteVertexArrays(1, &m_iHandle) );
		m_iVbo = 0;
		m_iEbo = 0;

		m_xMeshResource = nullptr;

		return Handled::Destroy();
	}

	bool	Mesh::IsValid() const
	{
		return m_iHandle != 0 && m_iVbo != 0 && m_xMeshResource->IsValid();
	}

	void	Mesh::Draw() const
	{
		FT_ASSERT(IsValid());

		FT_GL_ASSERT( glBindVertexArray(m_iHandle) );

		if (m_iEbo != 0)
		{
			FT_GL_ASSERT( glDrawElements((GLenum)m_xMeshResource->m_ePrimitiveType, m_xMeshResource->m_iVertexToDrawCount, GL_UNSIGNED_INT, 0) );
		}
		else
		{
			FT_GL_ASSERT( glDrawArrays((GLenum)m_xMeshResource->m_ePrimitiveType, 0, m_xMeshResource->m_iVertexToDrawCount) );
		}

		FT_GL_ASSERT( glBindVertexArray(0) );
	}
}
