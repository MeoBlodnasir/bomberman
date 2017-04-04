#pragma once

#include "Core/Path.hpp"
#include "Math/Matrix44.hpp"
#include "Resource/Resource.hpp"
#include "Graphics/MeshResource.hpp"

namespace ft
{
	// fw
	template <typename TResourceType> class SpecificResourceManager;

	struct ModelResourceInfos
	{
		Path oFilePath;
	};

	class ModelResource : public Resource<ModelResourceInfos>
	{
	public:

		struct Node
		{
			Matrix44			mLocalTransform;
			int32				iParentIndex;
			std::vector<uint32>	oMeshIndice;

			Node() : mLocalTransform(1.f) {}
		};

		virtual ~ModelResource();

		virtual bool	IsLoadedAndValid() const override;

	protected:
		friend class Model;

		ModelResource();

		ModelResourceInfos					m_oResourceInfos;
		std::vector<Node>					m_oNodes;
		std::vector< SPtr<MeshResource> >	m_oMeshResources;

	private:
		friend SpecificResourceManager<ModelResource>;

		virtual ErrorCode	Load(ResourceManager& oResourceManager, const ModelResourceInfos& oInfos) override;
		virtual ErrorCode	Unload() override;
	};
}

// Nécessaire pour SpecificResourceManager
namespace std
{
	template <> struct hash<ft::ModelResourceInfos>
	{
		size_t	operator ()	(const ft::ModelResourceInfos& oObj) const
		{
			return hash<std::string>()(oObj.oFilePath.GetFullPath());
		}
	};
}
