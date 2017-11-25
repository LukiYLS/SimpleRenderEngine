#pragma once
#include <vector>
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Mesh.h"
#include "Plugin.h"

namespace SRE {
	class RenderSystem {
	public:		
		RenderSystem(Scene* scene, Camera* camera);
		void render();				
	protected:
		void beforeRender();//渲染之前设置renderstate		
		void renderImpl();
		void afterRender();//一帧结束	
		void resize(int x, int y, int width, int height);

		void projectObject(Object::ptr object);
	private:	
		Scene::ptr _scene;
		Camera::ptr _camera;	

		std::vector<Mesh::ptr> _meshs;
		std::vector<Light::ptr> _lights

	};
}


