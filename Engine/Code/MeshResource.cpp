
#include "MeshResource.hpp"

#include <assimp\scene.h>
#include <assimp\mesh.h>

namespace ft
{
	// voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes
	// pour le moment, flemme
	ErrorCode	MeshResource::MakePrimitiveQuad(uint32 /*iVertexProperties*/)
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

		oVerticeData.assign((const float32*)oVert, (const float32*)((uint8*)oVert + sizeof(oVert)));
		oIndice.assign((const uint32*)oId, (const uint32*)((uint8*)oId + sizeof(oId)));
		FT_TEST(oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);

		return FT_OK;
	}

	// voir comment utiliser les propriétés pour assigner efficacement les valeurs correspondantes
	// pour le moment, flemme
	ErrorCode	MeshResource::MakePrimitiveCube(uint32 /*iVertexProperties*/)
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

		oVerticeData.assign((float32*)oVert, (float32*)((uint8*)(oVert) + sizeof(oVert)));
		oIndice.assign((uint32*)oId, (uint32*)((uint8*)(oId) + sizeof(oId)));
		FT_TEST(oVertexDescription.Create(E_VERTEX_PROP_POSITION | E_VERTEX_PROP_UV) == FT_OK);

		return FT_OK;
	}

	ErrorCode	MeshResource::MakeFromAssimpMesh(const aiMesh* pMesh)
	{
		FT_ASSERT(pMesh != nullptr);

		// S'il n'y a rien à récupérer, on sort
		if (pMesh->mNumVertices == 0)
			return FT_FAIL;

		// Récupération des propriétés de vertex disponibles
		uint32 iVertexProperties = E_VERTEX_PROP_POSITION;
		if (pMesh->HasTextureCoords(0))
			iVertexProperties |= E_VERTEX_PROP_UV;
		FT_TEST(oVertexDescription.Create(iVertexProperties) == FT_OK);

		// Récupération des données de vertex et d'indices
		// Réinitialisation, puis allocation de la mémoire nécessaire
		oVerticeData.clear();
		oIndice.clear();
		oVerticeData.reserve(pMesh->mNumVertices * oVertexDescription.GetVertexSize());
		oIndice.reserve(pMesh->mNumFaces * 3);

		// Vertice
		for (uint32 i = 0, iCount = pMesh->mNumVertices; i < iCount; ++i)
		{
			const aiVector3D* pPos = pMesh->mVertices + i;
			oVerticeData.push_back(pPos->x);
			oVerticeData.push_back(pPos->y);
			oVerticeData.push_back(pPos->z);
			if (iVertexProperties & E_VERTEX_PROP_UV)
			{
				const aiVector3D* pTexCoord = pMesh->mTextureCoords[0] + i;
				oVerticeData.push_back(pTexCoord->x);
				oVerticeData.push_back(pTexCoord->y);
			}
		}

		// Indices
		for (uint32 i = 0, iCount = pMesh->mNumFaces; i < iCount; ++i)
		{
			const aiFace& oFace = pMesh->mFaces[i];
			FT_ASSERT(oFace.mNumIndices == 3);
			oIndice.push_back(oFace.mIndices[0]);
			oIndice.push_back(oFace.mIndices[1]);
			oIndice.push_back(oFace.mIndices[2]);
		}

		return FT_OK;
	}
}
