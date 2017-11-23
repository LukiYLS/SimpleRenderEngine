#pragma once
#include "HardwareBuffer.h"
#include <glew\glew.h>
#include<memory>
namespace SRE {

	//index buffer与 vertex buffer基本类似，考虑实现放到HardwareBuffer？
	class HardwareIndexBuffer : public HardwareBuffer
	{
	public:
		enum IndexType {
			IT_16BIT,
			IT_32BIT
		};
		typedef std::shared_ptr<HardwareIndexBuffer> ptr;
		HardwareIndexBuffer(IndexType index_type, size_t num_indices, HardwareBuffer::Usage usage, bool use_shadow_buffer = false);
		~HardwareIndexBuffer();
	public:
		
		typedef std::shared_ptr<HardwareIndexBuffer> ptr;
		virtual void*	lock(size_t offset, size_t length, LockOptions options);
		virtual void*	lock(LockOptions options);
		virtual void	unlock(void);

		virtual void	readData(size_t offset, size_t length, void* dest);
		virtual void	writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer = false);

		//virtual void	copyData(HardwareBuffer& src_buffer, size_t src_offset, size_t dst_offset, size_t length, bool discardWholeBuffer = false);
		//virtual void	copyData(HardwareBuffer& src_buffer);

		virtual size_t	getSizeInBytes(void) const { return _sizeInBytes; }
		virtual Usage	getUsage(void) const { return _usage; }

		virtual bool    isLocked(void) const { return _isLocked; }
		virtual bool	isUseShadowBuffer(void) const { return _useShadowBuffer; }
		virtual void    upload(void);
		GLuint  getBufferID() { return _indexBufferID; }
		size_t  getIndexSize() { return _indexSize; }
		IndexType getIndexType() { return _indexType; }
	protected:

		GLuint _indexBufferID;
		IndexType _indexType;
		size_t _numIndexes;
		size_t _indexSize;
		bool _useShadowBuffer;
		unsigned char* _data;
		size_t _sizeInBytes;
		Usage  _usage;
		size_t _lockSize;
		size_t _lockStart;
		bool _isLocked;
		//从内存池取数据
		bool   _lockFromPool;
		size_t _scrachSize;
		size_t _scrachOffset;
		void * _scrachPtr;
		bool _scratchNeedUpload;
	};
}