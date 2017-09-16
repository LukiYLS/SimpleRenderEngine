#include "Mesh.h"
#include "TextureManager.h"
#include <glew\glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "data_structure.h"



namespace Core {

	Mesh::Mesh()
	{
		_hasMaterial = false;
	}
	void Mesh::setProgram(Shader::ptr shader)
	{
		_shader = shader;
		//_shader->use();

		//if (_material)
	}
	void Mesh::setupLights(std::vector<Light::ptr> lights)
	{
		for (auto it = lights.begin(); it != lights.end(); it++)
		{			
			if ((*it)->getType() == DirectLight)
			{
				_shader->setVec3("DirLight.direction", (*it)->getDirection());
				_shader->setVec3("DirLight.ambient", (*it)->getAmbient());
				_shader->setVec3("DirLight.diffuse", (*it)->getDiffuse());
				_shader->setVec3("DirLight.specular", (*it)->getSpecular());
			}
			else if ((*it)->getType() == PointLight)
			{				
				_shader->setVec3("pointLight.position", (*it)->getPosition());
				_shader->setVec3("pointLight.ambient", (*it)->getAmbient());
				_shader->setVec3("pointLight.diffuse", (*it)->getDiffuse());
				_shader->setVec3("pointLight.specular", (*it)->getSpecular());
			
				_shader->setFloat("pointLight.constant", (*it)->getConstantAttenuation());
				_shader->setFloat("pointLight.linear", (*it)->getLinearAttenuation());
				_shader->setFloat("pointLight.quadratic", (*it)->getQuadraticAttenuation());
			}
			else if ((*it)->getType() == SpotLight)
			{
				_shader->setVec3("spotLight.position", (*it)->getPosition());
				_shader->setVec3("spotLight.direction", (*it)->getDirection());
				_shader->setVec3("spotLight.ambient", (*it)->getAmbient());
				_shader->setVec3("spotLight.diffuse", (*it)->getDiffuse());
				_shader->setVec3("spotLight.specular", (*it)->getSpecular());

				_shader->setFloat("spotLight.constant", (*it)->getConstantAttenuation());
				_shader->setFloat("spotLight.linear", (*it)->getLinearAttenuation());
				_shader->setFloat("spotLight.quadratic", (*it)->getQuadraticAttenuation());

				_shader->setFloat("spotLight.innerCutoff", (*it)->getInnerCutoff());
				_shader->setFloat("spotLight.outerCutoff", (*it)->getOuterCutoff());
			}
		}
	}
	void Mesh::createBuffer()
	{			
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);


		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

		if (!_indices.empty())
		{
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
		}


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);

		glBindVertexArray(0);

	}
	bool Mesh::loadFromObjFile(const char* filename)
	{
		std::ifstream file(filename);
		if (!file)
		{
			std::cerr << "Error::ObjLoader, could not open obj file:"
				<< filename << " for reading." << std::endl;
			return false;
		}
		std::string line;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> positions;
		while (std::getline(file, line))
		{
			if (line.substr(0, 2) == "vt")     // 顶点纹理坐标数据
			{
				std::istringstream s(line.substr(2));
				glm::vec2 v;
				s >> v.x; s >> v.y;
				uvs.push_back(v);
			}
			else if (line.substr(0, 2) == "vn") // 顶点法向量数据
			{
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				normals.push_back(v);
			}
			else if (line.substr(0, 1) == "v")  // 顶点位置数据
			{
				std::istringstream s(line.substr(1));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				positions.push_back(v);
			}
			else if (line.substr(0, 1) == "f")  // 面数据
			{
				Vertex v;
				//cout << "f ";
				std::istringstream vtns(line.substr(1));
				std::string vtn;
				while (vtns >> vtn)             // 处理一行中多个顶点属性
				{
					Vertex vertex;
					std::replace(vtn.begin(), vtn.end(), '/', ' ');
					std::istringstream ivtn(vtn);
					int uvindex, normalindex, posindex;
					if (vtn.find("  ") != std::string::npos) // 没有纹理数据，注意这里是2个空格
					{

						ivtn >> posindex >> normalindex;
						glm::vec3 pos = positions[posindex - 1];
						glm::vec3 normal = normals[normalindex - 1];
						vertex = Vertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z);

					}
					else
					{
						ivtn >> posindex >> uvindex >> normalindex;

						glm::vec3 pos = positions[posindex - 1];
						glm::vec3 normal = normals[normalindex - 1];
						glm::vec2 uv = uvs[uvindex - 1];
						vertex = Vertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, uv.x, uv.y);

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
		this->setVertices(vertices);
	}
	void Mesh::draw(Shader::ptr shader)
	{
	
		shader->use();
		//_shader->setMat4("modelMatrix",_modelMatrix);
		//_shader->setMat4("projectionMatrix", _camera->getProjectMatrix());
		//_shader->setMat4("viewMatrix", _camera->getViewMatrix());
		//_shader->setVec3("EyeDirection", _camera->getPosition());

		//if (_hasMaterial)
		//{
		//	_shader->setVec3("ambientMt", _material.ambient);
		//	_shader->setVec3("diffuseMt", _material.diffuse);
		//	_shader->setVec3("specularMt", _material.specular);
		//	_shader->setFloat("shininessMt", _material.shininess);
		//}
		//if (!_textures.empty())
		//{
		//	for (int i = 0; i < _textures.size(); i++)
		//	{
		//		TextureManager::Inst()->bindTexture(_textures[i], i);
		//		_shader->setInt(_textures[i], i);
		//	}
		//}

		glBindVertexArray(_vao);
		if (_indices.empty())
			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
		else
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}