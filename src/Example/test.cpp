

#include "CommonHead.h"

Mesh* createFloor() {


	Mesh* mesh = new Mesh;


	VertexData* vertexdata = new VertexData;
	vertexdata->setVertexStart(0);
	vertexdata->setVertexCount(4);
	VertexDeclaration::ptr vd = vertexdata->getVertexDeclaration();
	VertexBufferBinding::ptr bind = vertexdata->getVertexBufferBinding();
	size_t offset = 0;
	VertexElement::ptr tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_POSITION);
	offset += tmp_ve->getTypeSize(VET_FLOAT3);

	tmp_ve = vd->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
	offset += tmp_ve->getTypeSize(VET_FLOAT3);

	tmp_ve = vd->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
	offset += tmp_ve->getTypeSize(VET_FLOAT2);

	char* data = (char*)malloc(sizeof(char)*vd->getVertexSize(0)*4);

	float vertices[32] = {
		-50.0, 0.0, 50.0, 0.0, 1.0, 0.0, 0.0, 0.0,
		50.0, 0.0, 50.0, 0.0, 1.0, 0.0, 0.0, 1.0,
		-50.0, 0.0, -50.0, 0.0, 1.0, 0.0, 1.0, 0.0,
		50.0, 0.0, -50.0, 0.0, 1.0, 0.0, 1.0, 1.0
	};

	HardwareVertexBuffer* buffer = new HardwareVertexBuffer(offset, 4, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	bind->setBinding(0, (HardwareVertexBuffer::ptr)buffer);
	buffer->writeData(0, buffer->getSizeInBytes(), vertices);

	IndexData* indexdata = new IndexData;
	indexdata->setIndexStart(0);
	indexdata->setIndexCount(6);
	HardwareIndexBuffer * index_buffer = new HardwareIndexBuffer(HardwareIndexBuffer::IT_16BIT, 6, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	indexdata->setHardwareIndexBuffer((HardwareIndexBuffer::ptr)index_buffer);

	unsigned short faces[36] = {
		
		0,1,2,
		2,1,3,
	};

	index_buffer->writeData(0, index_buffer->getSizeInBytes(), faces);
	
	mesh->setVertexData((VertexData::ptr)vertexdata);
	mesh->setIndexData((IndexData::ptr)indexdata);

	return mesh;

}
Scene::ptr createScene()
{
	Scene::ptr scene = std::make_shared<Scene>();

	Texture::ptr earthTex = TextureManager::Inst()->loadTexture("earth", "../../../src/Data/texture/earthmap.jpg");
	Texture::ptr boxTex = TextureManager::Inst()->loadTexture("cloud", "../../../src/Data/texture/box.jpg");
	Texture::ptr floorTex = TextureManager::Inst()->loadTexture("earth", "../../../src/Data/texture/floor.jpg");

	TextureUnitState::ptr earthUnit = std::make_shared<TextureUnitState>();
	earthUnit->setTexture(earthTex);
	TextureUnitState::ptr boxUnit = std::make_shared<TextureUnitState>();
	boxUnit->setTexture(boxTex);
	TextureUnitState::ptr floorUnit = std::make_shared<TextureUnitState>();
	floorUnit->setTexture(floorTex);

	Material::ptr floorMat = std::make_shared<Material>();
	floorMat->setMaterialType(Material::PhongMaterial);
	floorMat->setMap(floorUnit);
	floorMat->setCullFaceMode(CullFaceMode::DoubleSide);

	Mesh* floor = createFloor();
	floor->setMaterial(floorMat);
	
	Material::ptr boxMat = std::make_shared<Material>();
	boxMat->setMaterialType(Material::PhongMaterial);
	boxMat->setMap(boxUnit);

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
	
	Material::ptr earthMat = std::make_shared<Material>();
	earthMat->setMap(earthUnit);
	earthMat->setMaterialType(Material::PhongMaterial);
	sphere->setMaterial(earthMat);
	
	Object::ptr root = std::make_shared<Object>();

	DirectionLight* dlight = new DirectionLight();
	dlight->setPosition(Vector3D(0.0, 50.0, 50.0));
	dlight->setShadowCamera(new OrthographicCamera(-50.0, 50.0, -50.0, 50.0, 0.1, 100.0));

	PointLight* plight = new PointLight();
	plight->setPosition(Vector3D(0.0, 20.0, 0.0));
	plight->setShadowCamera(new PerspectiveCamera(MathHelper::radian(90.0), 1.0, 1.0, 5000.0));



	SpotLight* spotlight = new SpotLight;
	spotlight->setPosition(Vector3D(0.0, 50.0, 0.0));
	spotlight->setAngle(M_PI/8.0);
	spotlight->setDecay(1.0);
	spotlight->setDistance(500.0);
	spotlight->setPenumbra(0.05);
	spotlight->setShadowCamera(new PerspectiveCamera(MathHelper::radian(50.0), 1.0, 1.0, 500.0));
	root->add(floor);
	root->add(box1);
	root->add(box2);
	root->add(box3);
	root->add(sphere);

	root->add(dlight);
	//root->add(plight);
	root->add(spotlight);
	scene->setSceneRoot(root);
	scene->setUseShadowMap(true);

	return scene;
}
void main()
{
	Win::getSingleton()->create();	
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(65.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 500.0);
	camera->setPosition(Vector3D(0.0f, 50.0f, -50.0));
	camera->lookAt(0.0, 0.0, 0.0);

	Scene::ptr scene = createScene();

	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
}