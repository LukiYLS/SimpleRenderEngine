#ifndef RENDERSTATE_H
#define RENDERSTATE_H
#include <glew\glew.h>
#include <glm\glm.hpp>
namespace Core {
	class RenderState {//¿¼ÂÇÖÐ
		enum DepthFunc
		{
			NeverDepth,
			AlwaysDepth,
			LessDepth,
			LessEqualDepth,
			GreaterEqualDepth,
			GreaterDepth,
			NotEqualDepth
		};
		enum CullFace
		{
			Front,
			Back,
			DoubleSide
		};
	public:
		static RenderState* Inst();
		//void setClearColor(glm::vec4 color);
		void setDepthTest(bool depthTest);
		void setDepthFunc(DepthFunc func);
		void setStencilTest(bool stencilTest);
		void setStencilFunc();
		//cull face
		void setCullFace(CullFace face);
		void setBlending();
		void init();
	protected:
		RenderState() {};
		RenderState(const RenderState&) {};
		RenderState& operator = (const RenderState&) {};
		//depth test
		//stencil test
	private:
		static RenderState* m_Inst;
	};
}
#endif // RENDERSTATE_H

