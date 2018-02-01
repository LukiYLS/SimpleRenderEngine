#pragma once


namespace SRE {
	/*
	请求I3S标准服务发布的数据
	exameple url : https://tiles.arcgis.com/tiles/FQD0rKU8X5sAQfh8/arcgis/rest/services/VRICON_Yosemite_Sample_Integrated_Mesh_scene_layer/SceneServer/layers/0/
	请求过程 ： 1.想通过url请求得到关于数据的描述信息，格式为json字符串
				2.从json字符串中提取出rootNode的路径，url与路径组合得到根节点url地址，请求根节点信息
				3.得到根节点信息包括json、Geometry、Texture数据，并从json数据中得到子节点路径信息，用相同的方法得到子节点路径，请求子节点信息
				4.将每次得到的新的请求地址放入栈中，依次从上往下请求，直至栈中为空
	*/
	class RequestI3SData {

	public:


	};
}