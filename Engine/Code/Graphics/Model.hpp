#pragma once

#include "Graphics/SceneNode.hpp"
#include "Graphics/ModelResource.hpp"

namespace ft
{
	// fw
	class Mesh;

	class Model : public SceneNode
	{
	public:

		class InternalNode : public SceneNode
		{
		public:

			typedef HierarchyNodeIterator<		InternalNode>	iterator;
			typedef HierarchyNodeIterator<const InternalNode>	const_iterator;

			virtual void	Render(const RenderContext& oRenderContext) const override;

		//private:
		//	friend class Model;

			std::vector< SPtr<Mesh> >	m_oMeshes;
		};

				Model();
		virtual ~Model();

		virtual ErrorCode	Create(const Model::Desc* pDesc, const SPtr<ModelResource>& xModelResource);
		virtual ErrorCode	Destroy() override;

		virtual void		Update() override;

		virtual void		Render(const RenderContext& oRenderContext) const override;

	protected:

		SPtr<ModelResource>		m_xModelResource;
	public: // temp
		SPtr<InternalNode>		m_xRootNode;

	private:

		Model(const Model&) FT_DELETED;
		Model& operator = (const Model&) FT_DELETED;
	};
}
