#pragma once

#include "StrongPointer.hpp"
#include "VertexDescription.hpp"

#include <vector>

// fw
struct aiNode;
struct aiMesh;

namespace ft
{
	class MeshData : public CountableSPtr
	{
	public:

		MeshData();
		virtual ~MeshData();

		ErrorCode	MakePrimitiveQuad(uint32 iVertexProperties);
		ErrorCode	MakePrimitiveCube(uint32 iVertexProperties);
		ErrorCode	MakePrimitiveMatrixAxis(uint32 iVertexProperties);

		ErrorCode	MakeFromAssimpMesh(const aiMesh* pMesh);

		bool		IsValid() const;

	private:

		friend class Mesh;

		VertexDescription		m_oVertexDescription;
		uint32					m_iPrimitiveType;
		std::vector<uint32>		m_oIndice;
		std::vector<float32>	m_oVerticeData;
		uint32					m_iVerticeCount;
		uint32					m_iVertexToDrawCount;
	};
}
