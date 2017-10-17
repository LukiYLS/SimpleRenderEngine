#include "../Core/RenderSystem.h"
#include "../Core/TextureManager.h"
#include "../Core/ShaderManager.h"
#include "../Core/PerspectiveCamera.h"
#include "../Core/Mesh.h"
#include "../Core/Light.h"
#include "../Core/Scene.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
#include "../Math/MathHelper.h"
#include <vector>
#include <iostream>
using namespace Core;
using namespace std;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace Utils;


Mesh* createQuad()
{
	vector<Vertex> vertices;
	vertices.push_back(Vertex(0.5f, 0.5f, 0.0f, 0, 0, -1, 1, 1));
	vertices.push_back(Vertex(0.5f, -0.5f, 0.0f, 0, 0, -1, 1, 0));
	vertices.push_back(Vertex(-0.5f, -0.5f, 0.0f, 0, 0, -1, 0, 0));
	vertices.push_back(Vertex(-0.5f, 0.5f, 0.0f, 0, 0, -1, 0, 1));
	vector<unsigned int> indices = { 0, 3, 1, 1, 3, 2 };

	Mesh::ptr mesh = std::make_shared<Mesh>();
	mesh->setVertices(vertices);
	mesh->setIndex(indices);
	mesh->addTexture("texture1");
	mesh->setShaderName("basic_shader");

	return mesh.get();
}
Scene* createScene()
{
	
	Scene::ptr scene = make_shared<Scene>();
	Object::ptr root = make_shared<Object>();
	root->add(createQuad());

	Light::ptr light = std::make_shared<Light>();
	light->setType(PointLight);

	root->add(light.get());
	scene->setSceneRoot(root);

	return scene.get();
}
void initResource()
{


	TextureManager::Inst()->loadTexture("../../../src/Data/texture/1.jpg", "texture1");
//	TextureManager::Inst()->loadTexture("../../../src/Data/texture/2.jpg", "texture2");
//	TextureManager::Inst()->loadTexture("../../../src/Data/texture/3.jpg", "texture3");

	Shader::ptr shader_basic = make_shared<Shader>("basic", "../../../src/Data/shader/basic.vs", "../../../src/Data/shader/basic.fs");
	ShaderManager::getSingleton()->add("basic_shader", shader_basic.get());

}
int main()
{
	RenderSystem* rs = new RenderSystem;
	rs->createWindow();
	initResource();
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	camera->setPosition(Vector3D(0.0f, 0.0f, -2.0f));
	
	CameraControl::ptr cc = make_shared<CameraControl>(camera.get());
	EventManager::Inst()->registerReceiver("mouse_event", cc);
	Scene* scene = createScene();
	rs->render(createScene(), camera.get());
}