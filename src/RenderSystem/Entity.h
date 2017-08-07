#ifndef ENTITY_H
#define ENTITY_H
#include <memory>
#include <glm\glm.hpp>

namespace RenderSystem {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};	
	class Entity {
	public:
		typedef std::shared_ptr<Entity> ptr;
	public:
		Entity() {}
		virtual ~Entity() {}
	public:
		virtual void createBuffer() = 0;
		virtual void draw() = 0;
		virtual void setVertices() = 0;
		virtual void setIndex() = 0;
	};
}
#endif // !ENTITY_H
