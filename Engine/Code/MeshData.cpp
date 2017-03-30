
#include "MeshData.hpp"

#include "OpenGL.hpp"

#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace ft
{
	MeshData::MeshData()
		: m_oVertexDescription()
		, m_iPrimitiveType(GL_TRIANGLES)
		, m_oIndice()
		, m_oVerticeData()
		, m_iVerticeCount(0)
		, m_iVertexToDrawCount(0)
	{
	}

	MeshData::~MeshData()
	{
	}

	bool	 MeshData::IsValid() const
	{
		return m_oVertexDescription.IsValid()
			&& m_oVerticeData.size() > 0
			&& m_iVerticeCount == m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount()
			&& m_iVertexToDrawCount > 0;
	}

	// voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes
	// pour le moment, flemme
	ErrorCode	MeshData::MakePrimitiveQuad(uint32 /*iVertexProperties*/)
	{
		static const float32 oVert[] =
		{
			// pos				   uv
			 1.0f,  1.0f,  0.0f,   0.0f, 0.0f,
			 1.0f, -1.0f,  0.0f,   0.0f, 1.0f,
			-1.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f,   1.0f, 1.0f
		};

		static const uint32 oId[] =
		{
			0, 1, 2, 1, 2, 3
		};

		m_oVerticeData.assign((const float32*)oVert, (const float32*)((uint8*)oVert + sizeof(oVert)));
		m_oIndice.assign((const uint32*)oId, (const uint32*)((uint8*)oId + sizeof(oId)));

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		m_iPrimitiveType = GL_TRIANGLES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_oIndice.size();

		return FT_OK;
	}

	// voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes
	// pour le moment, flemme
	ErrorCode	MeshData::MakePrimitiveCube(uint32 /*iVertexProperties*/)
	{
		static const float32 oVert[] =
		{
			// pos				   uv
			 0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f
		};

		static const uint32 oId[] =
		{
			 0,  1,  2,  0,  2,  3,
			 4,  5,  6,  4,  6,  7,
			 8,  9, 10,  8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23
		};

		m_oVerticeData.assign((float32*)oVert, (float32*)((uint8*)(oVert) + sizeof(oVert)));
		m_oIndice.assign((uint32*)oId, (uint32*)((uint8*)(oId) + sizeof(oId)));

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);
		m_iPrimitiveType = GL_TRIANGLES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_oIndice.size();

		return FT_OK;
	}

	// voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes
	// pour le moment, flemme
	ErrorCode	MeshData::MakePrimitiveMatrixAxis(uint32 /*iVertexProperties*/)
	{
		static const float32 oVert[] =
		{
			// pos				color
			0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f,	0.0f, 0.0f, 1.0f, 1.0f,
		};

		m_oVerticeData.assign((float32*)oVert, (float32*)((uint8*)(oVert) + sizeof(oVert)));
		m_oIndice.clear();

		FT_TEST(m_oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_COLOR) == FT_OK);
		m_iPrimitiveType = GL_LINES;
		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		m_iVertexToDrawCount = m_iVerticeCount;

		return FT_OK;
	}

	ErrorCode	MeshData::MakeFromAssimpMesh(const aiMesh* pMesh)
	{
		FT_ASSERT(pMesh != nullptr);

		// S'il n'y a rien à récupérer, on sort
		if (pMesh->mNumVertices == 0)
			return FT_FAIL;

		// Récupération du type de primitive à laquelle correspondent les données
		switch (pMesh->mPrimitiveTypes)
		{
		case aiPrimitiveType_POINT:		{ m_iPrimitiveType = GL_POINTS; break; }
		case aiPrimitiveType_LINE:		{ m_iPrimitiveType = GL_LINES; break; }
		case aiPrimitiveType_TRIANGLE:	{ m_iPrimitiveType = GL_TRIANGLES; break; }
		default:						{ return FT_FAIL; }
		}

		// Récupération des propriétés de vertex disponibles
		uint32 iVertexProperties = E_VERTEX_PROP_POSITION;
		if (pMesh->HasTextureCoords(0))
			iVertexProperties |= E_VERTEX_PROP_UV;
		FT_TEST(m_oVertexDescription.Create(iVertexProperties) == FT_OK);

		// Récupération des données de vertex et d'indices
		// Réinitialisation, puis allocation de la mémoire nécessaire
		m_oVerticeData.clear();
		m_oIndice.clear();
		m_oVerticeData.reserve(pMesh->mNumVertices * m_oVertexDescription.GetVertexSize());
		m_oIndice.reserve(pMesh->mNumFaces * 3);

		// Vertice
		for (uint32 i = 0, iCount = pMesh->mNumVertices; i < iCount; ++i)
		{
			const aiVector3D* pPos = pMesh->mVertices + i;
			m_oVerticeData.push_back(pPos->x);
			m_oVerticeData.push_back(pPos->y);
			m_oVerticeData.push_back(pPos->z);
			if (iVertexProperties & E_VERTEX_PROP_UV)
			{
				const aiVector3D* pTexCoord = pMesh->mTextureCoords[0] + i;
				m_oVerticeData.push_back(pTexCoord->x);
				m_oVerticeData.push_back(pTexCoord->y);
			}
		}

		// Indices
		for (uint32 i = 0, iCount = pMesh->mNumFaces; i < iCount; ++i)
		{
			const aiFace& oFace = pMesh->mFaces[i];
			FT_ASSERT(oFace.mNumIndices == 3);
			m_oIndice.push_back(oFace.mIndices[0]);
			m_oIndice.push_back(oFace.mIndices[1]);
			m_oIndice.push_back(oFace.mIndices[2]);
		}

		m_iVerticeCount = m_oVerticeData.size() / m_oVertexDescription.GetVertexElementCount();
		if (m_oIndice.size() > 0)
			m_iVertexToDrawCount = m_oIndice.size();
		else
			m_iVertexToDrawCount = m_iVerticeCount;

		return FT_OK;
	}
}
