#ifndef MESH_H
#define MESH_H
#include "Entity.h"
#include <vector>
#include <map>
#include <glm\glm.hpp>

using namespace RenderSystem;
namespace RenderSystem {
	class Mesh
		:public Entity {
	public:
		Mesh();
		virtual ~Mesh();
		Mesh(const Mesh& mesh);
	public:
		virtual void draw();
		virtual void setVertices(std::vector<Vertex> vertices);
		virtual void setIndex(std::vector<unsigned int> indices);
		virtual 
		void loadFromFile(const char* filename);
		//void addTexture(Texture::ptr texture);

	public:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int vao, vbo, ebo;
	};
}

#endif // !MESH_H

