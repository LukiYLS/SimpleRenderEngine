#include "ParticleSystem.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f
#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f
namespace SRE {

	ParticleSystem::ParticleSystem(int count)
	{
		_count = count;
		_time = 0.0;
	}
	void ParticleSystem::render(Camera* camera, int deltaTime)
	{
		// update render;
		_time += deltaTime;
		updateParticles(deltaTime);
		renderParticles(camera);

		_currVB = _currTFB;
		_currTFB = (_currTFB + 1) & 0x1;

	}
	void ParticleSystem::init()
	{
		Particle particles[10000];

		particles[0].type = PARTICLE_TYPE_LAUNCHER;
		particles[0].pos = _position;
		particles[0].vel = Vector3D(0.0f, 0.0001f, 0.0f);
		particles[0].lifetime = 0.0f;

		glGenTransformFeedbacks(2, _transfromFeedBack);
		glGenBuffers(2, _particleBuffer);

		for (unsigned int i = 0; i < 2; i++)
		{
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transfromFeedBack[i]);
			glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer[i]);

			glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _particleBuffer[i]);

		}
		//update shader
		_update_shader = std::make_shared<Shader>("../Shader/", "", "");
		_render_shader = std::make_shared<Shader>("", "", "");
		
	}

	void ParticleSystem::updateParticles(int deltaTime)
	{
		//effect set
		_update_shader->use();
		_update_shader->setFloat("deltaTime", deltaTime);
		_update_shader->setFloat("time", _time);
		_update_shader->setFloat("launcherLifeTime", 100.0f);
		_update_shader->setFloat("shellLifetime", 10000.0f);
		_update_shader->setFloat("secondaryShellLifetime", 2000.0f);
		//shader set
		glEnable(GL_RASTERIZER_DISCARD);

		glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer[_currVB]);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transfromFeedBack[_currTFB]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28	);

		glBeginTransformFeedback(GL_POINTS);
		if (_isFrist) {
			glDrawArrays(GL_POINTS, 0, 1);
			_isFrist = false;
		}
		else
			glDrawTransformFeedback(GL_POINTS, _transfromFeedBack[_currVB]);

		glEndTransformFeedback();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);


	}
	void ParticleSystem::renderParticles(Camera* camera)
	{
		//billborad shader
		_render_shader->use();
		_render_shader->setMat4("VP", camera->getProjectionMatrix() * camera->getViewMatrix());
		_render_shader->setVec3("cameraPos", camera->getPosition());
		if (!_texture)
		{
//			_texture->bindTexture(0);
			_render_shader->setInt("texture", 0);
		}
		glDisable(GL_RASTERIZER_DISCARD);

		glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer[_currTFB]);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);
		glDrawTransformFeedback(GL_POINTS, _transfromFeedBack[_currTFB]);

		glDisableVertexAttribArray(0);

	}
}