#pragma once
#include "Resource.h"
#include "ResourceFactory.h"
#include <unordered_map>
#include "../Basic/Loader.h"
namespace Core {

	class ResourceManager {
	public:

		virtual Resource::ptr createResource(const string& name, bool isManual = false, Loader::ptr loader = 0);
		virtual Resource::ptr getResourceByName(const char* name) { return _resource_map[name]; }
		virtual void addResource(const char* name, Resource::ptr resource) { _resource_map[name] = resource; }
		virtual void remove(const char* name) { _resource_map.erase(name); }
		virtual void remove(Resource::ptr resource);
		virtual void removeAll() { _resource_map.clear(); }
		virtual void load();
		virtual void unLoad();

	private:

		typedef std::unordered_map<const char*, Resource::ptr> ResourceMap;
		ResourceMap _resource_map;
	};
}