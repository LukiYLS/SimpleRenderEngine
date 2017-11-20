#pragma once
#include "HardwareBuffer.h"
#include "PixelBox.h"
#include <glew\glew.h>
namespace SRE {

	class HardwareTextureBuffer : public HardwareBuffer{
	public:
	public:
		typedef std::shared_ptr<HardwareTextureBuffer> ptr;
		HardwareTextureBuffer(GLenum target, GLuint id, GLuint face, GLuint level, HardwareBuffer::Usage usage, bool software_mipmap);
		~HardwareTextureBuffer(){}
	public:

		virtual void*	lock(size_t offset, size_t length, LockOptions options);
		virtual void*	lock(LockOptions options);
		PixelBox::ptr lock(PixelBox::ptr box, HardwareBuffer::LockOptions options);
		virtual void	unlock(void);

		virtual void	readData(size_t offset, size_t length, void* dest){}
		virtual void	writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer = false){}

		//virtual void	copyData(HardwareBuffer& src_buffer, size_t src_offset, size_t dst_offset, size_t length, bool discardWholeBuffer = false);
		//virtual void	copyData(HardwareBuffer& src_buffer);

		virtual size_t	getSizeInBytes(void) const { return _sizeInBytes; }
		virtual Usage	getUsage(void) const { return _usage; }

		virtual bool    isLocked(void) const { return _isLocked; }
		virtual bool	isSystemMemory(void) const{}

		void blitFromMemory(PixelBox::ptr src, PixelBox::ptr dst);
		void blitFromMemory(PixelBox::ptr src);

		void  upload(PixelBox::ptr src, PixelBox::ptr dst);
		void  download(PixelBox::ptr box);
	protected:

		GLenum _target;
		GLenum _faceTarget;
		GLuint _textureID;
		GLuint _face;
		GLuint _level;
		Usage  _usage;
		bool _softwareMipmap;

		size_t _width;
		size_t _height;
		size_t _depth;

		GLenum _internalFormat;
		PixelFormat _format;

		size_t _rowPitch;
		size_t _slicePitch;

		size_t _sizeInBytes;
		PixelBox::ptr _buffer;
		bool _isLocked;

		PixelBox::ptr _current_locked_buffer;
		PixelBox::ptr _current_lock_box;
		HardwareBuffer::LockOptions _current_lock_options;
		//bool _useShadowBuffer;
		//unsigned char* _data;
	};
}