#pragma once
#include "Camera.h"
namespace Core {
	class Effect {//特效怎么设计啊！！！后期准备扩展出一个组件，专门用来写特效，特效必须封装好shader
	public:
		Effect(){}
		virtual ~Effect(){}
	public:
		virtual void init() = 0;
		virtual void render(Camera* camera, int deltaTime) = 0;

	};
}


