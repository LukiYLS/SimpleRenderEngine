#pragma once
#include "NodeVisitor.h"
#include "Group.h"
#include "../Utils/BoundingSphere.h"
#include "../Utils/BoundingBox.h"
#include <vector>
using namespace Utils;
namespace Core {	
	class SubMesh;
	class Light;
	class Node {
	public:
		typedef std::shared_ptr<Node> ptr;
	public:
		Node();
		Node(const Node&);

		virtual Light* asLight() { return 0; }
		virtual const Light* asLight() const { return 0; }

		virtual Group* asGroup() { return 0; }
		virtual const Group* asGroup() const { return 0; }

		virtual SubMesh* asSubMesh() { return 0; }
		virtual const SubMesh* asSubMesh() const { return 0; }

		virtual Mesh* asMesh() { return 0; }
		virtual const Mesh* asMesh() const { return 0; }

		virtual Node* asNode() { return this; }	
		virtual const Node* asNode() const { return this; }	

		inline Group* getParent(){}
		inline const Group* getParent()const{}

		//virtual TransformObject* asTransformObject() { return 0; }	
		//virtual const TransformObject* asTransformObject() const { return 0; }

		virtual void accept(NodeVisitor& nv);
		virtual void ascend(NodeVisitor& nv);
		virtual void traverse(NodeVisitor& nv){}

		typedef std::vector<Group::ptr> ParentList;
		inline const ParentList& getParents() const { return _parents; }	
		inline ParentList getParents() { return _parents; }
		inline Group* getParent(unsigned int i) { return _parents[i].get(); }
		inline const Group* getParent(unsigned int i) const { return _parents[i].get(); }
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