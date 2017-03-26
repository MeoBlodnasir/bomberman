#pragma once

#include "HierarchyNode.hpp"
#include "Matrix44.hpp"

namespace ft
{
	enum ESceneNodeFlag
	{
		E_NONE = 0,
		E_OUTDATED_WORLD_TRANSFORM = (1 << 0),
		E_OUTDATED_LOCAL_TRANSFORM = (1 << 1)
	};

	class SceneNode : public HierarchyNode
	{
	public:

		typedef HierarchyNodeIterator<		SceneNode>	iterator;
		typedef HierarchyNodeIterator<const SceneNode>	const_iterator;

		SceneNode();
		virtual ~SceneNode();

		virtual ErrorCode		Create(const HierarchyNode::Desc* pDesc) override;
		virtual ErrorCode		Destroy() override;

		virtual void			Update();
		static	void			UpdateHierarchy(SceneNode* pNode);

		// Transformations
		inline	const Matrix44&	GetLocalTransform() const						{ FT_ASSERT(!(m_iFlags & E_OUTDATED_LOCAL_TRANSFORM)); return m_mLocal; }
		inline	const Matrix44&	GetWorldTransform() const						{ FT_ASSERT(!(m_iFlags & E_OUTDATED_WORLD_TRANSFORM)); return m_mWorld; }
				void			SetLocalTransform(const Matrix44& m);
				void			SetWorldTransform(const Matrix44& m);

		// Hiérarchie
				void			SetAsChildOf(SceneNode* pParent, bool bPreserveWorldTransform);

		// Etat
		inline	uint32			GetFlags() const									{ return m_iFlags; }
				void			SetFlag(uint32 iFlag, bool bPropagate = false);
				void			UnsetFlag(uint32 iFlag, bool bPropagate = false);

	private:

				void			UpdateLocalTransform();
				void			UpdateWorldTransform();
				void			PropagateFlag(uint32 iFlag, bool bValue);

	protected:

		uint32			m_iFlags;
		Matrix44		m_mLocal;
		Matrix44		m_mWorld;

	private:

		SceneNode(const SceneNode&) FT_DELETED;
		SceneNode& operator = (const SceneNode&) FT_DELETED;
	};

	typedef SceneNode::iterator			SceneNodeIt;
	typedef SceneNode::const_iterator	SceneNodeConstIt;
}
