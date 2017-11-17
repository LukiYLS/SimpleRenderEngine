#pragma once

#include "HardwareBuffer.h"
namespace SRE {

	class HardwareBufferManager {
	public:
		HardwareBufferManager();
	public:
		static HardwareBufferManager& getSingleton(void);
		static HardwareBufferManager* getSingletonPtr(void);

		static GLenum getGLUsege(unsigned int usage);
	protected:

		static HardwareBufferManager* _singleton;
		char* _buffer_pool;
	};
}