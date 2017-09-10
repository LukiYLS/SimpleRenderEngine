#ifndef ENTITY_H
#define ENTITY_H
#include <memory>
//#include <glm\glm.hpp>
#include "Light.h"
#include "data_structure.h"

namespace Basic {


	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		Vertex(){}
		Vertex(float x, float y, float z, float n1, float n2, float n3, float u, float v) :
			Position(x, y, z), Normal(n1, n2, n3), TexCoords(u, v) {}
	};	
	class Entity {//所有的模型都必须继承这个类
	public:
		typedef std::shared_ptr<Entity> ptr;
	public:
		Entity() {}
		virtual ~Entity() {}
	public:
		virtual void createBuffer() = 0;
		virtual void draw(RenderParams* params) = 0;
		virtual void setVertices(std::vector<Vertex> vertices) = 0;
		virtual void setIndex(std::vector<unsigned int> indices) = 0;
		virtual void setupLights(std::vector<Light::ptr> lights) = 0;
		virtual void addTexture(const char* texName) = 0;		
		virtual void setModelMatrix(glm::mat4x4& matrix) = 0;
	};
}
#endif // !ENTITY_H
