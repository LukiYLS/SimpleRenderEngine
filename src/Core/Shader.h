#ifndef SHADER_H
#define SHADER_H
#include "Uniform.h"
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include <memory>
namespace Core {
	class Shader {
	public:
		typedef std::shared_ptr<Shader> ptr;
	public:
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		Shader(const Shader& shader) {}
		~Shader() {}
	public:
		void use();
		void setBool(const char* name, bool value)const;
		void setInt(const char* name, int value)const;
		void setFloat(const char* name, float value)const;
		void setVec2(const char* name, const glm::vec2& value)const;
		void setVec3(const char* name, const glm::vec3& value)const;
		void setVec4(const char* name, const glm::vec4& value)const;
		void setMat2(const char* name, const glm::mat2& value)const;
		void setMat3(const char* name, const glm::mat3& value)const;
		void setMat4(const char* name, const glm::mat4& value)const;
		
		//void addUniform(Uniform<> )
	private:
		void checkCompileErrors(GLuint shader, const char* type);
	public:
		unsigned int shader_ID;
		//std::vector<Uniform<T>> _uniforms;
	};
}

#endif
