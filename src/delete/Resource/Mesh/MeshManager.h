#pragma once
#include "Mesh.h"
#include "Loader.h"
#include "MeshFactory.h"
#include "../../Utils/Parameter.h"
#include <string>
#include <map>
using namespace std;
namespace Core {

	class MeshManager {
	protected:
		static MeshManager* instance;
	public:
		
		static MeshManager* getSingleton();
	public:

		Mesh::ptr createMesh(const string& fileName, Loader::ptr loader, const string& name);
		Mesh::ptr createMesh(Parameter::ptr paras, MeshFactory::ptr factory, const string& name);
		void remove(const string& name);
		void removeAll();
		Mesh::ptr getByName(const string& name);
	private:
	
		typedef map<string, Mesh::ptr> Mesh_Map;
		Mesh_Map _mesh_map;

	};
}