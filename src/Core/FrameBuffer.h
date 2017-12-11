#pragma once
#include <glew\glew.h>
#include <memory>
namespace SRE {

	class FrameBuffer {
	public:
		typedef std::shared_ptr<FrameBuffer> ptr;
		FrameBuffer() = default;
		FrameBuffer(int width, int height);
		~FrameBuffer() = default;

	public:
		bool createFrameBufferWithTexture(int width, int height);
		void bindForReading(int units);		
		void bingForWriting(bool isFullViewPort = true);
		void saveShadowMap();
		void bindFrameBufferTexture(int units, bool isMipMap = true);
	private:
		int _width, _height;
		unsigned int _fbo;
		unsigned int _texture;

	};
}


