#pragma once
#include "HardwareBuffer.h"
#include "PixelBox.h"
#include <glew\glew.h>
namespace SRE {
	//ogre针对OGL和D3D分别写了各自的texturebuffer，都继承自pixelbuffer，因为这里只考虑ogl，所以单独写了texturebuffer
	class HardwareTextureBuffer : public HardwareBuffer{
	public:
	public:
		typedef std::shared_ptr<HardwareTextureBuffer> ptr;
		HardwareTextureBuffer(GLenum target, GLuint id, GLuint face, GLuint level, HardwareBuffer::Usage usage, bool software_mipmap);
		~HardwareTextureBuffer(){}
	public:

		virtual void* lock(size_t offset, size_t length, LockOptions options);
		virtual void* lock(LockOptions options);
		//
		PixelBox::ptr lock(PixelBox::ptr box, HardwareBuffer::LockOptions options);
		virtual void unlock(void);

		virtual void readData(size_t offset, size_t length, void* dest){}
		virtual void writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer = false){}

		//virtual void	copyData(HardwareBuffer& src_buffer, size_t src_offset, size_t dst_offset, size_t length, bool discardWholeBuffer = false);
		//virtual void	copyData(HardwareBuffer& src_buffer);

		virtual size_t getSizeInBytes(void) const { return _sizeInBytes; }
		virtual Usage getUsage(void) const { return _usage; }

		virtual bool isLocked(void) const { return _isLocked; }
		virtual bool isUseShadowBuffer(void) const { return false; }
		virtual void upload(void){}
		//上传显卡
		void blitFromMemory(PixelBox::ptr data, PixelBox::ptr dst);
		void blitFromMemory(PixelBox::ptr data);
		void  upload(PixelBox::ptr data, PixelBox::ptr dst);
		void  download(PixelBox::ptr box);
	protected:

		GLenum _target;
		GLenum _faceTarget;
		GLuint _textureID;
		GLuint _face;
		GLuint _level;
		Usage  _usage;
		bool _softwareMipmap;

		unsigned int _width;
		unsigned int _height;
		unsigned int _depth;

		GLenum _internalFormat;
		PixelFormat _format;

		size_t _rowPitch;
		size_t _slicePitch;

		size_t _sizeInBytes;
		PixelBox::ptr _buffer;
		bool _isLocked;

		PixelBox::ptr _currentLockedBuffer;
		PixelBox::ptr _currentLockBox;
		HardwareBuffer::LockOptions _currentLockOptions;
		//bool _useShadowBuffer;
		//unsigned char* _data;
	};
}