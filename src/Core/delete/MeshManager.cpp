#include "MeshManager.h"

namespace Core {
	
	MeshManager* MeshManager::instance(0);
	MeshManager* MeshManager::getSingleton() {
		if (!instance)
			instance = new MeshManager();
		return instance;
	}
	Mesh* MeshManager::createMesh(const string& fileName, Loader::ptr loader, const string& name)
	{
		if (getByName(name) != NULL)
			remove(name);
		Mesh* mesh = loader->load(fileName);
		_mesh_map.insert(make_pair(name, mesh));
		return mesh;
	}

	void MeshManager::addMesh(const string& name, Mesh::ptr mesh)
	{
		if (_mesh_map.find(name) != _mesh_map.end())
			remove(name);
		_mesh_map.insert(make_pair(name, mesh));
	}

	Mesh::ptr MeshManager::getByName(const string& name)
	{
		auto it = _mesh_map.find(name);
		if (it != _mesh_map.end())
			return it->second;
		return NULL;
	}
	void MeshManager::remove(const string& name)
	{
		auto it = _mesh_map.find(name);
		if (it != _mesh_map.end())
			_mesh_map.erase(name);
	}
	void MeshManager::removeAll ()
	{
		_mesh_map.clear();
	}
}