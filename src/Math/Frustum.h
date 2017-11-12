#pragma once
#include "Plane3D.h"
#include "Vector3D.h"
#include "Matrix4D.h"
namespace Math {
	enum CULL_STATE
	{
		cull_outside,
		cull_part_inside,
		cull_all_inside

	};
	class Frustum {

	public:
		Frustum();
		~Frustum();
	public:
		CULL_STATE is_sphere_in_frustum(const Vector3D& center_, double radius_) const;
		void set_frustum(const Matrix4D& proj_view_matrix_, const Vector3D& render_origin_);
	private:
		void update_planes();
	private:
		Matrix4D    _mat_proj_view;
		Vector3D    _render_origin;
		Plane3D     _near_plane;
		Plane3D     _far_plane;
		Plane3D     _left_plane;
		Plane3D     _right_plane;
		Plane3D     _bottom_plane;
		Plane3D     _top_plane;
	};
}