#include "HardwareVertexBuffer.h"
#include "HardwareBufferManager.h"
#include <glew\glew.h>

namespace SRE {

	
	HardwareVertexBuffer::HardwareVertexBuffer(size_t vertexSize, size_t numVertices, HardwareBuffer::Usage usage, bool useShadowBuffer)
	: _numVertices(numVertices),
		_vertexSize(vertexSize),
		_usage(usage),
		_useShadowBuffer(useShadowBuffer),
		_lockSize(0),
		_lockStart(0)
	{
		//影子缓冲区的作用是为了在内存当中留一份顶点的备份数据，这样可以读取
		_sizeInBytes = numVertices * vertexSize;
		if (_useShadowBuffer)
		{
			_data = static_cast<unsigned char*>(malloc(_sizeInBytes));
		}
		else
		{
			glGenBuffersARB(1, &_verexBufferID);
			if (!_verexBufferID)
			{
				//产生缓冲区标示符失败
			}
		}
	}

	HardwareVertexBuffer::~HardwareVertexBuffer()
	{
		if (_useShadowBuffer)
		{
			if (_data)
				free(_data);
		}
		glDeleteBuffersARB(1, &_verexBufferID);		
	}
	void* HardwareVertexBuffer::lock(LockOptions options)
	{
		return lock(0, _sizeInBytes, options);
	}
	//why lock,what's mean, because we want read or write 
	void* HardwareVertexBuffer::lock(size_t offset, size_t length, LockOptions options)
	{
		//
		if (isLocked()){
			//printf("Cannot lock this buffer, it is already locked!");
			//
			return NULL;
		}

		void* ret = 0;

		if ((length + offset) > _sizeInBytes)
		{
			//锁定越界
			return ret;
		}
		else if (_useShadowBuffer)
		{
			if (options != HBL_READ_ONLY)
			{

			}
			ret = _data + offset;
			//
		}
		else
		{
			
			HardwareBufferManager* mgr = HardwareBufferManager::getSingletonPtr();
			//对于小内存，先从内存池中申请空间，避免内存碎片
			if (mgr->getMapBufferThreshold() > length)
			{
				
				ret = mgr->allocate((unsigned int)length);
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
			if (!ret)//没有申请成功，则直接向GPU申请
			{
				GLenum access = 0;
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, _verexBufferID);
				if (options == HBL_DISCARD)
				{
					//丢掉原来缓存的内容
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, _sizeInBytes, 0, HardwareBufferManager::getGLUsege(_usage));

				}
				//不允许回读
				if (_usage & HBU_WRITE_ONLY)
					access = GL_WRITE_ONLY_ARB;
				else if (options == HBL_READ_ONLY)
					access = GL_READ_ONLY_ARB;
				else
					access = GL_READ_WRITE_ARB;

				void *buffer = glMapBufferARB(GL_ARRAY_BUFFER_ARB, access);
				if (buffer == 0)
				{
					//出错
					return ret;
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

	void HardwareVertexBuffer::unlock(void)
	{
		if (_isLocked == false)
		{
			//assert()
			return;
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
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, _verexBufferID);
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);//释放缓冲区对象与CPU空间的关系
			}
		}
		_isLocked = false;
		
	}

	void HardwareVertexBuffer::readData(size_t offset, size_t length, void* dest)
	{
		if (_useShadowBuffer)
		{
			memcpy(dest, _data + offset, length);
		}
		else
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, _verexBufferID);
			glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, dest);
		}
	}
	void HardwareVertexBuffer::writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _verexBufferID);

		//全部提交
		if (offset == 0 && length == _sizeInBytes)
		{
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, _sizeInBytes, source, HardwareBufferManager::getGLUsege(_usage));
		}
		else //部分提交
		{
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, source);
		}
	}

	void HardwareVertexBuffer::upload(void)
	{
		if (_useShadowBuffer)
		{
			glGenBuffersARB(1, &_verexBufferID);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, _verexBufferID);
			writeData(_lockStart, _lockSize, _data, _lockStart == 0 && _lockSize == _sizeInBytes);
			free(_data);
			_data = 0;			
			_useShadowBuffer = false;			

		}
	}
}