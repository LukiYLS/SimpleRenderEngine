#ifndef WIN_H
#define WIN_H
#include <glfw3.h>
namespace RenderSystem {
	class Win {
	public:
		static Win* Inst();
		void createWindow(const int& width = 800, const int& height = 600, const char* name = "SimpleRenderEngine v1.0");
		void starup();

};
}
#endif // !WIN_H
