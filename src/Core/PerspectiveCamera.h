#pragma once
#include "Camera.h"

namespace Core {
	class PerspectiveCamera :public Camera {
	public:
		typedef std::shared_ptr<PerspectiveCamera> ptr;
		PerspectiveCamera(float fovy, float aspect, float zNear, float zFar):
		_fovy(fovy), _aspect(aspect), _near(zNear), _far(zFar) {}

		Matrix4D getProjectMatrix() const { return Matrix4D::makeProjectionMatrix(_fovy, _aspect, _near, _far); }

	private:
		float _fovy;
		float _aspect;
		float _near;
		float _far;
	};
}

