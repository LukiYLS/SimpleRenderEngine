#include "Node.h"

namespace Core {

	Node::Node() {

	}

	void Node::addParent(Group::ptr parent)
	{
		_parents.push_back(parent);
	}

	void Node::removeParent(Group::ptr parent)
	{
		ParentList::iterator pitr = std::find(_parents.begin(), _parents.end(), parent);
		if (pitr != _parents.end()) _parents.erase(pitr);
	}

	void Node::accept(NodeVisitor& nv)
	{
		
	}

	void Node::dirtyBound()
	{
		if (_boundingSphereComputed)
		{
			_boundingSphereComputed = false;

			// dirty parent bounding sphere's to ensure that all are valid.
			for (ParentList::iterator itr = _parents.begin();
				itr != _parents.end();
				++itr)
			{
				(*itr)->dirtyBound();
			}

		}
	}
}