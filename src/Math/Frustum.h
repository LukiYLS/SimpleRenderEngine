#pragma once
#include "Plane3D.h"
#include "Vector3D.h"
#include "Matrix4D.h"
#include "..\Utils\BoundingSphere.h"
using namespace Utils;
namespace Math {
	enum CULL_STATE
	{
		cull_outside,
		cull_part_inside,
		cull_all_inside

	};
	class Frustum {

	public:
		Frustum() {}
		~Frustum() {}
	public:
		//CULL_STATE is_sphere_in_frustum(const Vector3D& center_, double radius_) const;
		//void set_frustum(const Matrix4D& proj_view_matrix_, const Vector3D& render_origin_);
		void setFromMatrix(const Matrix4D& matrix);
		bool intersectsSphere(BoundingSphere& sphere);
	protected:

		Plane3D _planes[6];

	};
}