#pragma once
#include <glew\glew.h>
#include <memory>
namespace SRE {

	class FrameBuffer {

	public:
		enum BufferType {
			DepthBuffer,
			ColorBuffer,
			StencilBuffer
		};
		typedef std::shared_ptr<FrameBuffer> ptr;
		virtual void bindForWriting() = 0;
		virtual void bindForReading() = 0;


	protected:
		unsigned int _width;
		unsigned int _height;
		unsigned int _fbo;
		unsigned int _texture;

	};

	class FrameBuffer2D{
	public:

		FrameBuffer2D(unsigned int width, unsigned int height);
	};
	class FrameBufferCube : public FrameBuffer {

	public:

		FrameBufferCube(unsigned int width, unsigned int height);


	};

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


