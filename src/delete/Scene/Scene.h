#ifndef SCENE_H
#define SCENE_H
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Light.h"
#include "../RenderSystem/Plugin.h"
#include "../Utils/Parameter.h"
#include <map>
#include <vector>
#include <string>
#include "../Camera/Camera.h"
using namespace std;
namespace Core {
	class Plugin;
	class Scene {
	public:
		typedef shared_ptr<Scene> ptr;
	public:		
		void addRenderMesh(const string& shader_name, const string& mesh_name);
		void removeRenderMesh(const string& name);

		Light::ptr createLight(const string& name, Parameter::ptr paras);
		vector<Light::ptr> getLights() { return _lights; }
		void removeLight(const string& name);

		vector<Mesh::ptr> getMeshs();
		void addPlugin(const string& name, Plugin::ptr plugin);
		void removePlugin(const string& name);

		//void update(float time);//
		//void pickRender(int width, int height);//…Ë÷√color£¨render to buffer£¨read buffer£¨
		Camera::ptr crateCamera(Parameter::ptr paras);
		void setCamera(Camera::ptr camera) { _camera = camera; }
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

