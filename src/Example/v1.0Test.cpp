

#include "CommonHead.h"


Scene::ptr createScene()
{
	Scene::ptr scene = std::make_shared<Scene>();

	//texture
	Texture::ptr earthTex = TextureManager::Inst()->loadTexture("earth", "../../../src/Data/texture/earthmap.jpg");
	Texture::ptr boxTex = TextureManager::Inst()->loadTexture("cloud", "../../../src/Data/texture/box.jpg");
	Texture::ptr floorTex = TextureManager::Inst()->loadTexture("earth", "../../../src/Data/texture/floor.jpg");

	TextureUnitState::ptr earthUnit = std::make_shared<TextureUnitState>();
	earthUnit->setTexture(earthTex);
	TextureUnitState::ptr boxUnit = std::make_shared<TextureUnitState>();
	boxUnit->setTexture(boxTex);
	TextureUnitState::ptr floorUnit = std::make_shared<TextureUnitState>();
	floorUnit->setTexture(floorTex);
	//material
	Material::ptr floorMat = std::make_shared<Material>();
	floorMat->setMaterialType(Material::PhongMaterial);
	floorMat->setMap(floorUnit);
	floorMat->setCullFaceMode(CullFaceMode::DoubleSide);

	Material::ptr earthMat = std::make_shared<Material>();
	earthMat->setMap(earthUnit);
	earthMat->setMaterialType(Material::PhongMaterial);

	Material::ptr boxMat = std::make_shared<Material>();
	boxMat->setMaterialType(Material::PhongMaterial);
	boxMat->setMap(boxUnit);

	//mesh
	Mesh* floor = GeometryFactory::MakeQuad(100, 100);
	floor->setMaterial(floorMat);

	Mesh* box1 = GeometryFactory::MakeBox(20.0, 20.0, 20.0);
	box1->setPosition(Vector3D(-25.0, 10.0, 25.0));	
	box1->setMaterial(boxMat);

	Mesh* box2 = GeometryFactory::MakeBox(10.0, 10.0, 10.0);
	box2->setPosition(Vector3D(0.0, 5.0, -25.0));
	box2->setMaterial(boxMat);

	Mesh* box3 = GeometryFactory::MakeBox(10.0, 10.0, 10.0);
	box3->setPosition(Vector3D(25.0, 5.0, 25.0));
	box3->setMaterial(boxMat);

	Mesh* sphere = GeometryFactory::MakeSphere(8.0, 32, 32);
	sphere->setPosition(Vector3D(0.0, 8.0, 0.0));	
	sphere->setMaterial(earthMat);
	
	Object::ptr root = std::make_shared<Object>();

	//light
	DirectionLight* dlight = new DirectionLight();
	dlight->setPosition(Vector3D(0.0, 50.0, 50.0));
	dlight->setShadowCamera(new OrthographicCamera(-50.0, 50.0, -50.0, 50.0, 0.1, 100.0));

	PointLight* plight = new PointLight();
	plight->setPosition(Vector3D(0.0, 50.0, 0.0));
	plight->setShadowCamera(new PerspectiveCamera(MathHelper::radian(70.0), 1.0, 1.0, 200.0));

	SpotLight* spotlight = new SpotLight;
	spotlight->setPosition(Vector3D(0.0, 30.0, 0.0));
	spotlight->setAngle(M_PI/6.0);
	spotlight->setDecay(1.0);
	spotlight->setDistance(00.0);
	spotlight->setPenumbra(0.05);
	spotlight->setShadowCamera(new PerspectiveCamera(MathHelper::radian(50.0), 1.0, 1.0, 200.0));

	root->add(floor);
	root->add(box1);
	root->add(box2);
	root->add(box3);
	root->add(sphere);

	root->add(dlight);
	root->add(plight);
	root->add(spotlight);
	scene->setSceneRoot(root);
	scene->setUseShadowMap(true);

	return scene;
}
Texture::ptr loadSkybox()
{
	std::string path = "../../../src/Data/texture/skybox2/";
	std::vector<std::string> skyboxfile = {	path + "right.tga",
		path + "left.tga",
		path + "top.tga",
		path + "bottom.tga",
		path + "back.tga",
		path + "front.tga"};
	Texture::ptr skybox = TextureManager::Inst()->loadCubeMap("skybox", skyboxfile);
	return skybox;
}
void main()
{
	Win::getSingleton()->create();	
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(65.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 500.0);
	camera->setPosition(Vector3D(0.0f, 50.0f, -50.0));
	camera->lookAt(0.0, 0.0, 0.0);
	TerrianTile* tt = new TerrianTile();
	tt->loadFromHeightMap("../../../src/Data/texture/HeightMap.jpg");
	Scene::ptr scene = createScene();
	Skybox* skybox = new Skybox;
	skybox->setTexture(loadSkybox());
	scene->setSkybox((Skybox::ptr)skybox);
	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
}