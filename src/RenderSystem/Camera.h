#ifndef Camera_H
#define Camera_H

#include "Vector3.h"
#include "Matrix4.h"
class Camera{
public:
	Camera(){}
	~Camera(){}
public:
	virtual void setCamera(const Vector3&, const Vector3&, const Vector3&) = 0;
	virtual void setPosition(const Vector3&) = 0;
	virtual void setLookAt(const Vector3&) = 0;
	virtual void setUp(const Vector3&) = 0;
	virtual const Vector3& getPosition() = 0;	
	virtual const Vector3& getLookAt() = 0;		
	virtual const Vector3& getUp() = 0;
	//virtual const Vector3& getRight() = 0;

	virtual void updataVeiwMatrix() = 0;
	
	//virtual void roll(const float& angle) = 0;
	//virtual void yaw(const float& angle) = 0;
	//virtual void pitch(const float& angle) = 0;

	//virtual void rotate(const Vector3& axis, const float& angle) = 0;

	virtual const Matrix4& getViewMatrix() = 0;
	virtual const Matrix4& getProjectMatrix() = 0;

	virtual void BuildPerspectiveFovLHMatrix(float, float, float, float) = 0;
	virtual void BuildFrustumMatrix(float, float, float, float, float, float) = 0;

};
class PerspectiveCamera :public Camera{
public:
	//PerspectiveCamera(float,float,float,float);
	//PerspectiveCamera(const PerspectiveCamera&);
	PerspectiveCamera() {}
	virtual ~PerspectiveCamera() {}
public:
	virtual void setCamera(const Vector3&, const Vector3&, const Vector3&);
	virtual void setPosition(const Vector3&);
	virtual void setLookAt(const Vector3&);
	virtual void setUp(const Vector3&);
	virtual const Vector3& getPosition();
	virtual const Vector3& getLookAt();
	virtual const Vector3& getUp();
	//virtual const Vector3& getRight();

	virtual void updataVeiwMatrix();
	
	//virtual void roll(const float& angle);
	//virtual void yaw(const float& angle);
	//virtual void pitch(const float& angle);
	//virtual void rotate(const Vector3& axis, const float& angle);

	virtual const Matrix4& getViewMatrix();
	virtual const Matrix4& getProjectMatrix();
public:
	 void BuildPerspectiveFovLHMatrix(float, float, float, float);
	 void BuildFrustumMatrix(float, float, float, float, float, float);


private:
	Vector3 _position, _lookAt, _up;
	Matrix4 _viewMatrix;
	Matrix4 _projectMatrix;
};
#endif