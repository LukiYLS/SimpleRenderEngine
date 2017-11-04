#pragma once
#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Plugin.h"
namespace Core {

	class BillboardCollection 
		:public Object,public Plugin
	{
	public:
		typedef std::shared_ptr<BillboardCollection> ptr;
		BillboardCollection() = default;
		~BillboardCollection() = default;
		void setPositions(std::vector<Vector3D> positions) { _positions = positions; }
		void setTexture(Texture* texture) { _texture = (Texture::ptr)texture; }
		virtual void init();
		virtual void render(Camera* camera);
	protected:
		Texture::ptr _texture;
		Shader::ptr _shader;
		std::vector<Vector3D> _positions;
		GLuint _vao;
	};
}