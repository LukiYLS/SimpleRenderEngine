#pragma once
#include "../ViewPort.h"
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\geometric.hpp>
#include <memory>
namespace Core {

	class Camera {
	public:
		typedef std::shared_ptr<Camera> ptr;
		Camera() = default;
		Camera(const Camera&) = default;
		Camera(const glm::vec3& pos) :_position(pos) {}
		Camera(const glm::vec3& pos, const glm::quat& quat) :_position(pos), _orientation(quat) {}
	public:

		const glm::quat& getOrientation(void) const { return _orientation; }
		void setOrientation(const glm::quat& quat) { _orientation = quat; }
		const glm::vec3& getPosition(void) const { return _position; }
		void setPosition(const glm::vec3& pos) { _position = pos; }

		void setDirection(float x, float y, float z) { setDirection(glm::vec3(x, y, z)); }
		void setDirection(const glm::vec3& vec);
		glm::vec3 getDirection(void) const { return _orientation * glm::vec3(0, 0, 1); }
		glm::vec3 getUp(void) const { return _orientation * glm::vec3(0, 1, 0); }
		glm::vec3 getRight(void) const { return _orientation * glm::vec3(1, 0, 0); }
		void lookAt(const glm::vec3& target) { setDirection(target - _position); }
		void lookAt(float x, float y, float z) { lookAt(glm::vec3(x, y, z)); }


		void translate(const glm::vec3 &v) { _position += v * _orientation; }
		void translate(float x, float y, float z) { _position += glm::vec3(x, y, z) * _orientation; }

		void rotate(float angle, const glm::vec3 &axis) { _orientation *= glm::angleAxis(angle, axis * _orientation); }
		void rotate(float angle, float x, float y, float z) { _orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * _orientation); }

		void yaw(float angle) { rotate(angle, _orientation*glm::vec3(0, 1, 0)); }
		void pitch(float angle) { rotate(angle, _orientation*glm::vec3(1, 0, 0)); }
		void roll(float angle) { rotate(angle, _orientation*glm::vec3(0, 0, 1)); }

		glm::mat4 getViewMatrix()const { return (glm::translate(glm::mat4_cast(_orientation), _position)); }
		virtual glm::mat4 getProjectionMatrix(){}
	private:
		glm::quat RotationBetweenVectors(const glm::vec3& start, const glm::vec3& dest);

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
		glm::vec3 _position;
		glm::quat _orientation;
	};

}

