#ifndef SCENE_H
#define SCENE_H
#include "Mesh.h"
#include "Light.h"
#include "Plugin.h"
#include "Parameter.h"
#include <map>
#include <vector>
#include <string>
#include "Camera\PerspectiveCamera.h"
using namespace std;
namespace Core {
	class Plugin;
	class Scene {
	public:
		typedef shared_ptr<Scene> ptr;
	public:		
		void addRenderMesh(const string& shader_name, const string& mesh_name);
		void removeRenderMesh(const string& name);

		vector<Light::ptr> getLights(const string& name, Parameter::ptr paras);
		vector<Light::ptr> getLights() { return _lights; }
		void removeLight(const string& name);

		vector<Mesh::ptr> getMeshs();
		void addPlugin(Plugin::ptr plugin);
		void removePlugin();

		void update(float time);//
		void pickRender(int width, int height);//…Ë÷√color£¨render to buffer£¨read buffer£¨
		Camera::ptr crateCamera(Parameter::ptr paras);
		Camera::ptr getCamera() { return _camera; }
		void render();
	private:		
		map<string, vector<string>> _shader_mesh;	
		vector<Light::ptr> _lights;
		map<string, Light::ptr> _light_map;
		map<string, Plugin::ptr> _plugin_map;
		Camera::ptr _camera;
	};
}
#endif // !SCENEH

