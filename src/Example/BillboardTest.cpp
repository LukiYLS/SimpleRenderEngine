#include "CommonHead.h"
BillboardCollection* createBBC()
{
	BillboardCollection* bbc = new BillboardCollection;
	bbc->setPosition();
	bbc->setTexture();
	return bbc;
}
Scene* createScene()
{
	Scene::ptr scene = std::make_shared<Scene>();
	Object::ptr root = std::make_shared<Object>();
	scene->setSceneRoot(root);
	root->add(createBBC());
}
int main()
{
	Win::getSingleton()->create();

	PerspectiveCamera::ptr camera = make_shared<PerspectiveCamera>(MathHelper::radian(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);

	
	//scene->add()...
	//
}