//******************************
//支持欧拉变换，第一人称视角，以及缩放
//******************************
#ifndef CAMERA_H
#define CAMERA_H
#include <glm\glm.hpp>
#include <memory>
namespace RenderSystem {
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera {
	public:
		typedef std::shared_ptr<Camera> ptr;
	public:		
		Camera(const glm::vec3& eye = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& center = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
	public:

		//第一人称
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		//欧拉变换
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		// 缩放
		void ProcessMouseScroll(float yoffset);
		

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectMatrix();
		void setPerspectiveFovLHMatrix(float fovy, float aspect,float near,float far);

	private:
		void updateCamera();
	public:
		glm::vec3 Eye;
		glm::vec3 Center;
		glm::vec3 Up;

	protected:
		glm::vec3 _front;		
		glm::vec3 _right;
		glm::vec3 _worldUp;
		float _yaw;
		float _pitch;		
		float _movementSpeed;
		float _mouseSensitivity;
	protected:
		float _fovy;
		float _aspect;
		float _near;
		float _far;

	};
}
#endif
