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
			addChild(itr->get());
		}
	}

	Group::~Group()
	{		
		for (NodeList::iterator itr = _children.begin();
			itr != _children.end();
			++itr)
		{
			(*itr)->removeParent(this);
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


	bool Group::addChild(Node* child)
	{
		return Group::insertChild(_children.size(), child);
	}
	bool Group::insertChild(unsigned int index, Node* child)
	{
		if (!child) return false;
		if (index >= _children.size())
		{
			index = _children.size();      
			_children.push_back((ptr)child);
		}
		else
		{
			_children.insert(_children.begin() + index, (ptr)child);
		}
		child->addParent(this);
		return true;
	}
	unsigned int Group::getNumChildren() const
	{
		return static_cast<unsigned int>(_children.size());
	}

	bool Group::removeChild(Node* child)
	{
		unsigned int pos = getChildIndex(child);
		if (pos<_children.size()) return removeChildren(pos, 1);
		else return false;
	}
	bool Group::removeChildren(unsigned int pos, unsigned int numChildrenToRemove)
	{
		if (pos < _children.size() && numChildrenToRemove>0)
		{
			unsigned int endOfRemoveRange = pos + numChildrenToRemove;
			for (unsigned i = pos; i < endOfRemoveRange; ++i)
			{
				Node* child = _children[i].get();
				child->removeParent(this);
			}
			childRemoved(pos, endOfRemoveRange - pos);

			_children.erase(_children.begin() + pos, _children.begin() + endOfRemoveRange);
			dirtyBound();
			return true;
		}
		return false;
	}
	bool Group::replaceChild(Node *origChild, Node* newChild)
	{
		if (newChild == NULL || origChild == newChild) return false;

		unsigned int pos = getChildIndex(origChild);
		if (pos<_children.size())
		{
			return setChild(pos, newChild);
		}
		return false;
	}
	bool Group::setChild(unsigned  int i, Node* node)
	{
		if (i < _children.size() && node)
		{
			Node::ptr origNode = _children[i];
			origNode->removeParent(this);
			_children[i] = (ptr)node;
			
			node->addParent(this);
			return true;
			dirtyBound();
		}
		return false;
	}
	BoundingSphere Group::computeBound() const
	{
		
	}
}