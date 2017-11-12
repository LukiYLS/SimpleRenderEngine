#include "CommonHead.h"


Mesh* createQuad()
{
	Mesh* mesh = GeometryFactory::MakeBox(5, 5, 5);	
	mesh->addTexture("box");
	mesh->setShaderName("basic");

	return mesh;
}
Scene::ptr createScene()
{	
	Scene::ptr scene = make_shared<Scene>();
	Object::ptr root = make_shared<Object>();	
	
	vector<Vertex> vertices;
	vertices.push_back(Vertex(25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f));
	vector<unsigned int> indices = { 0, 3, 1, 1, 3, 2 };

	Mesh* floor = new Mesh();
	floor->setVertices(vertices);
	floor->setIndex(indices);
	floor->addTexture("floor");
	floor->setShaderName("basic");
	root->add(floor);

	Mesh* mesh1 = createQuad();
	mesh1->setPosition(Vector3D(-5.0, 0.0, 0.0));
	root->add(mesh1);
	Mesh* mesh2 = createQuad();
	mesh2->setPosition(Vector3D(5.0, 0.0, 0.0));
	mesh2->setScale(Vector3D(0.5));
	root->add(mesh2);

	Vector3D light_pos = Vector3D(5.0f, 5.0f, 5.0f);
	Light* point_light = new Light();
	point_light->setType(PointLight);
	point_light->setPosition(light_pos);
	//root->add(point_light);

	Light* dir_light1 = new Light();
	dir_light1->setType(DirectLight);	
	dir_light1->setDirection((-dir_light1->getPosition()).normalize());
	//root->add(dir_light1);

	Light* dir_light2 = new Light();
	dir_light2->setType(DirectLight);
	dir_light2->setPosition(light_pos);
	//dir_light2->setDirection((mesh2->getPosition() - light_pos).normalize());
	//root->add(dir_light2);
	Light* spot_light = new Light();
	spot_light->setType(SpotLight);
	spot_light->setPosition(light_pos);
	spot_light->setDirection((-light_pos).normalize());
	//root->add(spot_light);
	scene->setSceneRoot(root);	
	return scene;
}
void initResource()
{
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/floor.jpg", "floor");
	TextureManager::Inst()->loadTexture("../../../src/Data/texture/box.jpg", "box");
	//TextureManager::Inst()->loadTexture("../../../src/Data/texture/3.jpg", "texture3");
	Shader::ptr shader_basic = make_shared<Shader>("../../../src/Data/shader/basic.vs", "../../../src/Data/shader/basic.fs");
	//Shader::ptr shadow_map = make_shared<Shader>("../../../src/Data/shader/shadow_map.vs", "../../../src/Data/shader/shadow_map.fs");
	ShaderManager::getSingleton()->add("basic", shader_basic);
	//ShaderManager::getSingleton()->add("shadow_map", shadow_map);
}
int main()
{	
	ViewPort::ptr vp = std::make_shared<ViewPort>(0, 0, SCR_WIDTH, SCR_HEIGHT);
	Win::getSingleton()->create();	
	initResource();
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
	camera->setPosition(Vector3D(0.0f,0.0f, -35.0f));	
	camera->lookAt(0.0, 0.0, 0.0);
	//camera->setViewPort(vp);
	Matrix4D m = camera->getViewMatrix();
	Vector3D dir = camera->getDirection();
	Vector3D z = camera->zAxis();
	Scene::ptr scene = createScene();
	
	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
	return 0;
}