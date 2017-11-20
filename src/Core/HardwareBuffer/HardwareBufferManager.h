#pragma once
#include <glew\glew.h>
#include "HardwareBuffer.h"
#include "HardwareIndexBuffer.h"
#include "HardwareVertexBuffer.h"
#include "VertexDeclaration.h"
#include "VertexBufferBinding.h"
namespace SRE {


	#define DefaultMapBufferThreshold (1024 * 32)//
	//32位结构（位域）
	struct BufferAlloc
	{
		/// Size in bytes
		unsigned int size : 31;
		/// Free? (pack with size)
		unsigned int free : 1;
	};

	#define POOLSIZE 1 * 1024 * 1024
	#define ALIGNMENT 32

	class HardwareBufferManager {
	public:
		HardwareBufferManager();
		~HardwareBufferManager();
	public:
		static HardwareBufferManager& getSingleton(void);
		static HardwareBufferManager* getSingletonPtr(void);
		HardwareVertexBuffer::ptr
			createVertexBuffer(unsigned int vertex_size_, unsigned int num_verts_, HardwareBuffer::Usage usage,
				bool use_shadow_buffer = false);

		//HardwareIndexBuffer::ptr
		//	createIndexBuffer(HardwareIndexBuffer::index_type itype_, unsigned int num_indexes_,
		//		hardware_buffer::bufferusage usage_, bool use_shadow_buffer = false);

		VertexDeclaration::ptr createVertexDeclaration(void);

		void destroyVertexDeclaration(VertexDeclaration::ptr decl);

		VertexBufferBinding::ptr createVertexBufferBinding(void);

		void destroyVertexBufferBinding(VertexBufferBinding::ptr binding);
	
	public:
		size_t getMapBufferThreshold() { return DefaultMapBufferThreshold; }
		void* allocate(size_t size);
		void deallocate(void* ptr);
		static GLenum getGLUsege(unsigned int usage);
		static GLenum getGLType(unsigned int type);
	protected:
		static HardwareBufferManager* _singleton;
		char* _bufferpool;
	};
}