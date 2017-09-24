#include "RenderSystem.h"
#include "RenderState.h"
#include "Scene.h"
#include "SubMesh.h"
namespace Core {
	RenderSystem* RenderSystem::m_Inst(0);
	RenderSystem* RenderSystem::Inst() {
		if (m_Inst)
			m_Inst = new RenderSystem();
		return m_Inst;
	}

	void RenderSystem::beginRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void RenderSystem::render()
	{
		//Scene::Inst()->render(params);
	}
	void RenderSystem::renderObjects(Group::ptr objects)
	{
		for (int i = 0; i < objects->getNumChildren(); i++)
		{
			Node::ptr node = objects->getChild(i);
			if (node->asGroup() != NULL)
				renderObjects(node->asGroup());
			else
				renderObject(node);
		}
	}
	void RenderSystem::renderObject(Node::ptr node)
	{
		SubMesh * mesh = node->asSubMesh();
		if (mesh != 0)
		{
			mesh->draw()
		}
	}
	void RenderSystem::render(Scene::ptr scene, Camera::ptr camera)
	{
		if (scene->autoUpdate)scene->updateMatrixWorld();

		Group::ptr root = scene->getSceneRoot();
		
		for (int i = 0; i < root->getNumChildren(); i++)
		{
			Node::ptr node = root->getChild(i);
			if (node->asGroup() != NULL)
				renderObjects(node->asGroup());
			else
				renderObject(node);
		}

	}
	void RenderSystem::endRender()
	{

	}
}