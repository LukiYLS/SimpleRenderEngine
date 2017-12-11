#include "FrameBuffer.h"
#include "TextureManager.h"
namespace SRE {

	FrameBuffer::FrameBuffer(int width, int height)
	{
		glGenFramebuffers(1, &_fbo);
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _texture, 0);

		_width = width;
		_height = height;
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool FrameBuffer::createFrameBufferWithTexture(int width, int height)
	{
		if (_fbo)return false;
		glGenFramebuffers(1, &_fbo);		
		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);	
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

		_width = width;
		_height = height;
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//std::cout << "FB error";
			return false;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	void FrameBuffer::bingForWriting(bool isFullViewPort)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);	
		if (isFullViewPort)glViewport(0, 0, _width, _height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void FrameBuffer::bindForReading(int units)
	{
		glActiveTexture(GL_TEXTURE0 + units);
		glBindTexture(GL_TEXTURE_2D, _texture);
	}
	void FrameBuffer::saveShadowMap()
	{
		TextureManager::Inst()->addTexture("shadowMap", _texture);
	}
	void FrameBuffer::bindFrameBufferTexture(int units, bool isMipMap)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(units);
		glBindTexture(GL_TEXTURE_2D, _texture);
		if(isMipMap)glGenerateMipmap(GL_TEXTURE_2D);		
	}
}