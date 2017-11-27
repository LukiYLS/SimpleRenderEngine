#pragma once
#include <vector>
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Mesh.h"
#include "Plugin.h"
#include "..\Math\Frustum.h"
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
		void renderMeshs(std::vector<Mesh::ptr> meshs);
		void projectObject(Object::ptr object);
		void setPrograme(Mesh::ptr mesh);
		Shader::ptr getShader(Material::MaterialType type);
	private:	
		Scene::ptr _scene;
		Camera::ptr _camera;	

		std::vector<Mesh::ptr> _opaqueMehss;
		std::vector<Mesh::ptr> _transparentMeshs;
		std::vector<Light::ptr> _lights;
		Frustum _frustum;
	};
}


