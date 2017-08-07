#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace RenderSystem {


	Camera::Camera(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) :
		Eye(eye),
		Center(center),
		Up(up),
		_front(glm::normalize(center - eye)),
		_worldUp(Up),
		_yaw(-90.0f),
		_pitch( 0.0f),
		_movementSpeed(2.5f),
		_mouseSensitivity(0.1f),		
		_right(glm::normalize(glm::cross(_front, _worldUp)))
	{
	}

	void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = _movementSpeed * deltaTime;
		if (direction == FORWARD)
			Eye += _front * velocity;
		if (direction == BACKWARD)
			Eye -= _front * velocity;
		if (direction == LEFT)
			Eye -= _right * velocity;
		if (direction == RIGHT)
			Eye += _right * velocity;
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{
		xoffset *= _mouseSensitivity;
		yoffset *= _mouseSensitivity;

		_yaw += xoffset;
		_pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (_pitch > 89.0f)
				_pitch = 89.0f;
			if (_pitch < -89.0f)
				_pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCamera();
	}

	void Camera::ProcessMouseScroll(float yoffset)
	{
		if (_fovy >= 1.0f && _fovy <= 45.0f)
			_fovy -= yoffset;
		if (_fovy <= 1.0f)
			_fovy = 1.0f;
		if (_fovy >= 45.0f)
			_fovy = 45.0f;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(Eye, Center, Up);
	}
	glm::mat4 Camera::getProjectMatrix()
	{
		return glm::perspective(_fovy, _aspect, _near, _far);
	}

	void Camera::updateCamera()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		front.y = sin(glm::radians(_pitch));
		front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_front = glm::normalize(front);
	
		_right = glm::normalize(glm::cross(_front, _worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(_right, _front));
		Center = Eye + _front;
	}
	void Camera::setPerspectiveFovLHMatrix(float fovy, float aspect, float near, float far)
	{
		_fovy = fovy;
		_aspect = aspect;
		_near = near;
		_far = far;
	}
		
}
