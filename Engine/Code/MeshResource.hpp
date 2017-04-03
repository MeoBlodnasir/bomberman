#pragma once

#include "StrongPointer.hpp"
#include "VertexDescription.hpp"
#include "Resource.hpp"
#include "Handled.hpp"
#include "Path.hpp"

#include <vector>

// fw
struct aiMesh;
struct aiMaterial;

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;
	class MaterialResource;
	class ShaderProgramResource;
	class RenderTechnique;

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
		// E_PRIMITIVE_MESH
		uint32					iVertexProperties;
		const RenderTechnique*	pRenderTechnique;
		// E_ASSIMP_MESH
		Path					oLocalPath;
		const aiMesh*			pAiMesh;
		const aiMaterial*		pAiMaterial;

		MeshResourceInfos()
			: eSource(E_PRIMITIVE_MESH)
			, iVertexProperties(0)
			, pRenderTechnique(nullptr)
			, pAiMesh(nullptr)
			, pAiMaterial(nullptr)
		{}
	};

	class MeshResource : public Handled, public Resource<MeshResourceInfos>
	{
	public:

		virtual ~MeshResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:
		friend class Mesh;

		MeshResourceInfos		m_oResourceInfos;
		VertexDescription		m_oVertexDescription;
		uint32					m_iPrimitiveType;
		std::vector<uint32>		m_oIndice;
		std::vector<float32>	m_oVerticeData;
		uint32					m_iVerticeCount;
		uint32					m_iVertexToDrawCount;
		uint32					m_iVbo;
		uint32					m_iEbo;

		SPtr<MaterialResource>		m_xMaterialResource;
		SPtr<ShaderProgramResource>	m_xShaderProgramResource;
		
		MeshResource();

		ErrorCode				LoadPrimitiveQuad(ResourceManager& oResourceManager, uint32 iVertexProperties);
		ErrorCode				LoadPrimitiveCube(ResourceManager& oResourceManager, uint32 iVertexProperties);
		ErrorCode				LoadPrimitiveMatrixAxis(ResourceManager& oResourceManager, uint32 iVertexProperties);

		ErrorCode				LoadAssimpMesh(ResourceManager& oResourceManager, const MeshResourceInfos& oInfos);

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
