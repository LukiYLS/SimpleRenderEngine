#include "MathHelper.h"

namespace Math {



	Vector3D MathHelper::unProject(const Matrix4D& view, const Matrix4D& projection, const Vector3D& vec)
	{
		Matrix4D vpinverse = view.getInverse() * projection.getInverse();
		return vpinverse * vec;
	}
	Vector3D MathHelper::project(const Matrix4D& view, const Matrix4D& projection, const Vector3D& vec)
	{
		return projection * view * vec;
	}
}