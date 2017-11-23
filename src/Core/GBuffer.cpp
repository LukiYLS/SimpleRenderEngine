#include "GBuffer.h"


namespace SRE {

	GBuffer::GBuffer()
	{
		_fbo = 0;
		_depthTexture = 0;		
	}

	GBuffer::~GBuffer()
	{
		if (_fbo != 0) {
			glDeleteFramebuffers(1, &_fbo);
		}

		if (_textures[0] != 0) {
			glDeleteTextures(GBUFFER_NUM_TEXTURES, _textures);
		}

		if (_depthTexture != 0) {
			glDeleteTextures(1, &_depthTexture);
		}
	}

	bool GBuffer::init(unsigned int width, unsigned int height)
	{
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

		glGenTextures(GBUFFER_NUM_TEXTURES, _textures);
		glGenTextures(1, &_depthTexture);

		for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i], 0);
		}

		glBindTexture(GL_TEXTURE_2D, _depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

		GLenum DrawBuffers[] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3
		};

		glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			printf("FB error, status: 0x%x\n", status);
			return false;
		}
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		return true;

	}

	void GBuffer::bindForWriting()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
	}

	void GBuffer::bindForReading()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
	}


	void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
	}
}