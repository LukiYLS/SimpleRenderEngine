#include "RenderSystem.h"
#include "RenderState.h"
#include "Scene.h"
#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include <iostream>
namespace Core {
	
	std::vector<mouse_event> mouse_events;
	bool button_down = false;

	bool RenderSystem::createWindow(const int& width, const int& height, const char* name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glViewport(0, 0, width, height);
		window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		//³õÊ¼»¯glew
		glewExperimental = GL_TRUE;
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error: " << error << std::endl;
		}
		GLenum glewinit = glewInit();
		if (glewinit != GLEW_OK) {
			std::cout << "Glew not okay! " << glewinit;
			exit(EXIT_FAILURE);
		}

		glfwSetCursorPosCallback(window, cursorPositionCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetKeyCallback(window, keyCallback);

		//register

		return true;
	}

	void RenderSystem::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
	{
		if (!button_down)return;
		mouse_event event;
		double seconds = glfwGetTime();
		event.mouse_status = M_MOVE;
		event.time = seconds;
		event.xpos = xpos;
		event.ypos = ypos;
		mouse_events.push_back(event);
	}
	void RenderSystem::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		mouse_event event;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		double seconds = glfwGetTime();
		event.mouse_status = MOUSE_NONE;
		event.time = seconds;
		event.xpos = xpos;
		event.ypos = ypos;
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			event.mouse_status = LB_DOWN;
			button_down = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			event.mouse_status = MB_DOWN;
			button_down = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT  && action == GLFW_PRESS)
		{
			event.mouse_status = RB_DOWN;
			button_down = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			event.mouse_status = LB_UP;
			button_down = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		{
			event.mouse_status = MB_UP;
			button_down = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT  && action == GLFW_RELEASE)
		{
			event.mouse_status = RB_UP;
			button_down = false;
		}

		if (event.mouse_status != MOUSE_NONE)
			mouse_events.push_back(event);



	}
	void RenderSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{

		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{

		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{

		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{

		}
	}
	void RenderSystem::processInput()
	{
		unsigned int num_events = mouse_events.size();
		if (mouse_events.size() > 0)
		{
			Event::ptr event = EventManager::Inst()->createEvent();
			mouse_event_vector xx;
			xx._mouse_events = &mouse_events[0];//mouse_events;
			xx._event_num = num_events;
			event->setName("mouse.event");
			event->setValue("mouse.event", xx);
			EventManager::Inst()->sendEvent(event);
		}
		mouse_events.clear();
	}	
	
	void RenderSystem::render(Scene* scene, Camera* camera)
	{
		while (!glfwWindowShouldClose(window))
		{
			processInput();
			float currentFrame = glfwGetTime();
			beforeRender();
			renderImpl(scene, camera);
			afterRender();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}	

	}
	void RenderSystem::beforeRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderSystem::renderImpl(Scene* scene, Camera* camera)
	{
		if (camera == NULL)
		{
			std::cout << "no camera" << std::endl;
			return;
		}		
		//render skybox
		Object* root = scene->getSceneRoot();	
		root->updateMatrixWorld();
		projectObject(root);
		if (scene->enableShadow)
		{
			Plugin* shadow = new ShadowMapPlugin(_lights, _render_mesh);
			shadow->render();
		}
		Skybox* skybox = scene->getSkybox();
		if (!skybox)
			skybox->render();
		else
		{
			//clear color
		}
		//render mesh
		for (auto mesh : _render_mesh)
		{
			Shader* shader = ShaderManager::getSingleton()->getByName(mesh->getShaderName()).get();
			shader->use();
			shader->setMat4("worldMatrix", mesh->getWorldMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
			setupLights(shader);
			mesh->setShaderUniform((Shader::ptr)shader);
			mesh->draw();
		}
		
		for (auto plugin : _plugins)
		{
			plugin->render();
		}
		
	}
	void RenderSystem::afterRender()
	{

	}
	void RenderSystem::projectObject(Object* object)
	{
		if (object->visible = false)return;
		if (!object->asMesh())
		{
			Mesh* mesh = object->asMesh();
			_render_mesh.push_back((Mesh::ptr)mesh);

		}
		else if (!object->asLight())
		{
			Light* light = object->asLight();
			_lights.push_back((Light::ptr)light);
		}
		else if (!object->asPlugin())
		{

		}
		else if (!object->asSprite())
		{

		}
		else if (!object->asBillboard())
		{

		}
		else if (!object->asParticleSystem())
		{

		}
		else
		{

		}
	}
	void RenderSystem::setupLights(Shader* shader)
	{
		shader->use();
		for (auto it = _lights.begin(); it != _lights.end(); it++)
		{
			if ((*it)->getType() == DirectLight)
			{
				shader->setVec3("dirLight.direction", (*it)->getDirection());
				shader->setVec3("dirLight.ambient", (*it)->getAmbient());
				shader->setVec3("dirLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("dirLight.specular", (*it)->getSpecular());
			}
			else if ((*it)->getType() == PointLight)
			{
				shader->setVec3("pointLight.position", (*it)->getPosition());
				shader->setVec3("pointLight.ambient", (*it)->getAmbient());
				shader->setVec3("pointLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("pointLight.specular", (*it)->getSpecular());

				shader->setFloat("pointLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("pointLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("pointLight.quadratic", (*it)->getQuadraticAttenuation());
			}
			else if ((*it)->getType() == SpotLight)
			{
				shader->setVec3("spotLight.position", (*it)->getPosition());
				shader->setVec3("spotLight.direction", (*it)->getDirection());
				shader->setVec3("spotLight.ambient", (*it)->getAmbient());
				shader->setVec3("spotLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("spotLight.specular", (*it)->getSpecular());

				shader->setFloat("spotLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("spotLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("spotLight.quadratic", (*it)->getQuadraticAttenuation());

				shader->setFloat("spotLight.innerCutoff", (*it)->getInnerCutoff());
				shader->setFloat("spotLight.outerCutoff", (*it)->getOuterCutoff());
			}
		}
	}
}