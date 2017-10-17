#pragma once
#include <vector>
#include <memory>
#include "../Utils/Event.h"
#include "../Utils/InputDefine.h"
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Mesh.h"
#include "Plugin.h"
#include <glfw3.h>
namespace Core {
	class RenderSystem {
	public:		
		bool createWindow(const int& width = 800, const int& height = 600, const char* name = "SimpleRenderEngine v1.0");
		void render(Scene* scene, Camera* camera);
	protected:
		void beforeRender();//渲染之前设置renderstate		
		void renderImpl(Scene* scene, Camera* camera);
		void afterRender();//一帧结束
		void projectObject(Object* root);
		void setupLights(Shader* shader);
		void processInput();		

		static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);			
	private:		
		GLFWwindow* window;
		std::vector<Mesh::ptr> _render_mesh;
		std::vector<Light::ptr> _lights;
		std::vector<Plugin::ptr> _plugins;

	};
}


