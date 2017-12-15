#pragma once
#include "Node.h"
namespace Core {
	typedef std::vector<Node::ptr> NodeList;
	class Group
		:public Node{

	public:
		typedef std::shared_ptr<Group> ptr;

		Group();
		virtual Group* asGroup() { return this; }
		virtual const Group* asGroup() const { return this; }

		virtual void traverse(NodeVisitor& nv);
		virtual bool addChild(Node* child);

		virtual bool insertChild(unsigned int index, Node* child);
		virtual bool removeChild(Node* child);

		virtual bool removeChildren(unsigned int pos, unsigned int numChildrenToRemove);
		virtual bool replaceChild(Node *origChild, Node* newChild);		
		virtual unsigned int getNumChildren() const;

		virtual bool setChild(unsigned  int i, Node* node);		
		inline Node* getChild(unsigned  int i) { return _children[i].get(); }		
		inline const Node* getChild(unsigned  int i) const { return _children[i].get(); }

		inline bool removeChild(unsigned int pos, unsigned int numChildrenToRemove = 1)
		{
			if (pos<_children.size()) return removeChildren(pos, numChildrenToRemove);
			else return false;
		}
		inline unsigned int getChildIndex(const Node* node) const
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
