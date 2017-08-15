#include "FrameBuffer.h"

namespace RenderSystem {

	bool FrameBuffer::createFrameBufferWithTexture(int width, int height)
	{
		if (_fbo)return false;
		glGenFramebuffers(1, &_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

		_width = width;
		_height = height;

		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void FrameBuffer::bindFrameBuffer(bool isFullViewPort)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		if (isFullViewPort)glViewport(0, 0, _width, _height);
	}
}