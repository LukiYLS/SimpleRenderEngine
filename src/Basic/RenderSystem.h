#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "Camera.h"
namespace Basic {
	class RenderSystem {
	public:
		static RenderSystem* Inst();
		void beginRender();//渲染之前设置renderstate
		void render(Camera::ptr camera, float time);
		void endRender();//一帧结束
	protected:
		RenderSystem() {}
		RenderSystem(const RenderSystem&);
		RenderSystem& operator = (const RenderSystem&);
	private:
		static RenderSystem* m_Inst;
	};
}
#endif // !RENDERSYSTEM_H

