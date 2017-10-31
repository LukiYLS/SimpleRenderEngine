#include <glew\glew.h>
#include "../Math/Vector3D.h"
#include "Camera.h"
#include "Texture.h"
#include "Effect.h"
namespace Core {
	enum TYPE { PARTICLE_TYPE_LAUNCHER };
	struct Particle{
		TYPE type;
		Vector3D pos;
		Vector3D vel;//velocity
		float lifetime;
	};
	class ParticleSystem {//用transfrom feedback实现的粒子还没测试
	public:
		ParticleSystem(int count);
		~ParticleSystem();
	public:
		bool initParitcleSystem(const Vector3D& start);
		void render(int detlaTime);
	private:
		void updateParticles(int detlaTime);
		void renderParticles(Camera* camera);
	private:
		int _count;
		bool _isFrist;
		unsigned int _currVB;
		unsigned int _currTFB;
		
		GLuint _particleBuffer[2];
		GLuint _transfromFeedBack[2];
		Texture::ptr _texture;
		int _time;
	};
}