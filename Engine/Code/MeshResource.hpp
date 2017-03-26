#pragma once

#include "StrongPointer.hpp"
#include "VertexDescription.hpp"

#include <vector>

// fw
struct aiNode;
struct aiMesh;

namespace ft
{
	// Maintenair la correspondance avec les GLenum
	enum EPrimitiveType
	{
		E_POINTS			= 0,
		E_LINES				= 1,
		E_LINE_STRIP		= 3,
		E_TRIANGLES			= 4,
		E_TRIANGLE_STRIP	= 5,
		E_TRIANGLE_FAN		= 6
	};

	// Utiliser une classe générique de ressource, plus tard, quand elle existera, bitch
	class MeshResource : public CountableSPtr
	{
	public:

		MeshResource();
		virtual ~MeshResource();

		ErrorCode	MakePrimitiveQuad(uint32 iVertexProperties);
		ErrorCode	MakePrimitiveCube(uint32 iVertexProperties);
		ErrorCode	MakePrimitiveMatrixAxis(uint32 iVertexProperties);

		ErrorCode	MakeFromAssimpMesh(const aiMesh* pMesh);

		bool		IsValid() const;

	private:

		friend class Mesh;

		VertexDescription		m_oVertexDescription;
		EPrimitiveType			m_ePrimitiveType;
		std::vector<uint32>		m_oIndice;
		std::vector<float32>	m_oVerticeData;
		uint32					m_iVerticeCount;
		uint32					m_iVertexToDrawCount;
	};
}
