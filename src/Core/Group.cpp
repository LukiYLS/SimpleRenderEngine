#include "Group.h"


namespace Core {


	Group::Group()
	{
	}
	Group::Group(const Group& group):
		Node(group)
	{
		for (NodeList::const_iterator itr = group._children.begin();
			itr != group._children.end();
			++itr)
		{			
			addChild(*itr);
		}
	}

	Group::~Group()
	{		
		for (NodeList::iterator itr = _children.begin();
			itr != _children.end();
			++itr)
		{
			(*itr)->removeParent((ptr)this);
		}

	}
	void Group::traverse(NodeVisitor& nv)
	{
		for (NodeList::iterator itr = _children.begin();
			itr != _children.end();
			++itr)
		{
			(*itr)->accept(nv);
		}
	}


	bool Group::addChild(Node::ptr child)
	{
		return Group::insertChild(_children.size(), child);
	}
	unsigned int Group::getNumChildren() const
	{
		return static_cast<unsigned int>(_children.size());
	}


	bool Group::removeChild(Node::ptr child)
	{
		unsigned int pos = getChildIndex(child);
		if (pos<_children.size()) return removeChildren(pos, 1);
		else return false;
	}

	BoundingSphere Group::computeBound() const
	{
	}
}