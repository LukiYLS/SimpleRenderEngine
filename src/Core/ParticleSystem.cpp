#include "ParticleSytem.h"

namespace Core {

	ParticleSystem::ParticleSystem(int count)
	{
		_count = count;
	}

	bool ParticleSystem::initParitcleSystem(const glm::vec3& start)
	{
		Particle particles[10000];

		particles[0].type = PARTICLE_TYPE_LAUNCHER;
		particles[0].pos = start;
		particles[0].vel = glm::vec3(0.0f, 0.0001f, 0.0f);
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
		return true;
	}

	void ParticleSystem::updateParticles(int deltaTime)
	{
		//effect set


		glEnable(GL_RASTERIZER_DISCARD);

		glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer[_currVB]);
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _transfromFeedBack[_currTFB]);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	}
}