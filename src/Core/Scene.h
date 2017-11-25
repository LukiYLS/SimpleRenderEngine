#pragma once
#include "Object.h"
#include "Mesh.h"
#include "Light.h"
#include "Plugin.h"
#include <map>
#include <vector>
#include <string>
#include "Camera.h"
#include "Skybox.h"
using namespace std;
using namespace Utils;
namespace SRE {
	class Plugin;
	class Scene {
	public:
		typedef shared_ptr<Scene> ptr;
	public:
		void setSceneRoot(Object::ptr root) { _root = root; }
		Object::ptr const getSceneRoot()const { return _root; }		
		void setSkybox(Skybox* skybox) {}
		Skybox* getSkybox() { return skybox; }
		void render(Camera* camera);
		void enableShadow() { _enable_shadow = true; }
	protected:
		void projectObject(Object* object);
		void setMaterial();
		void update();
		void setupLights(Shader* shader);
		void clearTemp();
	protected:		
		Skybox* skybox;
		Object::ptr _root;
		std::vector<Mesh*> _render_mesh;
		std::vector<Light*> _lights;
		std::vector<Plugin*> _plugins;	
		bool _enable_shadow;
	};
}

