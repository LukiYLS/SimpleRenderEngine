#pragma once
#include <memory>
namespace Core {

	class NodeVistor {
	public:
		typedef std::shared_ptr<NodeVistor> ptr;
	};
}