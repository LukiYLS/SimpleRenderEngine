#ifndef WIN_H
#define WIN_H
#include "Camera.h"
#include <glfw3.h>
#include "data_structure.h"

namespace Basic {//时间怎么加进来啊，glfw的研究一下
	class Win {
	public:
		static Win* Inst();
		bool createWindow(const int& width = 800, const int& height = 600, const char* name = "SimpleRenderEngine v1.0");
		void starup(RenderParams* params);
	public:
		GLFWwindow* window;
		static Win* m_Inst;
	};
}
#endif // !WIN_H
