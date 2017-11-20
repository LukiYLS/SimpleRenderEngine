#include "HardwareIndexBuffer.h"
#include "HardwareBufferManager.h"
#include <glew\glew.h>
namespace SRE {
	HardwareIndexBuffer::HardwareIndexBuffer(size_t index_size, size_t num_indices,
		HardwareBuffer::Usage usage, bool use_shadow_buffer)
		:_numIndexes(num_indices),
		_indexSize(index_size),
		_usage(usage),
		_useShadowBuffer(use_shadow_buffer),		
		_locked_start(0),
		_locked_size(0)
	{
		_sizeInBytes = num_indices * index_size;
		if (use_shadow_buffer)
		{
			_data = static_cast<unsigned char*>(malloc(_indexSize));
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
	HardwareIndexBuffer::~HardwareIndexBuffer()
	{
		if (_useShadowBuffer)
		{
			if (_data)
				free(_data);
		}
	
		{
			glDeleteBuffersARB(1, &_bufferID);
			
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
	void* HardwareIndexBuffer::lock(unsigned int offset, unsigned int length, LockOptions options)
	{
		if (isLocked())
		{
			//不能再次锁定,报错
		}
		void *ret_ptr = 0;
		if ((length + offset) > _sizeInBytes)
		{
			//锁定越界
		}
		else if (_useShadowBuffer)
		{
			//if (options_ != HBL_READ_ONLY)
			//	_memory_need_update_to_video_card = true;
			ret_ptr = _data + offset;
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
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _bufferID);
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
		_is_locked = true;
		_locked_start = offset_;
		_locked_size = length_;
		return ret_ptr;
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
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _bufferID);
				glUnmapBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB);
			}
		}
		_isLocked = false;
	}

	void HardwareIndexBuffer::upload(void)
	{
		if (_useShadowBuffer)
		{
			glGenBuffersARB(1, &_bufferID);

			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _bufferID);
			writeData(_locked_start, _locked_size, _data, _locked_start == 0 && _locked_size == _sizeInBytes);
			free(_data);
			_data = 0;
			//_memory_need_update_to_video_card = false;
			_useShadowBuffer = false;
			//_need_to_delete_from_video_card = true;

		}
	}

	/**
	* 回读数据
	**/
	void HardwareIndexBuffer::readData(gw_uint32 offset_, gw_uint32 length_, void* dest_)
	{

		if (_useShadowBuffer)
		{
			memcpy(dest, _data + offset, length);
		}
		else
		{
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _bufferID);
			glGetBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, offset, length, dest);
		}
	}

	/**
	* write
	**/
	void HardwareIndexBuffer::writeData(gw_uint32 offset_, gw_uint32 length_, const void* source_,
		bool discard_whole_buffer_)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, _bufferID);

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

	void hardware_vertex_buffer_impl::copy_data(hardware_buffer& src_buffer_, gw_uint32 src_offset_,
		gw_uint32 dst_offset_, gw_uint32 length_, bool discard_whole_buffer_)
	{

	}
	void hardware_vertex_buffer_impl::copy_data(hardware_buffer& src_buffer_)
	{

	}

}