#pragma once

#include "StrongPointer.hpp"
#include "VertexDescription.hpp"
#include "Resource.hpp"
#include "Handled.hpp"

#include <vector>

// fw
struct aiNode;
struct aiMesh;

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;

	enum EMeshResourceSource
	{
		E_PRIMITIVE_MESH	= 0x00010000,
		E_ASSIMP_MESH		= 0x00020000,

		E_PRIMITIVE_CUBE	= (1 | E_PRIMITIVE_MESH),
		E_PRIMITIVE_QUAD	= (2 | E_PRIMITIVE_MESH),
		E_PRIMITIVE_AXIS	= (3 | E_PRIMITIVE_MESH),
	};

	struct MeshResourceInfos
	{
		EMeshResourceSource	eSource;
		union
		{
			uint32			iVertexProperties;
			const aiMesh*	pAiMesh;
		};
	};

	class MeshResource : public Handled, public Resource<MeshResourceInfos>
	{
	public:

		virtual ~MeshResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:

		friend class Mesh;

		MeshResource();

		MeshResourceInfos		m_oResourceInfos;
		VertexDescription		m_oVertexDescription;
		uint32					m_iPrimitiveType;
		std::vector<uint32>		m_oIndice;
		std::vector<float32>	m_oVerticeData;
		uint32					m_iVerticeCount;
		uint32					m_iVertexToDrawCount;
		uint32					m_iVbo;
		uint32					m_iEbo;

		ErrorCode				LoadPrimitiveQuad(uint32 iVertexProperties);
		ErrorCode				LoadPrimitiveCube(uint32 iVertexProperties);
		ErrorCode				LoadPrimitiveMatrixAxis(uint32 iVertexProperties);

		ErrorCode				LoadAssimpMesh(const aiMesh* pMesh);

	private:
		friend SpecificResourceManager<MeshResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const MeshResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <> struct hash<ft::MeshResourceInfos>
	{
		size_t	operator ()	(const ft::MeshResourceInfos& oObj) const
		{
			return hash<int>()((int)oObj.eSource) ^ hash<void*>()((void*)oObj.pAiMesh);
		}
	};
}
