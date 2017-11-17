#include "HardwareVertexBuffer.h"
#include "HardwareBufferManager.h"
#include <glew\glew.h>

namespace SRE {

	void* HardwareVertexBuffer::lock(LockOptions options)
	{
		lock(0, _sizeInBytes, options);
	}
	void* HardwareVertexBuffer::lock(size_t offset, size_t length, LockOptions options)
	{
		if (isLocked())
		{
			//printf("Cannot lock this buffer, it is already locked!");
			//return;
		}

		void* ret = NULL;

		if ((length + offset) > _sizeInBytes)
		{

			ret = _data + offset;
		}
		else
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
		_isLocked = true;
		//_locked_start = offset_;
		//_locked_size = length_;
		return ret;
	}
}