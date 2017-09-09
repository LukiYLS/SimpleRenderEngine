#ifndef WIN_H
#define WIN_H
#include "Camera.h"
#include <glfw3.h>
#include "data_structure.h"
#include "Camera\PerspectiveCamera.h"
namespace Basic {
	class Win {
	public:
		static Win* Inst();
		bool createWindow(const int& width = 800, const int& height = 600, const char* name = "SimpleRenderEngine v1.0");
		void starup(RenderParams* params);

		void onMouseButton(int button, int action);

		//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		//void processInput(GLFWwindow *window);

		//mouse
		static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
		void cursorEnterCallback(GLFWwindow *window, int entered);
		void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
		/*glfwSetCursorPosCallback(window, cursorPositionCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwSetCursorEnterCallback(window, cursorEnterCallback);

		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

		glfwSetScrollCallback(window, scrollCallback);*/
	public:
		GLFWwindow* window;
		static Win* m_Inst;
	};
}
#endif // !WIN_H
