#ifndef WIN_H
#define WIN_H
#include <glfw3.h>
#include <vector>

#include "../Utils/Event.h"
#include "../Utils/InputDefine.h"
namespace Core {
	
	
	class Win {
	public:
		static Win* Inst();
		bool createWindow(const int& width = 800, const int& height = 600, const char* name = "SimpleRenderEngine v1.0");	
		void starup();				
		void processInput();
		//void framebuffer_size_callback(GLFWwindow* window, int width, int height);	
		//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);	
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		//glfwSetCursorEnterCallback(window, cursorEnterCallback);
	public:
		static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		
	public:
		GLFWwindow* window;
		static Win* m_Inst;
	};
}
#endif // !WIN_H
