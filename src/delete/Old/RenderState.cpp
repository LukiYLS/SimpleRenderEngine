#include "RenderState.h"

namespace Core {

	RenderState* RenderState::m_Inst(0);
	RenderState* RenderState::Inst() {
		if (m_Inst)
			m_Inst = new RenderState();
		return m_Inst;
	}

	//void RenderState::setClearColor(glm::vec4 color)
	//{
	//glClearColor(color.r, color.g, color.b,color.a);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}
}