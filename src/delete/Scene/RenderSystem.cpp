#include "RenderSystem.h"
#include "RenderState.h"
#include "Scene.h"
namespace Core {
	RenderSystem* RenderSystem::m_Inst(0);
	RenderSystem* RenderSystem::Inst() {
		if (m_Inst)
			m_Inst = new RenderSystem();
		return m_Inst;
	}

	void RenderSystem::beginRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderSystem::render()
	{
		//Scene::Inst()->render(params);
	}
	void RenderSystem::endRender()
	{

	}
}