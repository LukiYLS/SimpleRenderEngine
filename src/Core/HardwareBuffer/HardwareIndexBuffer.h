#pragma once
#include "HardwareBuffer.h"

#include<memory>
namespace SRE {

	class HardwareIndexBuffer : public HardwareBuffer
	{
	public:
		enum IndexType {
			IT_16BIT,
			IT_32BIT
		};
		typedef std::shared_ptr<HardwareIndexBuffer> ptr;
		virtual void*	lock(size_t offset, size_t length, LockOptions options);
		virtual void*	lock(LockOptions options);
		virtual void	unlock(void);

		virtual void	readData(size_t offset, size_t length, void* dest);
		virtual void	writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer = false);

		virtual void	copyData(HardwareBuffer& src_buffer, size_t src_offset, size_t dst_offset, size_t length, bool discardWholeBuffer = false);
		virtual void	copyData(HardwareBuffer& src_buffer);

		virtual size_t	getSizeInBytes(void) const;
		virtual Usage	getUsage(void) const;

		virtual bool    isLocked(void) const;
		virtual bool	isSystemMemory(void) const;

		virtual void    upload(void);
	protected:

		GLuint _bufferID;
		IndexType _indexType;
		size_t _numIndexes;
		size_t _indexSize;
		bool _useShadowBuffer;
		unsigned char* _data;
		size_t _sizeInBytes;
		Usage  _usage;
		size_t        _locked_start;
		size_t        _locked_size;
	};
}