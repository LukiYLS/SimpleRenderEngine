#ifndef EFFECT_H
#define EFFECT_H

namespace Basic {
	class Effect {//特效怎么设计啊！！！
	public:
		Effect(){}
		virtual ~Effect(){}
	public:
		virtual bool init() = 0;
		virtual bool setCamera() = 0;

	};
}
#endif // !EFFECT_H

