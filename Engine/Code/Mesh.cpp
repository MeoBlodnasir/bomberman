
#include "Mesh.hpp"

#include "OpenGL.hpp"
#include "VertexLocation.h"

namespace ft
{
	Mesh::Mesh()
		: m_iVbo(0)
		, m_iEbo(0)
		, m_iVertexCount(0)
		, m_iVertexToDrawCount(0)
	{
	}

	Mesh::~Mesh()
	{
		FT_TEST(Destroy() == FT_OK);
	}

	ErrorCode	Mesh::Create(const SPtr<MeshResource>& xMeshResource)
	{
		if (	xMeshResource == nullptr
			||  xMeshResource->oVerticeData.size() == 0
			|| !xMeshResource->oVertexDescription.IsValid()
			||  xMeshResource->oVerticeData.size() % (xMeshResource->oVertexDescription.GetVertexSize() / sizeof(GLfloat)) != 0)
			return FT_FAIL;

		if (m_iHandle != 0)
		{
			FT_TEST_RETURN(Destroy() == FT_OK, FT_FAIL);
		}

		FT_GL_ASSERT( glGenVertexArrays(1, &m_iHandle) );
		FT_GL_ASSERT( glBindVertexArray(m_iHandle) );

		if (xMeshResource->oIndice.size() > 0)
		{
			FT_GL_ASSERT( glGenBuffers(1, &m_iEbo) );
			FT_GL_ASSERT( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEbo) );
			FT_GL_ASSERT( glBufferData(GL_ELEMENT_ARRAY_BUFFER, xMeshResource->oIndice.size() * sizeof(GLuint), (GLuint*)xMeshResource->oIndice.data(), GL_STATIC_DRAW) );
		}

		FT_GL_ASSERT( glGenBuffers(1, &m_iVbo) );
		FT_GL_ASSERT( glBindBuffer(GL_ARRAY_BUFFER, m_iVbo) );
		FT_GL_ASSERT( glBufferData(GL_ARRAY_BUFFER, xMeshResource->oVerticeData.size() * sizeof(GLfloat), (GLfloat*)xMeshResource->oVerticeData.data(), GL_STATIC_DRAW) );

		uint32 iProperties = xMeshResource->oVertexDescription.GetProperties();
		uint32 iVertexSize = xMeshResource->oVertexDescription.GetVertexSize();
		uint32 iPropertyElementCount = 0;
		uint64 iCurrentStride = 0;
		for (uint32 i = 0; i < FT_VERTEX_LOCATION_COUNT; ++i)
		{
			if (iProperties & (1 << i))
			{
				FT_ASSERT(iCurrentStride == xMeshResource->oVertexDescription.GetPropertyStride((EVertexProperty)(1 << i)));
				iPropertyElementCount = VertexDescription::s_iElementCount[i];
				FT_GL_ASSERT( glVertexAttribPointer(i, iPropertyElementCount, GL_FLOAT, GL_FALSE, iVertexSize, (GLvoid*)iCurrentStride) );
				FT_GL_ASSERT( glEnableVertexAttribArray(i) );
				iCurrentStride += iPropertyElementCount * sizeof(GLfloat);
			}
		}

		FT_GL_ASSERT( glBindVertexArray(0) );

		m_iVertexCount = xMeshResource->oVerticeData.size() / (iVertexSize / sizeof(GLfloat));

		if (m_iEbo == 0)
			m_iVertexToDrawCount = m_iVertexCount;
		else
			m_iVertexToDrawCount = xMeshResource->oIndice.size();

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

		return Handled::Destroy();
	}

	bool	Mesh::IsValid() const
	{
		return m_iHandle != 0 && m_iHandle != 0 && m_iVertexCount != 0 && m_iVertexToDrawCount != 0;
	}

	void	Mesh::Draw() const
	{
		FT_ASSERT(IsValid());

		FT_GL_ASSERT( glBindVertexArray(m_iHandle) );

		if (m_iEbo != 0)
		{
			FT_GL_ASSERT( glDrawElements(GL_TRIANGLES, m_iVertexToDrawCount, GL_UNSIGNED_INT, 0) );
		}
		else
		{
			FT_GL_ASSERT( glDrawArrays(GL_TRIANGLES, 0, m_iVertexToDrawCount) );
		}

		FT_GL_ASSERT( glBindVertexArray(0) );
	}
}
