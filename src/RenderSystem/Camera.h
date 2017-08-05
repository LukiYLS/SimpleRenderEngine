#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
namespace RenderSystem {
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	//默认设置
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera {
	public:
		Camera(const glm::vec3& pos,const glm::vec3& lookat,const glm::vec3& up);
	public:

		//键盘控制前后左右
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		//鼠标控制旋转
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		// 放大缩小
		void ProcessMouseScroll(float yoffset);
		

		glm::mat4 getViewMatrix();
		glm::mat4 BuildPerspectiveFovLHMatrix(float angle, float fov,float near,float far);


	public:
		glm::vec3 Position;
		glm::vec3 LookAt;
		glm::vec3 Up;

	protected:
		glm::vec3 _front;		
		glm::vec3 _right;
		glm::vec3 _worldUp;
		float _yaw;
		float _pitch;		
		float _movementSpeed;
		float _mouseSensitivity;
		float _zoom;

	};
}
#endif
/*
class Camera{
public:
	Camera(){}
	~Camera(){}
public:
	virtual void setCamera(const glm::vec3&, const glm::vec3&, const glm::vec3&) = 0;
	virtual void setPosition(const glm::vec3&) = 0;
	virtual void setLookAt(const glm::vec3&) = 0;
	virtual void setUp(const glm::vec3&) = 0;
	virtual const glm::vec3& getPosition() = 0;	
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