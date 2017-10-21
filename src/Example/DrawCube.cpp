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
#include "../Core/GeometryFactory.h"
#include "../Core/Win.h"
#include <vector>
#include <iostream>
using namespace Core;
using namespace std;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace Utils;


Mesh* createQuad()
{
	

	Mesh* mesh = GeometryFactory::MakeBox(2, 2, 2);	
	mesh->addTexture("box");
	mesh->setShaderName("basic_shader");

	return mesh;
}
Scene::ptr createScene()
{	
	Scene::ptr scene = make_shared<Scene>();
	Object::ptr root = make_shared<Object>();
	

	vector<Vertex> vertices;
	vertices.push_back(Vertex(5.f,0.0f, 5.f, 0, 1, 0, 1, 1));
	vertices.push_back(Vertex(5.f, 0.0f,-5.f, 0, 1, 0, 1, 0));
	vertices.push_back(Vertex(-5.f,0.0f, -5.f, 0, 1, 0, 0, 0));
	vertices.push_back(Vertex(-5.f,0.0f ,5.f,   0, 1, 0, 0, 1));
	vector<unsigned int> indices = { 0, 3, 1, 1, 3, 2 };

	Mesh* floor = new Mesh();
	floor->setVertices(vertices);
	floor->setIndex(indices);
	floor->addTexture("floor");
	floor->setShaderName("basic_shader");
	root->add(floor);

	Mesh* mesh1 = createQuad();
	mesh1->setPosition(Vector3D(-2.0, 0.0, 0.0));
	root->add(mesh1);
	Mesh* mesh2 = createQuad();
	mesh2->setPosition(Vector3D(2.0, 0.0, 0.0));
	root->add(mesh2);

	Vector3D light_pos = Vector3D(1.0, 2.0, 0.0);
	Light* point_light = new Light();
	point_light->setType(PointLight);
	point_light->setPosition(light_pos);
	//root->add(point_light);

	Light* dir_light1 = new Light();
	dir_light1->setType(DirectLight);	
	dir_light1->setDirection((-dir_light1->getPosition()).normalize());
	root->add(dir_light1);

	Light* dir_light2 = new Light();
	dir_light2->setType(DirectLight);
	dir_light2->setPosition(light_pos);
	dir_light2->setDirection((mesh2->getPosition() - light_pos).normalize());
	//root->add(dir_light2);
	Light* spot_light = new Light();
	spot_light->setType(SpotLight);
	spot_light->setPosition(Vector3D(0,5,0));
	spot_light->setDirection(-light_pos.normalize());
	//root->add(spot_light);

	scene->setSceneRoot(root);	
	return scene;
}
void initResource()
{
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/floor.jpg", "floor");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/box.jpg", "box");
	//TextureManager::Inst()->loadTexture("../../../src/Data/texture/3.jpg", "texture3");
	Shader::ptr shader_basic = make_shared<Shader>("../../../src/Data/shader/multi_lighting.vs", "../../../src/Data/shader/multi_lighting.fs");
	ShaderManager::getSingleton()->add("basic_shader", shader_basic);
}
int main()
{	
	ViewPort::ptr vp = std::make_shared<ViewPort>(0, 0, SCR_WIDTH, SCR_HEIGHT);
	Win::getSingleton()->create();	
	initResource();
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
	camera->setPosition(Vector3D(0.0f,10.0f, -10.0f));	
	camera->lookAt(0.0, 0.0, 0.0);
	camera->setViewPort(vp);
	CameraControl::ptr cc = make_shared<CameraControl>(camera.get());
	EventManager::Inst()->registerReceiver("mouse.event", cc);
	EventManager::Inst()->registerReceiver("keyboard.event", cc);
	Scene::ptr scene = createScene();
	scene->enableShadow();
	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
	return 0;
}