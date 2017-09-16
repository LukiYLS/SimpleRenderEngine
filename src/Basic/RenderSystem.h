#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "data_structure.h"

namespace Core {
	class RenderSystem {//后期扩展成一个组件，专门做渲染的工作
	public:
		static RenderSystem* Inst();
		void beginRender();//渲染之前设置renderstate
		void render(RenderParams* params);
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

