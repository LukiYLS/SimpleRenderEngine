

#include "CommonHead.h"


Scene::ptr createScene()
{
	Scene::ptr scene = std::make_shared<Scene>();

	Texture::ptr texture1 = TextureManager::Inst()->loadTexture("earth", "../../../src/Data/texture/earthmap.jpg");
	Texture::ptr texture2 = TextureManager::Inst()->loadTexture("cloud", "../../../src/Data/texture/cloudsmap.jpg");

	TextureUnitState::ptr us1 = std::make_shared<TextureUnitState>();
	us1->setTexture(texture1);
	TextureUnitState::ptr us2 = std::make_shared<TextureUnitState>();
	us2->setTexture(texture2);

	Mesh* box = GeometryFactory::MakeBox(5.0, 5.0, 5.0);
	//Mesh* mesh_box = dynamic_cast<Mesh*>(box.get());
	Material::ptr mesh_mat1 = std::make_shared<Material>();
	mesh_mat1->setMaterialType(Material::PhongMaterial);
	mesh_mat1->setMap(us1);
	box->setMaterial(mesh_mat1);

	Mesh* sphere = GeometryFactory::MakeSphere(5.0, 32, 32);
	//Mesh* mesh_sphere = dynamic_cast<Mesh*>(sphere.get());
	Material::ptr mesh_mat2 = std::make_shared<Material>();
	mesh_mat2->setMap(us2);
	mesh_mat2->setMaterialType(Material::PhongMaterial);
	sphere->setMaterial(mesh_mat2);
	
	Object::ptr root = std::make_shared<Object>();

	//root->add(box);
	root->add(sphere);

	scene->setSceneRoot(root);

	return scene;
}
void main()
{
	Win::getSingleton()->create();	
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 100.0);
	camera->setPosition(Vector3D(0.0f, 0.0f, 50.0));
	camera->lookAt(0.0, 0.0, 0.0);

	Scene::ptr scene = createScene();

	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
}