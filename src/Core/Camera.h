#pragma once
#include "ViewPort.h"
#include "../Math/Matrix4D.h"
#include "../Math/Quaternion.h"
#include "../Math/Vector3D.h"
#include <memory>
namespace Core {

	class Camera {
	public:
		typedef std::shared_ptr<Camera> ptr;
		Camera() = default;
		Camera(const Camera&) = default;
		Camera(const Vector3D& pos) :_position(pos) {}
		Camera(const Vector3D& pos, const Quaternion& quat) :_position(pos), _orientation(quat) {}
	public:

		const Quaternion& getOrientation(void) const { return _orientation; }
		void setOrientation(const Quaternion& quat) { _orientation = quat; }
		const Vector3D& getPosition(void) const { return _position; }
		void setPosition(const Vector3D& pos) { _position = pos; }

		void setDirection(float x, float y, float z) { setDirection(Vector3D(x, y, z)); }
		void setDirection(const Vector3D& vec);
		Vector3D getDirection(void) const { return _orientation * Vector3D(0, 0, 1); }
		Vector3D getUp(void) const { return _orientation * Vector3D(0, 1, 0); }
		Vector3D getRight(void) const { return _orientation * Vector3D(1, 0, 0); }
		void lookAt(const Vector3D& target) { setDirection(target - _position); }
		void lookAt(float x, float y, float z) { lookAt(Vector3D(x, y, z)); }

		void translate(const Vector3D &v) { _position += _orientation.Inverse() * v; }
		void translate(float x, float y, float z) { _position += _orientation.Inverse() * Vector3D(x, y, z); }


		void rotate(float angle, const Vector3D &axis);
		void rotate(float angle, float x, float y, float z) { rotate(angle, Vector3D(x, y, z)); }

		void yaw(float angle) { rotate(angle, _orientation.xAxis()); }
		void pitch(float angle) { rotate(angle, _orientation.yAxis()); }
		void roll(float angle) { rotate(angle, _orientation.zAxis()); }

		Matrix4D getViewMatrix()const { return Matrix4D::makeTransformMatrix(_position, Vector3D(1.0), _orientation); }
		virtual Matrix4D getProjectionMatrix() { return Matrix4D(); }
	private:
		Quaternion RotationBetweenVectors(const Vector3D& start, const Vector3D& dest);

		//for mouse and keyboard control
	public:
		void ProcessKeyboard(float xoffset, float yoffset);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void setViewPort(ViewPort::ptr vp) { _view_port = vp; }
		ViewPort::ptr getViewPort() { return _view_port; }
		//void ProcessMouseScroll(float yoffset);
	private:
		float _movementSpeed;
		float _mouseSensitivity;
		ViewPort::ptr _view_port;
	private:
		Vector3D _position;
		Quaternion _orientation;
	};

}

