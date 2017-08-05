#include "Camera.h"



namespace RenderSystem {


	Camera::Camera(const glm::vec3& pos = glm::vec3(0.0f,0.0f,0.0f), const glm::vec3& lookat = glm::vec3(), const glm::vec3& up):
		
}
/*

void PerspectiveCamera::setCamera(const Vector3& pos, const Vector3& look, const Vector3& up)
{
	_position = pos;
	_lookAt = look;
	_up = up;
	updataVeiwMatrix();
}

void PerspectiveCamera::setPosition(const Vector3& pos)
{
	_position = pos;
	updataVeiwMatrix();
}

void PerspectiveCamera::setLookAt(const Vector3& lookAt)
{
	_lookAt = lookAt;
	updataVeiwMatrix();
}

void PerspectiveCamera::setUp(const Vector3& up)
{
	_up = up;
	updataVeiwMatrix();
}

const Vector3& PerspectiveCamera::getPosition()
{
	return _position;
}

const Vector3& PerspectiveCamera::getLookAt()
{
	return _lookAt;
}

const Vector3& PerspectiveCamera::getUp()
{
	return _up;
}

const Matrix4& PerspectiveCamera::getViewMatrix()
{
	return _viewMatrix;
}

const Matrix4& PerspectiveCamera::getProjectMatrix()
{
	return _projectMatrix;
}

void PerspectiveCamera::updataVeiwMatrix()
{
	Vector3 zAxis, xAxis, yAxis;
	float length, result1, result2, result3;



	zAxis.x = _lookAt.x - _position.x;
	zAxis.y = _lookAt.y - _position.y;
	zAxis.z = _lookAt.z - _position.z;
	
	zAxis.x = zAxis.x / zAxis.length();
	zAxis.y = zAxis.y / zAxis.length();
	zAxis.z = zAxis.z / zAxis.length();


	xAxis.x = (_up.y * zAxis.z) - (_up.z * zAxis.y);
	xAxis.y = (_up.z * zAxis.x) - (_up.x * zAxis.z);
	xAxis.z = (_up.x * zAxis.y) - (_up.y * zAxis.x);
	
	xAxis.x = xAxis.x / xAxis.length();
	xAxis.y = xAxis.y / xAxis.length();
	xAxis.z = xAxis.z / xAxis.length();


	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);


	result1 = ((xAxis.x * _position.x) + (xAxis.y * _position.y) + (xAxis.z * _position.z)) * -1.0f;


	result2 = ((yAxis.x * _position.x) + (yAxis.y * _position.y) + (yAxis.z * _position.z)) * -1.0f;


	result3 = ((zAxis.x * _position.x) + (zAxis.y * _position.y) + (zAxis.z * _position.z)) * -1.0f;


	_viewMatrix[0] = xAxis.x;
	_viewMatrix[1] = yAxis.x;
	_viewMatrix[2] = zAxis.x;
	_viewMatrix[3] = 0.0f;

	_viewMatrix[4] = xAxis.y;
	_viewMatrix[5] = yAxis.y;
	_viewMatrix[6] = zAxis.y;
	_viewMatrix[7] = 0.0f;

	_viewMatrix[8] = xAxis.z;
	_viewMatrix[9] = yAxis.z;
	_viewMatrix[10] = zAxis.z;
	_viewMatrix[11] = 0.0f;

	_viewMatrix[12] = result1;
	_viewMatrix[13] = result2;
	_viewMatrix[14] = result3;
	_viewMatrix[15] = 1.0f;
}

void PerspectiveCamera::BuildPerspectiveFovLHMatrix(float fieldOfView, float screenAspect, float screenNear, float screenDepth)
{
	_projectMatrix[0] = 1.0f / (screenAspect * tan(fieldOfView * 0.5f));
	_projectMatrix[1] = 0.0f;
	_projectMatrix[2] = 0.0f;
	_projectMatrix[3] = 0.0f;

	_projectMatrix[4] = 0.0f;
	_projectMatrix[5] = 1.0f / tan(fieldOfView * 0.5f);
	_projectMatrix[6] = 0.0f;
	_projectMatrix[7] = 0.0f;

	_projectMatrix[8] = 0.0f;
	_projectMatrix[9] = 0.0f;
	_projectMatrix[10] = screenDepth / (screenDepth - screenNear);
	_projectMatrix[11] = 1.0f;

	_projectMatrix[12] = 0.0f;
	_projectMatrix[13] = 0.0f;
	_projectMatrix[14] = (-screenNear * screenDepth) / (screenDepth - screenNear);
	_projectMatrix[15] = 0.0f;
}

void PerspectiveCamera::BuildFrustumMatrix(float left, float right, float top, float bottom, float Near, float Far)
{
	_projectMatrix[0] = Near / right;
	_projectMatrix[1] = 0.0f;
	_projectMatrix[2] = 0.0f;
	_projectMatrix[3] = 0.0f;

	_projectMatrix[4] = 0.0f;
	_projectMatrix[5] = Near / top;
	_projectMatrix[6] = 0.0f;
	_projectMatrix[7] = 0.0f;

	_projectMatrix[8] = 0.0f;
	_projectMatrix[9] = 0.0f;
	_projectMatrix[10] = -(Far + Near) / (Far - Near);
	_projectMatrix[11] = (-2 * Near * Far) / (Far - Near);

	_projectMatrix[12] = 0.0f;
	_projectMatrix[13] = 0.0f;
	_projectMatrix[14] = -1.0f;
	_projectMatrix[15] = 0.0f;
}