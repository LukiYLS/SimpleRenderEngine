#pragma once
#include "Object.h"
#include "Mesh.h"
#include "Light.h"
#include "Plugin.h"
#include "../Utils/Parameter.h"
#include <map>
#include <vector>
#include <string>
#include "Camera.h"
#include "Skybox.h"
using namespace std;
using namespace Utils;
namespace Core {
	class Plugin;
	class Scene {//:Group
	public:
		typedef shared_ptr<Scene> ptr;
	public:		
		//void addRenderMesh(const string& shader_name, const string& mesh_name);
		//void removeRenderMesh(const string& name);
		void setSceneRoot(Object::ptr root) { _root = root; }
		Object* const getSceneRoot()const { return _root.get(); }
		
		void setSkybox(Skybox* skybox) {}
		Skybox* getSkybox() { return skybox; }
		//

		/*Light::ptr createLight(const string& name, Parameter::ptr paras);
		void addLight(Light::ptr light) { _lights.push_back(light); }
		vector<Light::ptr> getLights() { return _lights; }
		void removeLight(const string& name);
		
		void addPlugin(Plugin::ptr plugin);
		void removePlugin();

		void update(float time);//
		void pickRender(int width, int height);//…Ë÷√color£¨render to buffer£¨read buffer£¨
		vector<Mesh::ptr> getMeshs();

		Camera::ptr crateCamera(Parameter::ptr paras);
		Camera::ptr getCamera() { return _camera; }
		void render();
		void updateMatrixWorld();
		void update();*/
	public:
		bool enableShadow;
	private:		
		Skybox* skybox;
		Object::ptr _root;
	};
}

