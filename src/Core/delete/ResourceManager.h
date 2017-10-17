#pragma once
#include "Resource.h"
#include "ResourceFactory.h"
#include <map>
#include "Loader.h"
namespace Core {

	class ResourceManager {
	public:

		virtual Resource::ptr createResource(const string& name, bool isManual = false, Loader::ptr loader = 0);
		virtual Resource::ptr getResourceByName(const char* name);
		virtual void addResource(const char* name, Resource::ptr resource);
		virtual void remove(const char* name);
		virtual void remove(Resource::ptr resource);
		virtual void removeAll();
		virtual void load();
		virtual void unLoad();

	private:

		typedef std::map<const char*, Resource::ptr> ResourceMap;
		ResourceMap _load_resource_map, _unload_resource_map;
	};
}