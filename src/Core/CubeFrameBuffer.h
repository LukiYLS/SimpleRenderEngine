#pragma once


namespace SRE {

	class FrameBuffer {

	public:
		enum BufferType {
			DepthBuffer,
			ColorBuffer,
			StencilBuffer
		};

		virtual void bindForWriting() = 0;
		virtual void bindForReading() = 0;

	protected:
		unsigned int _width;
		unsigned int _height;
		unsigned int _fbo;
		unsigned int _texture;

	};

	class FrameBuffer2D : public FrameBuffer{
	public:

		FrameBuffer2D(unsigned int width, unsigned int height);
	};
	class FrameBufferCube : public FrameBuffer{

	public:

		FrameBufferCube(unsigned int width, unsigned int height);


	};
}