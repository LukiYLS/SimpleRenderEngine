#pragma once
#include "../Core/Object.h"
#include "rapidjson/document.h"
namespace SRE {

	/*
	请求Cesium发布的3D tilset模型服务
	example url = "https://beta.cesium.com/api/assets/1458/"
	请求过程： 1.先请求tileset.json文件得到节点树信息
			   2.根据json文件根节点以及子节点信息得到各节点请求url
	*/
	class Request3DTileSet {

	public:

		Object* requestFromUrl(const char* url);

	};
}