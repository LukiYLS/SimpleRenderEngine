#include "ParticleSytem.h"

namespace Core {

	ParticleSystem::ParticleSystem(int count)
	{
		_count = count;
	}

	bool ParticleSystem::initParitcleSystem(const Vector3D& start)
	{
		Particle particles[10000];

		particles[0].type = PARTICLE_TYPE_LAUNCHER;
		particles[0].pos = start;
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
		return true;
	}

	void ParticleSystem::updateParticles(int deltaTime)
	{
		//effect set

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
		glDisable(GL_RASTERIZER_DISCARD);

		glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer[_currTFB]);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);
		glDrawTransformFeedback(GL_POINTS, _transfromFeedBack[_currTFB]);

		glDisableVertexAttribArray(0);

	}
}