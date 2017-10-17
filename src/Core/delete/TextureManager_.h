#pragma once
#include "Texture.h"
#include "ResourceManager.h"
namespace Core {

	class TextureManager
		:public ResourceManager {

	public:
		Texture::ptr getByName(const string& name);


	};
}