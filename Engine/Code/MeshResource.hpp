#pragma once

#include "StrongPointer.hpp"
#include "VertexDescription.hpp"

#include <vector>

// fw
struct aiNode;
struct aiMesh;

namespace ft
{
	// Utiliser une classe générique de ressource, plus tard, quand elle existera, bitch
	struct MeshResource : public CountableSPtr
	{
		VertexDescription		oVertexDescription;
		std::vector<uint32>		oIndice;
		std::vector<float32>	oVerticeData;

		ErrorCode	MakePrimitiveQuad(uint32 iVertexProperties);
		ErrorCode	MakePrimitiveCube(uint32 iVertexProperties);

		ErrorCode	MakeFromAssimpMesh(const aiMesh* pMesh);
	};
}
