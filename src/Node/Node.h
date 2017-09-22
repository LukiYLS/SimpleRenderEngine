#pragma once
#include "NodeVistor.h"
#include "Group.h"
#include "../Core/Mesh.h"
#include <vector>
namespace Core {

	class Node {
	public:
		Node();
		Node(const Node&);

		virtual Mesh::ptr asMesh() { return 0; }


		typedef std::vector<Group::ptr> ParentList;
		ParentList getParents(){}
	public:

		virtual void accept(NodeVistor::ptr nv);
		virtual void ascend(NodeVistor::ptr nv);
		virtual void traverse(NodeVistor::ptr nv);
	private:
		ParentList _parents;
	};
}