#ifndef EFFECT_H
#define EFFECT_H

namespace Basic {
	class Effect {//��Ч��ô��ư�����������׼����չ��һ�������ר������д��Ч����Ч�����װ��shader
	public:
		Effect(){}
		virtual ~Effect(){}
	public:
		virtual bool init() = 0;
		virtual bool setCamera() = 0;

	};
}
#endif // !EFFECT_H
