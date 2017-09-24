#pragma once
#include <memory>
namespace Core {

	class Group;
	class Mesh;
	class TransformObject;
	class Billboard;
	class Sprite;
	class NodeVisitor {
	public:
		typedef std::shared_ptr<NodeVisitor> ptr;
		enum TraversalMode
		{
			TRAVERSE_NONE,
			TRAVERSE_PARENTS,
			TRAVERSE_ALL_CHILDREN,
			TRAVERSE_ACTIVE_CHILDREN
		};

		enum VisitorType
		{
			NODE_VISITOR = 0,
			UPDATE_VISITOR,
			EVENT_VISITOR,
			COLLECT_OCCLUDER_VISITOR,
			CULL_VISITOR,
			INTERSECTION_VISITOR
		};

		NodeVisitor(TraversalMode tm = TRAVERSE_NONE);

		NodeVisitor(VisitorType type, TraversalMode tm = TRAVERSE_NONE);

		inline void setVisitorType(VisitorType type) { _visitorType = type; }		
		inline VisitorType getVisitorType() const { return _visitorType; }

		virtual void apply(Group& group);
		virtual void apply(Mesh& mesh);
		virtual void apply(Node& node);
		virtual void apply(TransformObject& object);
		virtual void apply(Billboard& billboard);
		virtual void apply(Sprite& sprite);
	protected:
		VisitorType                     _visitorType;
		unsigned int                    _traversalNumber;
	};
}