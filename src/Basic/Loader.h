#pragma once
#include "../Texture/Resource.h"
namespace Core {
	/*
	resource loader basic class
	*/
	class Loader {
	public:
		typedef std::shared_ptr<Loader> ptr;
	public:
		virtual Resource::ptr load(const string& name) = 0;
	};
}