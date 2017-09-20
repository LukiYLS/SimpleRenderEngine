#pragma once
#include <memory>
namespace Core {

	class ResourceFactory {
	public:
		typedef std::shared_ptr<ResourceFactory> ptr;
	};
}