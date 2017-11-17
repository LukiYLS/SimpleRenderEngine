#include "HardwareBufferManager.h"
#include <memory>
#define POOL_SIZE 1 * 1024 * 1024
namespace SRE {

	HardwareBufferManager::HardwareBufferManager()
	{
		_buffer_pool = (char*)malloc(sizeof(char) * POOL_SIZE);
	}
}