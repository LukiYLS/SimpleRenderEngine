#include "GeometryFactory.h"

namespace Core {

	Mesh* GeometryFactory::MakeBox(int width, int height, int depth)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Mesh* box = new Mesh();

		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;

		vertices.push_back(Vertex(-halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 1.f));
		vertices.push_back(Vertex(-halfW, halfH, -halfD, 0.f, 0.f, -1.f, 0.f, 0.f));
		vertices.push_back(Vertex(halfW, halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 0.f));
		vertices.push_back(Vertex(halfW, -halfH, -halfD, 0.f, 0.f, -1.f, 1.f, 1.f));

		vertices.push_back(Vertex(-halfW, -halfH, halfD, -1.f, 0.f, 0.f, 0.f, 1.f));
		vertices.push_back(Vertex(-halfW, halfH, halfD, -1.f, 0.f, 0.f, 0.f, 0.f));
		vertices.push_back(Vertex(-halfW, halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 0.f));
		vertices.push_back(Vertex(-halfW, -halfH, -halfD, -1.f, 0.f, 0.f, 1.f, 1.f));

		vertices.push_back(Vertex(halfW, -halfH, halfD, 0.f, 0.f, 1.f, 0.f, 1.f));
		vertices.push_back(Vertex(halfW, halfH, halfD, 0.f, 0.f, 1.f, 0.f, 0.f));
		vertices.push_back(Vertex(-halfW, halfH, halfD, 0.f, 0.f, 1.f, 1.f, 0.f));
		vertices.push_back(Vertex(-halfW, -halfH, halfD, 0.f, 0.f, 1.f, 1.f, 1.f));

		vertices.push_back(Vertex(halfW, -halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 1.f));
		vertices.push_back(Vertex(halfW, halfH, -halfD, 1.f, 0.f, 0.f, 0.f, 0.f));
		vertices.push_back(Vertex(halfW, halfH, halfD, 1.f, 0.f, 0.f, 1.f, 0.f));
		vertices.push_back(Vertex(halfW, -halfH, halfD, 1.f, 0.f, 0.f, 1.f, 1.f));
		
		vertices.push_back(Vertex(-halfW, halfH, -halfD, 0.f, 1.f, 0.f, 0.f, 1.f));
		vertices.push_back(Vertex(-halfW, halfH, halfD, 0.f, 1.f, 0.f, 0.f, 0.f));
		vertices.push_back(Vertex(halfW, halfH, halfD, 0.f, 1.f, 0.f, 1.f, 0.f));
		vertices.push_back(Vertex(halfW, halfH, -halfD, 0.f, 1.f, 0.f, 1.f, 1.f));

		vertices.push_back(Vertex(-halfW, -halfH, halfD, 0.f, -1.f, 0.f, 0.f, 1.f));
		vertices.push_back(Vertex(-halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 0.f, 0.f));
		vertices.push_back(Vertex(halfW, -halfH, -halfD, 0.f, -1.f, 0.f, 1.f, 0.f));
		vertices.push_back(Vertex(halfW, -halfH, halfD, 0.f, -1.f, 0.f, 1.f, 1.f));

		indices = { 0,1,2,0,2,3 , 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11 , 12, 13, 14, 12, 14, 15 , 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

		box->setVertices(vertices);
		box->setIndex(indices);
		box->createBuffer();
		return box;
	}
}