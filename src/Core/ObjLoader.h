#pragma once
#include "Loader.h"
#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
namespace Core {

	class ObjLoader 
		:public Loader{
	public:
		virtual Mesh* load(const std::string& fileName) {
			Mesh* mesh;

			std::ifstream file(fileName);
			if (!file)
			{
				std::cerr << "Error::ObjLoader, could not open .obj file:"
					<< fileName << " for reading." << std::endl;
				return false;
			}
			std::string line;
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Vector2D> uvs;
			std::vector<Vector3D> normals;
			std::vector<Vector3D> positions;
			while (std::getline(file, line))
			{
				if (line.substr(0, 2) == "vt")     // 顶点纹理坐标数据
				{
					std::istringstream s(line.substr(2));
					Vector2D v;
					s >> v.x; s >> v.y;
					uvs.push_back(v);
				}
				else if (line.substr(0, 2) == "vn") // 顶点法向量数据
				{
					std::istringstream s(line.substr(2));
					Vector3D v;
					s >> v.x; s >> v.y; s >> v.z;
					normals.push_back(v);
				}
				else if (line.substr(0, 1) == "v")  // 顶点位置数据
				{
					std::istringstream s(line.substr(1));
					Vector3D v;
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
							Vector3D pos = positions[posindex - 1];
							Vector3D normal = normals[normalindex - 1];
							vertex = Vertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z);

						}
						else
						{
							ivtn >> posindex >> uvindex >> normalindex;

							Vector3D pos = positions[posindex - 1];
							Vector3D normal = normals[normalindex - 1];
							Vector2D uv = uvs[uvindex - 1];
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
			mesh->setVertices(vertices);
		}
	};
}