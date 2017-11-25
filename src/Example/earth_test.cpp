#include "CommonHead.h"


#define radius 6378137.f
Mesh* createSphere()
{
	Mesh* mesh = GeometryFactory::MakeSphereOld(1.025*radius, 128, 64);
//	mesh->addTexture("earth");
//	mesh->addRenderPass("atmosphere");
//	mesh->addRenderPass("earth");
	mesh->setShaderName("skyfromspace");
	
	return mesh;
}

Scene::ptr createScene()
{
	Scene::ptr scene = make_shared<Scene>();
	Object::ptr root = make_shared<Object>();
	Mesh* mesh = createSphere();

	Mesh* mesh1 = GeometryFactory::MakeSphereOld(radius, 128, 64);
	mesh1->addTexture("earth");
	mesh1->addTexture("cloud");
	mesh1->addTexture("light");
	mesh1->setShaderName("groundfromspace");
	//root->add(mesh1);
	root->add(mesh);
	
	scene->setSceneRoot(root);
	return scene;
}

void initResource()
{
	//RenderObject::ptr ro = GeometryFactory::MakeBox(10, 10, 10);
	//Shader::ptr basic = make_shared<Shader>("../../../src/Data/shader/basic.vs", "../../../src/Data/shader/basic.fs");
	///basic->use();
	//ro->drawPrimitive();
	//RenderObject::ptr ro2 = GeometryFactory::MakeSphere(10, 10, 10);

	TextureManager::Inst()->loadTexture("earth","../../../src/Data/texture/earthmap.jpg");
	TextureManager::Inst()->loadTexture( "cloud", "../../../src/Data/texture/cloudsmap.jpg");
	TextureManager::Inst()->loadTexture("light","../../../src/Data/texture/lightsmap.jpg" );
	Shader::ptr shader_skyfromspace = make_shared<Shader>("../../../src/Data/shader/skyfromspace.vs", "../../../src/Data/shader/skyfromspace.fs");
	Shader::ptr shader_skyfromatmosphere = make_shared<Shader>("../../../src/Data/shader/skyfromatmosphere.vs", "../../../src/Data/shader/skyfromatmosphere.fs");
	Shader::ptr shader_groundfromspace = make_shared<Shader>("../../../src/Data/shader/groundfromspace.vs", "../../../src/Data/shader/groundfromspace.fs");
	Shader::ptr shader_groundfromatmosphere = make_shared<Shader>("../../../src/Data/shader/groundfromatmosphere.vs", "../../../src/Data/shader/groundfromatmosphere.fs");
	float Kr = 0.0030f;
	float Km = 0.0015f;
	float ESun = 15.0f;
	float g = -0.95f;
	float InnerRadius = radius;;
	float OuterRadius = 1.025*radius;
	float Scale = 1.0f / (OuterRadius - InnerRadius);
	float ScaleDepth = 0.25f;
	float ScaleOverScaleDepth = Scale / ScaleDepth;
	for (int i = 0; i < 4; i++)
	{
		Shader::ptr shader = (i == 0) ? shader_skyfromspace : (i == 1 ? shader_skyfromatmosphere : (i == 2 ? shader_groundfromspace : shader_groundfromatmosphere));
		shader->use();
		shader->setVec3("v3LightDirection", Vector3D(0.0,0.0, 1.0));
		shader->setVec3("v3InvWavelength", Vector3D(1.0f / powf(0.650f, 4.0f), 1.0f / powf(0.570f, 4.0f), 1.0f / powf(0.475f, 4.0f)));
		shader->setFloat("fInnerRadius", InnerRadius);
		shader->setFloat("fInnerRadius2", InnerRadius * InnerRadius);
		shader->setFloat("fOuterRadius", OuterRadius);
		shader->setFloat("fOuterRadius2", OuterRadius * OuterRadius);
		shader->setFloat("fKrESun", Kr * ESun);
		shader->setFloat("fKmESun", Km * ESun);
		shader->setFloat("fKr4PI", Kr * 4.0f * (float)M_PI);
		shader->setFloat("fKm4PI", Km * 4.0f * (float)M_PI);
		shader->setFloat("fScale", Scale);
		shader->setFloat("fScaleDepth", ScaleDepth);
		shader->setFloat("fScaleOverScaleDepth", ScaleOverScaleDepth);
		shader->setFloat("g", g);
		shader->setFloat("g2", g * g);
		shader->setInt("Samples", 4);
	}

	ShaderManager::getSingleton()->add("skyfromspace", shader_skyfromspace);
	ShaderManager::getSingleton()->add("skyfromatmosphere", shader_skyfromatmosphere);
	ShaderManager::getSingleton()->add("groundfromspace", shader_groundfromspace);
	ShaderManager::getSingleton()->add("groundfromatmosphere", shader_groundfromatmosphere);

}

int main()
{
	Win::getSingleton()->create();
	initResource();
	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, radius*0.5, radius*5);
	camera->setPosition(Vector3D(0.0f, 0.0f, radius*3));
	camera->lookAt(0.0, 0.0, 0.0);

	Scene::ptr scene = createScene();

	RenderSystem *rs = new RenderSystem(scene.get(), camera.get());
	Win::getSingleton()->loadRenderSystem(rs);
	Win::getSingleton()->startRenderLoop();

	delete rs;
	return 0;
}