#pragma once

#include <glew\glew.h>
#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"
#include "../Math/Vector4D.h"
#include "../Math/Matrix3D.h"
#include "../Math/Matrix4D.h"
#include <vector>
#include <memory>
namespace Core {
	class Shader {
	public:
		typedef std::shared_ptr<Shader> ptr;
	public:
		Shader(uint32_t shaderID):shader_ID(shaderID){}
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		Shader(const Shader& shader) {}
		~Shader() {}
	public:
		void use();
		void setBool(const char* name, bool value)const;
		void setInt(const char* name, int value)const;
		void setFloat(const char* name, float value)const;
		void setVec2(const char* name, const Vector2D& value)const;
		void setVec3(const char* name, const Vector3D& value)const;
		void setVec4(const char* name, const Vector4D& value)const;
		//void setMat2(const char* name, const Matrix2D& value)const;
		void setMat3(const char* name, const Matrix3D& value)const;
		void setMat4(const char* name, const Matrix4D& value)const;		
		
	private:
		void checkCompileErrors(GLuint shader, const char* type);
	public:
		unsigned int shader_ID;	
	};
}

