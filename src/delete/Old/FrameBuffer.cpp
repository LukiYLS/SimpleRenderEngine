#include "FrameBuffer.h"
#include "TextureManager.h"
namespace Core {

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
		glReadBuffer(GL_NONE);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//std::cout << "FB error";
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	

		return true;
	}

	void FrameBuffer::bindFrameBuffer(bool isFullViewPort)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		if (isFullViewPort)glViewport(0, 0, _width, _height);
	}

	void FrameBuffer::bindFrameBufferTexture(int units, bool isMipMap)
	{
		glActiveTexture(units);
		glBindTexture(GL_TEXTURE_2D, _texture);
		if(isMipMap)glGenerateMipmap(GL_TEXTURE_2D);
	}
}