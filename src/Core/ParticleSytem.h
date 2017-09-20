#include <glew\glew.h>
#include <glm\glm.hpp>
#include "Texture.h"
#include "Effect.h"
namespace Core {
	enum TYPE { PARTICLE_TYPE_LAUNCHER };
	struct Particle{
		TYPE type;
		glm::vec3 pos;
		glm::vec3 vel;//velocity
		float lifetime;
	};
	class ParticleSystem {//用transfrom feedback实现的粒子还没测试
	public:
		ParticleSystem(int count);
		~ParticleSystem();
	public:
		bool initParitcleSystem(const glm::vec3& start);
		void render(int detlaTime);
	private:
		void updateParticles(int detlaTime);
		void renderParticles();
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