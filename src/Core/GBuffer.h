#pragma once
#include <glew\glew.h>
#include <memory>
namespace Core {

	class GBuffer {

	public:
		enum GBUFFER_TEXTURE_TYPE {
			GBUFFER_TEXTURE_TYPE_POSITION,
			GBUFFER_TEXTURE_TYPE_DIFFUSE,
			GBUFFER_TEXTURE_TYPE_NORMAL,
			GBUFFER_TEXTURE_TYPE_TEXCOORD,
			GBUFFER_NUM_TEXTURES
		};
		GBuffer();
		virtual ~GBuffer();
		virtual bool init(unsigned int width, unsigned int height);
		virtual void bindForWriting();
		virtual void bindForReading();
		void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
	protected:
		GLuint _fbo;
		GLuint _textures[GBUFFER_NUM_TEXTURES];
		GLuint _depthTexture;

	};
}