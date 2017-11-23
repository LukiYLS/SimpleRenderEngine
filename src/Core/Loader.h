#pragma once
#include "Mesh.h"
#include <string>
namespace SRE {
	/*
	resource loader basic class
	*/
	class Loader {
	public:
		typedef std::shared_ptr<Loader> ptr;
	public:
		virtual Mesh* load(const std::string& name) = 0;
	};
}