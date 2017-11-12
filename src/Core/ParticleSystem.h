#include <glew\glew.h>
#include "../Math/Vector3D.h"
#include "Camera.h"
#include "Texture.h"
#include "Effect.h"
#include "Shader.h"
#include "BillboardCollection.h"
using namespace Math;
namespace Core {
	
	struct Particle{
		float type;
		Vector3D pos;
		Vector3D vel;//velocity
		float lifetime;
	};
	class ParticleSystem 
		:public Object,public Effect{//用transfrom feedback实现的粒子还没测试
	public:
		ParticleSystem(int count);
		~ParticleSystem() {};
	public:
		virtual void init();
		virtual void render(Camera* camera, int deltaTime);		
	protected:
		void updateParticles(int detlaTime);
		void renderParticles(Camera* camera);
	private:
		int _count;
		bool _isFrist;
		unsigned int _currVB;
		unsigned int _currTFB;
		
		GLuint _particleBuffer[2];
		GLuint _transfromFeedBack[2];

		Shader::ptr _update_shader;
		Shader::ptr _render_shader;
		Texture::ptr _texture;
		BillboardCollection::ptr _billboard;
		int _time;
	};
}