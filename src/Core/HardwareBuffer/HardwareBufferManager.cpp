#include "HardwareBufferManager.h"
#include "VertexElement.h"
#include <memory>

namespace SRE {


	HardwareBufferManager* HardwareBufferManager::_singleton(0);

	HardwareBufferManager* HardwareBufferManager::getSingletonPtr()
	{
		if (!_singleton)
			_singleton = new HardwareBufferManager();

		return _singleton;
	}


	HardwareBufferManager::~HardwareBufferManager()
	{
		free(_buffer_pool);
	}
	HardwareBufferManager::HardwareBufferManager()
	{
		_buffer_pool = (char*)malloc(sizeof(char) * POOLSIZE);

		BufferAlloc *ptr = (BufferAlloc*)_buffer_pool;
		ptr->size = POOLSIZE - sizeof(BufferAlloc);
		ptr->free = 1;
	}

	void* HardwareBufferManager::allocate(size_t size)
	{
		//将分配的字节对齐
		if (size % 4 != 0)
			size += 4 - (size % 4);

		size_t buffer_pos = 0;

		while (buffer_pos < POOLSIZE)
		{
			BufferAlloc* next = (BufferAlloc*)(_buffer_pool + buffer_pos);

			if (next->free && next->size >= size)
			{
				if (next->size > size + sizeof(BufferAlloc))
				{
					unsigned int offset = (unsigned int)sizeof(BufferAlloc) + size;
					BufferAlloc* split_alloc = (BufferAlloc*)(_buffer_pool + buffer_pos + offset);
					split_alloc->free = 1;
					split_alloc->size = next->size - sizeof(BufferAlloc) - size;
					next->size = size;
				}
				next->free = 0;
				return ++next;
			}

			buffer_pos += sizeof(BufferAlloc) + next->size;
		}

		return 0;
	}

	void HardwareBufferManager::deallocate(void* ptr)	
	{
		unsigned int buffer_pos = 0;
		BufferAlloc *last = 0;

		while (buffer_pos < POOLSIZE)
		{
			BufferAlloc* current = (BufferAlloc*)(_buffer_pool + buffer_pos);
			if ((_buffer_pool + buffer_pos + sizeof(BufferAlloc)) == ptr)
			{
				current->free = 1;
				if (last&& last->free)
				{
					buffer_pos -= (last->size + (unsigned int)sizeof(BufferAlloc));
					last->size += current->size + sizeof(BufferAlloc);
					current = last;
				}
				unsigned int offset = buffer_pos + current->size + sizeof(BufferAlloc);
				if (offset < POOLSIZE)
				{
					BufferAlloc *next = (BufferAlloc*)(_buffer_pool + offset);
					if (next->free)
						current->size += next->size + sizeof(BufferAlloc);
				}

				return;
			}
			buffer_pos += (unsigned int)sizeof(BufferAlloc) + current->size;
			last = current;
		}
	}

	GLenum HardwareBufferManager::getGLUsege(unsigned int usage)
	{
		switch (usage)
		{
		case HardwareBuffer::HBU_STATIC:
		case HardwareBuffer::HBU_STATIC_WRITE_ONLY:
			return GL_STATIC_DRAW_ARB;
		case HardwareBuffer::HBU_DYNAMIC:
		case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY:
			return GL_DYNAMIC_DRAW_ARB;
		case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
			return GL_STREAM_DRAW_ARB;
		default:
			return GL_DYNAMIC_DRAW_ARB;
		};
	}

	GLenum HardwareBufferManager::getGLType(unsigned int type)
	{
		switch (type)
		{
		case VET_FLOAT1:
		case VET_FLOAT2:
		case VET_FLOAT3:
		case VET_FLOAT4:
			return GL_FLOAT;
		case VET_SHORT1:
		case VET_SHORT2:
		case VET_SHORT3:
		case VET_SHORT4:
			return GL_SHORT;
		case VET_COLOUR:
		case VET_COLOUR_ABGR:
		case VET_COLOUR_ARGB:
		case VET_UBYTE4:
			return GL_UNSIGNED_BYTE;
		default:
			return 0;
		};
	}


}