#include <glew\glew.h>
#include "RenderSystem.h"
#include "Scene.h"
#include "Entity.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Win.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <memory>
using namespace RenderSystem;
using namespace std;

Mesh::ptr loadObj(const char* path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Error::ObjLoader, could not open obj file:"
			<< path << " for reading." << std::endl;
		return false;
	}
	std::string line;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<glm::vec3> positions;
	while (std::getline(file, line))
	{
		if (line.substr(0, 2) == "vt")     // 顶点纹理坐标数据
		{
			istringstream s(line.substr(2));
			glm::vec2 v;
			s >> v.x; s >> v.y;
			uvs.push_back(v);
		}
		else if (line.substr(0, 2) == "vn") // 顶点法向量数据
		{
			istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;			
			normals.push_back(v);
		}
		else if (line.substr(0, 1) == "v")  // 顶点位置数据
		{
			istringstream s(line.substr(1));
			glm::vec3 v;
			s >> v.x; s >> v.y; s >> v.z;			
			positions.push_back(v);
		}
		else if (line.substr(0, 1) == "f")  // 面数据
		{
			Vertex v;
			//cout << "f ";
			istringstream vtns(line.substr(1));
			string vtn;
			while (vtns >> vtn)             // 处理一行中多个顶点属性
			{
				Vertex vertex;
				replace(vtn.begin(), vtn.end(), '/', ' ');
				istringstream ivtn(vtn);
				int uvindex, normalindex, posindex;
				if (vtn.find("  ") != string::npos) // 没有纹理数据，注意这里是2个空格
				{

					ivtn >> posindex >> normalindex;

					vertex.Position = positions[posindex - 1];
					vertex.Normal = normals[normalindex - 1];
				}
				else
				{
					ivtn >> posindex >> uvindex >> normalindex;

					vertex.Position = positions[posindex - 1];
					vertex.TexCoords = uvs[uvindex - 1];
					vertex.Normal = normals[normalindex - 1];

				}
				vertices.push_back(vertex);
			}			
		}
		else if (line[0] == '#')            // 注释忽略
		{
		}
		else
		{
			// 其余内容 暂时不处理
		}
	}
	Mesh::ptr mesh = std::make_shared<Mesh>();
	mesh->setVertices(vertices);
	return mesh;
}
int main()
{

	
	
	Mesh::ptr mesh = loadObj("../../../src/Data/model/cube.obj ");	
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	
	
	// camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	camera.setPerspectiveFovLHMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	Shader shader("../../../src/Data/shader/test.vs", "../../../src/Data/shader/fs");
	shader.use();
	TextureManager::Inst()->LoadTexture("../../../src/Data/texture/1.jpg", 0);
	TextureManager::Inst()->BindTexture(0);
	shader.setInt("texture", 0);
	shader.setMat4("projection", camera.getProjectMatrix());

	// camera/view transformation
	glm::mat4 view = camera.getViewMatrix();
	shader.setMat4("view", view);

	Scene::Inst()->addEntity("test", (Entity::ptr)mesh);
	Win::Inst()->createWindow();
	Win::Inst()->starup();
}