#pragma once

// IHierarchyNode est l'interface générique pour créer un noeud de graphe hiérarchique.

namespace ft
{
	class IHierarchyNode
	{
	public:

		virtual			IHierarchyNode*	GetParent()			= 0;
		virtual const	IHierarchyNode*	GetParent() const	= 0;
		virtual			IHierarchyNode*	GetChild()			= 0;
		virtual const	IHierarchyNode*	GetChild() const	= 0;
		virtual			IHierarchyNode*	GetSibling()		= 0;
		virtual const	IHierarchyNode*	GetSibling() const	= 0;
	};
}
