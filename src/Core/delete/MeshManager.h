#pragma once
#include "Mesh.h"
#include "Loader.h"
#include "MeshFactory.h"
#include "../Utils/Parameter.h"
#include <string>
#include <map>
using namespace std;
using namespace Utils;
namespace Core {

	class MeshManager {
	protected:
		static MeshManager* instance;
	public:
		
		static MeshManager* getSingleton();
	public:

		Mesh* createMesh(const string& fileName, Loader::ptr loader, const string& name);
		SubMesh* createSubMesh(const string& fileName, Loader::ptr, const string& name);
		Mesh* createMesh(Parameter::ptr paras, MeshFactory::ptr factory, const string& name);
		void addMesh(const string& name, Mesh::ptr mesh);
		void remove(const string& name);
		void removeAll();
		Mesh::ptr getByName(const string& name);
	private:
	
		typedef map<string, Mesh::ptr> Mesh_Map;
		Mesh_Map _mesh_map;

	};
}