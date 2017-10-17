#pragma once
#include <glew\glew.h>
#include <memory>
#include <map>
namespace Core {
	class RenderState {//¿¼ÂÇÖÐ
		enum CompareFunc
		{
			NeverDepth = GL_NEVER,
			AlwaysDepth = GL_ALWAYS,
			LessDepth = GL_LESS,
			LessEqualDepth = GL_LEQUAL,
			GreaterEqualDepth = GL_GEQUAL,
			GreaterDepth = GL_GREATER,
			NotEqualDepth = GL_NOTEQUAL
		};
		enum CullFaceMode
		{
			Front = GL_FRONT,
			Back = GL_BACK,
			DoubleSide = GL_FRONT_AND_BACK
		};
		enum StateList {
			DepthTest = GL_DEPTH_TEST,
			Blend = GL_BLEND,
			CullFace = GL_CULL_FACE,
			StencilTest = GL_STENCIL_TEST,
			ScissorTest = GL_SCISSOR_TEST
		};		
	public:		
		typedef std::shared_ptr<RenderState> ptr;
		RenderState();
		void use();
		void enable(StateList state) { _state_map[state] = true; }
		void disable(StateList state) { _state_map[state] = false; }
		void setDepthFunc(CompareFunc func) { depthFunc = func; }
		void setStencilFunc(CompareFunc func) { stencilFunc = func; }
		void setCullFace(CullFaceMode face);
		void setBlendingFunc();
		void init();	
	protected:		
		typedef std::map<StateList, bool> StateMap;
		StateMap _state_map;
		CullFaceMode mode;
		CompareFunc depthFunc;
		CompareFunc stencilFunc;
	};
}

