#pragma once

#include "MeshData.hpp"
#include "HierarchyNode.hpp"
#include "Matrix44.hpp"
#include "Namable.hpp"

namespace ft
{
	// fw
	class Path;

	struct ModelNodeResource : public HierarchyNode, public Namable
	{
		typedef HierarchyNodeIterator<		ModelNodeResource>	iterator;
		typedef HierarchyNodeIterator<const ModelNodeResource>	const_iterator;

		Matrix44				mLocalTransform;
		int32					iParentIndex;
		std::vector<uint32>		oMeshIndice;

		ModelNodeResource() : HierarchyNode(), mLocalTransform(1), oMeshIndice() {}
	};

	// Utiliser une classe générique de ressource, plus tard, quand elle existera, bitch
	struct ModelResource : public CountableSPtr
	{
		SPtr<ModelNodeResource>				xRootNode;
		std::vector<ModelNodeResource*>		oNodes;
		std::vector< SPtr<MeshData> >	oMeshResources;

		ErrorCode	LoadFromFile(const Path& oModelFilePath);
	};
}
