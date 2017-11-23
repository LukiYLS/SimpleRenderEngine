#include "RenderSystem.h"
#include "RenderState.h"
#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
using namespace Utils;
#include <iostream>
namespace SRE {
	
	RenderSystem::RenderSystem(Scene* scene, Camera* camera)
	{
		_scene = (Scene::ptr)scene;
		_camera = (Camera::ptr)camera;
		//注册鼠标键盘事件，放在哪里？
		CameraControl::ptr cc = make_shared<CameraControl>(camera);
		EventManager::Inst()->registerReceiver("mouse.event", cc);
		EventManager::Inst()->registerReceiver("keyboard.event", cc);
	}
	
	void RenderSystem::render()
	{	
		beforeRender();
		renderImpl();
		afterRender();
	}
	void RenderSystem::beforeRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	void RenderSystem::renderImpl()
	{
		if (_camera == NULL)
		{
			std::cout << "no camera" << std::endl;
			return;
		}		
		_scene->render(_camera.get());
	}

	void RenderSystem::resize(int x, int y, int width, int height)
	{
		ViewPort::ptr vp = _camera->getViewPort();
		vp->setViewport(x, y, width, height);
	}
	void RenderSystem::afterRender()
	{
		//_render_mesh.clear();
		//_lights.clear();

	}		
}