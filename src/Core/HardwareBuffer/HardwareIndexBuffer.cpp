#include "HardwareIndexBuffer.h"
#include "HardwareBufferManager.h"
#include <glew\glew.h>
namespace SRE {
	HardwareIndexBuffer::HardwareIndexBuffer(IndexType index_type, size_t numIndexes, HardwareBuffer::Usage usage, bool useShadowBuffer)
		:_numIndexes(numIndexes),
		_indexType(index_type),
		_usage(usage),
		_useShadowBuffer(useShadowBuffer),
		_lockStart(0),
		_lockSize(0)
	{
		switch (index_type)
		{
		case IT_16BIT:
			_indexSize = sizeof(unsigned short);
			break;
		case IT_32BIT:
			_indexSize = sizeof(unsigned int);
			break;
		}
		_sizeInBytes = _numIndexes * _indexSize;
		if (_useShadowBuffer)
		{
			_data = static_cast<unsigned char*>(malloc(_sizeInBytes));
		}
		else
		{
			glGenBuffersARB(1, &_indexBufferID);
			if (!_indexBufferID)
			{
				//产生缓冲区标示符失败
			}
		}
	
	}
	HardwareIndexBuffer::~HardwareIndexBuffer()
	{
		if (_useShadowBuffer)
		{
			if (_data)
				free(_data);
		}
	
		{
			glDeleteBuffersARB(1, &_indexBufferID);
			
		}
	}
	/**
	* lock all
	**/
	void* HardwareIndexBuffer::lock(LockOptions options)
	{
		return this->lock(0, _sizeInBytes, options);
	}

	/**
	* lock part
	**/
	void* HardwareIndexBuffer::lock(size_t offset, size_t length, LockOptions options)
	{
		if (isLocked())
		{
			//不能再次锁定,报错
		}
		void *ret = 0;
		if ((length + offset) > _sizeInBytes)
		{
			//锁定越界
		}
		else if (_useShadowBuffer)
		{
			//if (options_ != HBL_READ_ONLY)
			//	_memory_need_update_to_video_card = true;
			ret = _data + offset;
		}
		else
		{
			HardwareBufferManager* hdbm = HardwareBufferManager::getSingletonPtr();
			if (hdbm->getMapBufferThreshold() > length)
			{
				ret = hdbm->allocate((unsigned int)length);
				if (ret)
				{
					//保存信息
					_lockFromPool = true;
					_scrachOffset = offset;
					_scrachSize = length;
					_scrachPtr = ret;
					//如果不是只读的buffer，需要更新GPU
					_scratchNeedUpload = (options != HBL_READ_ONLY);

					//需要回读数据，读取缓冲器的数据
					if (options != HBL_DISCARD)
						readData(offset, length, ret);
				}
			}
			if (!ret)
			{
				GLenum access = 0;
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBufferID);
				if (options == HBL_DISCARD)
				{
					//丢掉原来缓存的内容
					glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _sizeInBytes, 0, HardwareBufferManager::getGLUsege(_usage));

				}
				//不允许回读
				if (_usage & HBU_WRITE_ONLY)
					access = GL_WRITE_ONLY_ARB;
				else if (options == HBL_READ_ONLY)
					access = GL_READ_ONLY_ARB;
				else
					access = GL_READ_WRITE_ARB;

				void *buffer = glMapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, access);
				if (buffer == 0)
				{
					//出错
				}

				ret = static_cast<void*>(static_cast<unsigned char *>(buffer) + offset);
				//_locked_to_scratch = false;
			}

		
		}
		_isLocked = true;
		_lockStart = offset;
		_lockSize = length;
		return ret;
	}


	void HardwareIndexBuffer::unlock(void)
	{
		if (_isLocked == false)
		{
			//assert()
		}
		if (_useShadowBuffer && _isLocked)
		{
			_isLocked = false;
			return;
		}
		else
		{
			//如果是从内存缓冲区分配的
			if (_lockFromPool)
			{
				if (_scratchNeedUpload)
					writeData(_scrachOffset, _scrachSize, _scrachPtr, _scrachOffset == 0 && _scrachSize == getSizeInBytes());
				//释放缓存				
				HardwareBufferManager* hdbm = HardwareBufferManager::getSingletonPtr();
				hdbm->deallocate(_scrachPtr);
				//_locked_to_scratch = false;
			}
			else
			{
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBufferID);
				glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);//释放缓冲区对象与CPU空间的关系
			}		
		}
		_isLocked = false;
	}

	void HardwareIndexBuffer::upload(void)
	{
		if (_useShadowBuffer)
		{
			glGenBuffersARB(1, &_indexBufferID);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, _indexBufferID);
			writeData(_lockStart, _lockSize, _data, _lockStart == 0 && _lockSize == _sizeInBytes);
			free(_data);
			_data = 0;
			_useShadowBuffer = false;

		}
	}

	/**
	* 回读数据
	**/
	void HardwareIndexBuffer::readData(size_t offset, size_t length, void* dest)
	{
		if (_useShadowBuffer)
		{
			memcpy(dest, _data + offset, length);
		}
		else
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBufferID);
			glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, dest);
		}
	}	
	/**
	* write
	**/
	void HardwareIndexBuffer::writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer)
	{
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indexBufferID);

		//全部提交
		if (offset == 0 && length == _sizeInBytes)
		{
			glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _sizeInBytes, source, HardwareBufferManager::getGLUsege(_usage));
		}
		else //部分提交
		{
			glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, source);
		}
	}	

}