#ifndef UNIFORM_H
#define UNIFORM_H
#include <glew\glew.h>
#include <memory>
namespace Core {
	//这个类有必要吗？？
	/*enum Type {
		FLOAT = GL_FLOAT,
		FLOAT_VEC2 = GL_FLOAT_VEC2,
		FLOAT_VEC3 = GL_FLOAT_VEC3,
		FLOAT_VEC4 = GL_FLOAT_VEC4,

		DOUBLE = GL_DOUBLE,
		DOUBLE_VEC2 = GL_DOUBLE_VEC2,
		DOUBLE_VEC3 = GL_DOUBLE_VEC3,
		DOUBLE_VEC4 = GL_DOUBLE_VEC4,

		INT = GL_INT,
		INT_VEC2 = GL_INT_VEC2,
		INT_VEC3 = GL_INT_VEC3,
		INT_VEC4 = GL_INT_VEC4,

		UNSIGNED_INT = GL_UNSIGNED_INT,
		UNSIGNED_INT_VEC2 = GL_UNSIGNED_INT_VEC2_EXT,
		UNSIGNED_INT_VEC3 = GL_UNSIGNED_INT_VEC3_EXT,
		UNSIGNED_INT_VEC4 = GL_UNSIGNED_INT_VEC4_EXT,

		BOOL = GL_BOOL,
		BOOL_VEC2 = GL_BOOL_VEC2,
		BOOL_VEC3 = GL_BOOL_VEC3,
		BOOL_VEC4 = GL_BOOL_VEC4,

		FLOAT_MAT2 = GL_FLOAT_MAT2,
		FLOAT_MAT3 = GL_FLOAT_MAT3,
		FLOAT_MAT4 = GL_FLOAT_MAT4,
		FLOAT_MAT2x3 = GL_FLOAT_MAT2x3,
		FLOAT_MAT2x4 = GL_FLOAT_MAT2x4,
		FLOAT_MAT3x2 = GL_FLOAT_MAT3x2,
		FLOAT_MAT3x4 = GL_FLOAT_MAT3x4,
		FLOAT_MAT4x2 = GL_FLOAT_MAT4x2,
		FLOAT_MAT4x3 = GL_FLOAT_MAT4x3,

		DOUBLE_MAT2 = GL_DOUBLE_MAT2,
		DOUBLE_MAT3 = GL_DOUBLE_MAT3,
		DOUBLE_MAT4 = GL_DOUBLE_MAT4,
		DOUBLE_MAT2x3 = GL_DOUBLE_MAT2x3,
		DOUBLE_MAT2x4 = GL_DOUBLE_MAT2x4,
		DOUBLE_MAT3x2 = GL_DOUBLE_MAT3x2,
		DOUBLE_MAT3x4 = GL_DOUBLE_MAT3x4,
		DOUBLE_MAT4x2 = GL_DOUBLE_MAT4x2,
		DOUBLE_MAT4x3 = GL_DOUBLE_MAT4x3,

		SAMPLER_1D = GL_SAMPLER_1D,
		SAMPLER_2D = GL_SAMPLER_2D,
		SAMPLER_3D = GL_SAMPLER_3D,
		SAMPLER_CUBE = GL_SAMPLER_CUBE,
		SAMPLER_1D_SHADOW = GL_SAMPLER_1D_SHADOW,
		SAMPLER_2D_SHADOW = GL_SAMPLER_2D_SHADOW,
		SAMPLER_1D_ARRAY = GL_SAMPLER_1D_ARRAY_EXT,
		SAMPLER_2D_ARRAY = GL_SAMPLER_2D_ARRAY_EXT,
		SAMPLER_CUBE_MAP_ARRAY = GL_SAMPLER_CUBE_MAP_ARRAY,
		SAMPLER_1D_ARRAY_SHADOW = GL_SAMPLER_1D_ARRAY_SHADOW_EXT,
		SAMPLER_2D_ARRAY_SHADOW = GL_SAMPLER_2D_ARRAY_SHADOW_EXT,
		SAMPLER_2D_MULTISAMPLE = GL_SAMPLER_2D_MULTISAMPLE,
		SAMPLER_2D_MULTISAMPLE_ARRAY = GL_SAMPLER_2D_MULTISAMPLE_ARRAY,
		SAMPLER_CUBE_SHADOW = GL_SAMPLER_CUBE_SHADOW_EXT,
		SAMPLER_CUBE_MAP_ARRAY_SHADOW = GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW,
		SAMPLER_BUFFER = GL_SAMPLER_BUFFER_EXT,
		SAMPLER_2D_RECT = GL_SAMPLER_2D_RECT,
		SAMPLER_2D_RECT_SHADOW = GL_SAMPLER_2D_RECT_SHADOW,
	};*/
	/*template<class T>
	class Uniform {
	public:		
	public:
		Uniform(const char* name, T value, Type type) :
			name(name), value(value), type(type) {}
	public:
		const char* name;
		Type type;
		T value;

	};*/
}
#endif // !UNIFORM_H

