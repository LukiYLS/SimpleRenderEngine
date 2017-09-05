#include "Mesh.h"
#include "TextureManager.h"
#include <glew\glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "data_structure.h"


namespace Basic {

	Mesh::Mesh()
	{
		_hasMaterial = false;
	}
	void Mesh::setProgram(Shader::ptr shader)
	{
		_shader = shader;
		_shader->use();

		//if (_material)
	}
	void Mesh::setupLights(std::vector<Light::ptr> lights)
	{
		for (auto it = lights.begin(); it != lights.end(); it++)
		{
			//有必要区分多少个点光源，多少个方向关光源，多少个聚光灯吗？
			//这里应该支持数组，考虑如何设计
			if ((*it)->getType() == DirectLight)
			{
				_shader->setVec3("DirLight.direction", (*it)->getDirection());
				_shader->setFloat("pointLight.ambientIntensity", (*it)->getAmbientIntensity());
				_shader->setFloat("pointLight.diffuseIntensity", (*it)->getDiffuseIntensity());
			}
			else if ((*it)->getType() == PointLight)
			{
				_shader->setVec3("pointLight.color", (*it)->getColor());
				_shader->setVec3("pointLight.position", (*it)->getPosition());
				_shader->setFloat("pointLight.ambientIntensity", (*it)->getAmbientIntensity());
				_shader->setFloat("pointLight.diffuseIntensity", (*it)->getDiffuseIntensity());				

				_shader->setFloat("pointLight.shiness", (*it)->getShiness());
				_shader->setFloat("pointLight.strength", (*it)->getStrength());
				_shader->setFloat("pointLight.constant", (*it)->getConstantAttenuation());
				_shader->setFloat("pointLight.linear", (*it)->getLinearAttenuation());
				_shader->setFloat("pointLight.quadratic", (*it)->getQuadraticAttenuation());
			}
			else if ((*it)->getType() == SpotLight)
			{
				_shader->setVec3("SpotLight.position", (*it)->getPosition());
				_shader->setVec3("SpotLight.direction", (*it)->getDirection());
				_shader->setFloat("SpotLight.cutoff", (*it)->getSpotCutoff());
				_shader->setFloat("pointLight.ambientIntensity", (*it)->getAmbientIntensity());
				_shader->setFloat("pointLight.diffuseIntensity", (*it)->getDiffuseIntensity());

				_shader->setFloat("SpotLight.constant", (*it)->getConstantAttenuation());
				_shader->setFloat("SpotLight.linear", (*it)->getLinearAttenuation());
				_shader->setFloat("SpotLight.quadratic", (*it)->getQuadraticAttenuation());
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
		this->setVertices(vertices);
	}
	void Mesh::draw(RenderParams* params)
	{
		//shader赋值
		_shader->use();
		//_shader->setMat4("modelMatrix", _modelMatrix);
		//glm::mat4 pjM = camera->getProjectMatrix();
		//glm::mat4 vM = camera->getProjectMatrix();
		//_shader->setMat4("projectionMatrix", camera->getProjectMatrix());
		//_shader->setMat4("viewMatrix", camera->getViewMatrix());
		//_shader->setVec3("EyeDirection", camera->Eye);
		_shader->setMat4("modelMatrix", params->m()*_modelMatrix);
		_shader->setMat4("projectionMatrix", params->p());
		_shader->setMat4("viewMatrix", params->v());
		_shader->setVec3("EyeDirection", params->eye());

		if (_hasMaterial)
		{
			_shader->setVec3("ambientMt", _material.ambient);
			_shader->setVec3("diffuseMt", _material.diffuse);
			_shader->setVec3("specularMt", _material.specular);
			_shader->setFloat("shininessMt", _material.shininess);
		}
		if (!_textures.empty())
		{
			for (int i = 0; i < _textures.size(); i++)
			{
				TextureManager::Inst()->bindTexture(_textures[i], i);
				_shader->setInt(_textures[i], i);
			}
		}

		glBindVertexArray(_vao);
		if (_indices.empty())
			glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
		else
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

}