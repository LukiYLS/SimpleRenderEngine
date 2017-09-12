#include "Camera.h"

namespace Core {

	glm::quat Camera::RotationBetweenVectors(const glm::vec3& start, const glm::vec3& dest)
	{
		glm::vec3 start_normal = normalize(start);
		glm::vec3 dest_normal = normalize(dest);

		float cosTheta = glm::dot(start_normal, dest_normal);
		glm::vec3 rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start_normal);
			if (glm::length(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start_normal);
			rotationAxis = -normalize(rotationAxis);
			return glm::angleAxis(glm::radians(180.0f), rotationAxis);
		}

		rotationAxis = cross(start_normal, dest_normal);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);
	}
	/*glm::mat4 Camera::getViewMatrix() const
	{
		glm::mat4 viewMatrix;

		// View matrix is:
		//
		//  [ Lx  Uy  Dz  Tx  ]
		//  [ Lx  Uy  Dz  Ty  ]
		//  [ Lx  Uy  Dz  Tz  ]
		//  [ 0   0   0   1   ]
		//
		// Where T = -(Transposed(Rot) * Pos)

		
		glm::mat3 rot = glm::mat3_cast(_orientation);
		rot = glm::inverse(rot);
		// Make the translation relative to new axes
		glm::mat3 rotT = glm::transpose(rot);
		glm::vec3 trans = rotT * _position;

	
		viewMatrix = glm::mat4(rotT); 
		viewMatrix[3][0] = trans.x;
		viewMatrix[3][1] = trans.y;
		viewMatrix[3][2] = trans.z;		

		return viewMatrix;
	}*/
	void Camera::setDirection(const glm::vec3& vec)
	{
		//glm::vec3 axis[3] = quatToAxis(_orientation);
		glm::vec3 direction = getDirection();
		glm::quat q = RotationBetweenVectors(direction, vec);
		_orientation *= q;			
	}

	void Camera::ProcessKeyboard(float xoffset, float yoffset)
	{	
		glm::vec3 direction = getDirection();
		glm::vec3 right = getRight();
		_position += direction * _movementSpeed * xoffset;		
		_position += right * _movementSpeed * yoffset;
	}

	void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
	{
		xoffset *= _mouseSensitivity;
		yoffset *= _mouseSensitivity;

		yaw(xoffset);
		pitch(yoffset);

		if (constrainPitch)
		{
			//if (_pitch > 89.0f)
			//	_pitch = 89.0f;
			//if (_pitch < -89.0f)
			//	_pitch = -89.0f;
		}

	}
}