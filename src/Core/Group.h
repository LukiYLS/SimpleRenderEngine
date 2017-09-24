#pragma once
#include "Node.h"
namespace Core {
	typedef std::vector<Node::ptr> NodeList;
	class Group
		:public Node{

	public:
		typedef std::shared_ptr<Group> ptr;

		Group();
		virtual Group::ptr asGroup() { return (ptr)this; }
		virtual const Group::ptr asGroup() const { return (ptr)this; }

		virtual void traverse(NodeVisitor& nv);
		virtual bool addChild(Node::ptr child);
		virtual bool insertChild(unsigned int index, Node::ptr child);
		virtual bool removeChild(Node::ptr child);

		virtual bool removeChildren(unsigned int pos, unsigned int numChildrenToRemove);
		virtual bool replaceChild(Node *origChild, Node* newChild);		
		virtual unsigned int getNumChildren() const;

		virtual bool setChild(unsigned  int i, Node* node);		
		inline Node::ptr getChild(unsigned  int i) { return _children[i].get(); }		
		inline const Node::ptr getChild(unsigned  int i) const { return _children[i].get(); }

		inline bool removeChild(unsigned int pos, unsigned int numChildrenToRemove = 1)
		{
			if (pos<_children.size()) return removeChildren(pos, numChildrenToRemove);
			else return false;
		}
		inline unsigned int getChildIndex(const Node::ptr node) const
		{
			for (unsigned int childNum = 0; childNum<_children.size(); ++childNum)
			{
				if (_children[childNum] == node) return childNum;
			}
			return static_cast<unsigned int>(_children.size()); // node not found.
		}

		virtual BoundingSphere computeBound() const;
	protected:

		virtual ~Group();

		virtual void childRemoved(unsigned int /*pos*/, unsigned int /*numChildrenToRemove*/) {}
		virtual void childInserted(unsigned int /*pos*/) {}

		NodeList _children;

	};
}
/*
Node *a = new Node
Group *g = new Group;
g.addChild(a);
Group *g1 = new Group;
g1.addChild(g);

Group scene = new Group;
scnee.addChid(gl);

update(){

for(children)
update;
scene.add

*/