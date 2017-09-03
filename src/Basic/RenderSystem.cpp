#include "RenderSystem.h"
#include "RenderState.h"
#include "Scene.h"
namespace Basic {
	RenderSystem* RenderSystem::m_Inst(0);
	RenderSystem* RenderSystem::Inst() {
		if (m_Inst)
			m_Inst = new RenderSystem();
		return m_Inst;
	}

	void RenderSystem::beginRender()
	{
		//RenderState::Inst()->setc
	}
	void RenderSystem::render(Camera::ptr camera, float currentTime)
	{
		Scene::Inst()->render(camera, currentTime);
	}
	void RenderSystem::endRender()
	{

	}
}