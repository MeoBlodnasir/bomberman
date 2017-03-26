#pragma once

#include "SceneNode.hpp"
#include "ModelResource.hpp"
#include "Mesh.hpp"

namespace ft
{
	class ModelNode : public SceneNode
	{
	public:

		typedef HierarchyNodeIterator<		ModelNode>	iterator;
		typedef HierarchyNodeIterator<const ModelNode>	const_iterator;

	public: // temp
		friend class Model;
		std::vector<SPtr<Mesh>>	m_oMeshes;
	};

	class Model : public SceneNode
	{
	public:

				Model();
		virtual ~Model();

		virtual ErrorCode	Create(const Model::Desc* pDesc, const SPtr<ModelResource>& xModelResource);
		virtual ErrorCode	Destroy() override;

				bool		IsValid() const;

				void		Draw() const;

	protected:

		SPtr<ModelResource>		m_xModelResource;
	public: // temp
		SPtr<ModelNode>			m_xRootNode;

	private:

		Model(const Model&) FT_DELETED;
		Model& operator = (const Model&) FT_DELETED;
	};
}
