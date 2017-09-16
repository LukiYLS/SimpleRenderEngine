#ifndef EFFECT_H
#define EFFECT_H

namespace Core {
	class Effect {//特效怎么设计啊！！！后期准备扩展出一个组件，专门用来写特效，特效必须封装好shader
	public:
		Effect(){}
		virtual ~Effect(){}
	public:
		virtual bool init() = 0;
		virtual bool setCamera() = 0;
		virtual void render() = 0;

	};
}
#endif // !EFFECT_H

