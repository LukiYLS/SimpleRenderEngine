#include "Win.h"
#include  <iostream>
#include "RenderSystem.h"
namespace RenderSystem {

	bool Win::createWindow(const int& width, const int& height, const char* name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
														
		GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		return true;
	}
	void Win::starup()
	{
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			RenderSystem::RenderSystem::Inst()->beginRender();
			RenderSystem::RenderSystem::Inst()->render(currentFrame);
			RenderSystem::RenderSystem::Inst()->endRender();
			//deltaTime = currentFrame - lastFrame;
			//lastFrame = currentFrame;

			// input
			// -----
			//processInput(window);



			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

	}
}