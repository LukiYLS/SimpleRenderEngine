#include "HardwareVertexBuffer.h"
#include "HardwareBufferManager.h"
#include <glew\glew.h>

namespace SRE {


	HardwareVertexBuffer::HardwareVertexBuffer(size_t vertex_size, size_t num_vertices, HardwareBuffer::Usage usage, bool use_shadow_buffer)
	: _numVertices(num_vertices),
		_vertexSize(vertex_size),
		_usage(usage),
		_useShadowBuffer(use_shadow_buffer),
		//_memory_need_update_to_video_card(false),
		//_need_to_delete_from_video_card(false),
		_locked_start(0),
		_locked_size(0) 
	{
		//影子缓冲区的作用是为了在内存当中留一份顶点的备份数据，这样可以读取
		if (_useShadowBuffer)
		{
			_data = static_cast<unsigned char*>(malloc(_sizeInBytes));
		}
		else
		{
			glGenBuffersARB(1, &_bufferID);
			if (!_bufferID)
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
		//if (_need_to_delete_from_video_card)
		{
		//	glDeleteBuffersARB(1, &_bufferid);
		//	_need_to_delete_from_video_card = false;
		}
	}
	void* HardwareVertexBuffer::lock(LockOptions options)
	{
		lock(0, _sizeInBytes, options);
	}
	//why lock,what's mean, because we want read or write 
	void* HardwareVertexBuffer::lock(size_t offset, size_t length, LockOptions options)
	{
		if (isLocked())		{
			//printf("Cannot lock this buffer, it is already locked!");
			//
			return;
		}

		void* ret = 0;

		if ((length + offset) > _sizeInBytes)
		{
			//锁定越界
			return;
		}
		else if (_useShadowBuffer)
		{
			if (options != HBL_READ_ONLY)
				//_memory_need_update_to_video_card = true;
			ret = _data + offset;
			//
		}
		else
		{
			HardwareBufferManager* hdbm = HardwareBufferManager::getSingletonPtr();
			if (hdbm->getMapBufferThreshold() > length)
			{
				ret = hdbm->allocate((unsigned int)length);
				if (ret)
				{
					_locked_to_scratch = true;
					_scratch_offset = offset;
					_scratch_size = length;
					_scratch_ptr = ret;
					_scratch_upload_on_unlock = (options != HBL_READ_ONLY);

					//需要回读数据
					if (options != HBL_DISCARD)
						readData(offset, length, ret);
				}
			}
			if (!ret)
			{
				GLenum access = 0;
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);
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
				}

				ret = static_cast<void*>(static_cast<unsigned char *>(buffer) + offset);
				//_locked_to_scratch = false;
			}
			
		}
		_isLocked = true;
		_locked_start = offset;
		_locked_size = length;
		return ret;
	}

	void HardwareVertexBuffer::unlock(void)
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
			if (_locked_to_scratch)
			{
				if (_scratch_upload_on_unlock)
					writeData(_scratch_offset, _scratch_size, _scratch_ptr, _scratch_offset == 0 && _scratch_size == getSizeInBytes());
				//释放缓存
				//hardware_buffermanagerimpl::ptr mgrimpl = _mgr.to_shared_ptr<hardware_buffermanagerimpl>();
				HardwareBufferManager* hdbm = HardwareBufferManager::getSingletonPtr();
				hdbm->deallocate(_scratch_ptr);
				_locked_to_scratch = false;
			}
			else
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);
				glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
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
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);
			glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, dest);
		}
	}
	void HardwareVertexBuffer::writeData(size_t offset, size_t length, const void* source, bool discardWholeBuffer = false)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);

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
			glGenBuffersARB(1, &_bufferID);

			glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);
			writeData(_locked_start, _locked_size, _data, _locked_start == 0 && _locked_size == _sizeInBytes);
			free(_data);
			_data = 0;
			//_memory_need_update_to_video_card = false;
			_useShadowBuffer = false;
			//_need_to_delete_from_video_card = true;

		}
	}
}