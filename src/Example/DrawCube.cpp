#include "../Core/TextureManager.h"
#include "../Core/Shader.h"
#include "../Core/Camera/PerspectiveCamera.h"
#include "../Core/Mesh.h"
#include "../Core/Light.h"
#include "../Core/Win.h"
#include "../Core/Scene.h"
#include "../Core/utils/CamerControl.h"
#include "../Core/utils/Event.h"
#include <vector>
#include <iostream>
using namespace Core;
using namespace std;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
using namespace Utils;
int main()
{
	
	Win::Inst()->createWindow();	
	
	vector<Vertex> vertices;
	vertices.push_back(Vertex(0.5f, 0.5f, 0.0f, 0, 0, -1, 1, 1));
	vertices.push_back(Vertex(0.5f, -0.5f, 0.0f, 0, 0, -1, 1, 0));
	vertices.push_back(Vertex(-0.5f, -0.5f, 0.0f, 0, 0, -1, 0, 0));
	vertices.push_back(Vertex(-0.5f, 0.5f, 0.0f, 0, 0, -1, 0, 1));
	vector<unsigned int> indices = { 0, 3, 1, 1, 3, 2 };
	
	Mesh::ptr mesh = std::make_shared<Mesh>();
	mesh->setVertices(vertices);
	mesh->setIndex(indices);
	
	mesh->createBuffer();
	
	Light::ptr light = std::make_shared<Light>();
	light->setType(PointLight);

	TextureManager::Inst()->loadTexture("../../../src/Data/texture/1.jpg", "texture1");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/2.jpg", "texture2");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/3.jpg", "texture3");

	//Camera::ptr camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	//camera->setPerspectiveFovLHMatrix(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	camera->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));	



	Shader::ptr shader = std::make_shared<Shader>("../../../src/Data/shader/Core.vs", "../../../src/Data/shader/Core.fs");	
	mesh->setProgram(shader);	

	mesh->addTexture("texture3");

	Scene::Inst()->addEntity("test", (Entity::ptr)mesh);
	Scene::Inst()->addLight(light);

	RenderParams * params = new RenderParams();
	params->setEye(camera->getPosition());
	params->setM(glm::mat4());
	params->setV(camera->getViewMatrix());
	params->setP(camera->getProjectMatrix());

	CameraControl::ptr cc = make_shared<CameraControl>((Core::Camera::ptr)camera);
	EventManager::Inst()->registerReceiver("mouse.event", cc);//new完之后应该自动注册

	
	Win::Inst()->starup(params);
	
}