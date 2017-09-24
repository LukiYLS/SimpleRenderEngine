#pragma once
#include "NodeVisitor.h"
#include "Group.h"

#include "../Utils/BoundingSphere.h"
#include "../Utils/BoundingBox.h"
#include <vector>
using namespace Utils;
namespace Core {
	class Mesh;
	class RenderObject
	class Node {
	public:
		typedef std::shared_ptr<Node> ptr;
	public:
		Node();
		Node(const Node&);

		virtual RenderObject* asRenderObject() { return 0; }
		virtual const RenderObject* asRenderObject()const { return 0; }

		virtual Mesh* asMesh() { return 0; }
		virtual const Mesh* asMesh() const { return 0; }

		virtual Node::ptr asNode() { return (ptr)this; }	
		virtual const Node::ptr asNode() const { return (ptr)this; }

		virtual Group::ptr asGroup() { return 0; }
		virtual const Group::ptr asGroup() const { return 0; }

		//virtual TransformObject* asTransformObject() { return 0; }	
		//virtual const TransformObject* asTransformObject() const { return 0; }

		virtual void accept(NodeVisitor& nv);
		virtual void ascend(NodeVisitor& nv);
		virtual void traverse(NodeVisitor& nv){}

		typedef std::vector<Group::ptr> ParentList;
		inline const ParentList& getParents() const { return _parents; }	
		inline ParentList getParents() { return _parents; }
		inline Group::ptr getParent(unsigned int i) { return _parents[i]; }
		inline const Group::ptr getParent(unsigned int i) const { return _parents[i]; }
		inline unsigned int getNumParents() const { return static_cast<unsigned int>(_parents.size()); }
		
		
		void setInitialBound(const BoundingSphere& bsphere) { _initialBound = bsphere; dirtyBound(); }

		/** Set the initial bounding volume to use when computing the overall bounding volume.*/
		const BoundingSphere& getInitialBound() const { return _initialBound; }
		void dirtyBound();
		virtual BoundingSphere computeBound() const;
	public:

		
	protected:

		void addParent(Group::ptr parent);
		void removeParent(Group::ptr parent);
		friend class Group;
		ParentList _parents;
		BoundingSphere _initialBound;
		BoundingSphere _boundingSphere;
		bool     _boundingSphereComputed;
	};
}