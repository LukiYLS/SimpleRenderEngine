#include "Camera.h"
#include "../math/MathHelper.h"
namespace Core {

	Quaternion Camera::RotationBetweenVectors(const Vector3D& start, const Vector3D& dest)
	{
		Quaternion quat;
		Vector3D start_normal = start; start_normal.normalize();
		Vector3D dest_normal = dest; dest_normal.normalize();

		float cosTheta = start_normal.dot(dest_normal);
		Vector3D rotationAxis;

		if (cosTheta < -1 + 0.001f) {
			rotationAxis = Vector3D(0.0f, 0.0f, 1.0f).cross(start_normal);
			if (rotationAxis.length() < 0.01)
				rotationAxis = Vector3D(1.0f, 0.0f, 0.0f).cross(start_normal);
			rotationAxis = -rotationAxis.normalize();
			quat.fromAngleAxis(MathHelper::radian(180.0), rotationAxis);
			return quat;
		}

		rotationAxis = start_normal.cross(dest_normal);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return Quaternion(
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
	void Camera::rotate(float angle, const Vector3D &axis)
	{
		Quaternion quat;
		quat.fromAngleAxis(angle, _orientation.Inverse() * axis);
		_orientation = _orientation * quat;	
	}
	void Camera::setDirection(const Vector3D& vec)
	{
		//glm::vec3 axis[3] = quatToAxis(_orientation);
		Vector3D direction = getDirection();
		Quaternion q = RotationBetweenVectors(direction, vec);
		_orientation = _orientation * q;
	}

	void Camera::ProcessKeyboard(float xoffset, float yoffset)
	{	
		Vector3D direction = getDirection();
		Vector3D right = getRight();
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