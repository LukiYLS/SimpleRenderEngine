


#include "../Core/Mesh.h"
#include "../Core/TextureManager.h"
using namespace Core;


void initResource()
{
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/floor.jpg", "floor");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/box.jpg", "box");
	//TextureManager::Inst()->loadTexture("../../../src/Data/texture/3.jpg", "texture3");
	Shader::ptr shader_basic = make_shared<Shader>("../../../src/Data/shader/multi_lighting.vs", "../../../src/Data/shader/multi_lighting.fs");
	Shader::ptr shadow_map = make_shared<Shader>("../../../src/Data/shader/shadow_map.vs", "../../../src/Data/shader/shadow_map.fs");
	ShaderManager::getSingleton()->add("basic", shader_basic);
	ShaderManager::getSingleton()->add("shadow_map", shadow_map);
}
int main()
{
	Shader* shader_map_depth = new Shader("");
	Shader* shader_map = new Shader("", "");

	TextureManager::Inst()->loadTexture();


}