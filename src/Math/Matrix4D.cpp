#include "Matrix4D.h"
#include "Matrix3D.h"
#include "Quaternion.h"
#include "Vector3D.h"
#include "Vector4D.h"
namespace Core {

	const Matrix4D Matrix4D::Zero(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);

	const Matrix4D Matrix4D::Identity(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	Matrix4D::Matrix4D()
	{

	}

	Matrix4D::Matrix4D(const Vector4D& a0_, const Vector4D& a1_, const Vector4D& a2_, const Vector4D& a3_)
	{
		_m[0] = a0_[0];
		_m[1] = a0_[1];
		_m[2] = a0_[2];
		_m[3] = a0_[3];
		_m[4] = a1_[0];
		_m[5] = a1_[1];
		_m[6] = a1_[2];
		_m[7] = a1_[3];
		_m[8] = a2_[0];
		_m[9] = a2_[1];
		_m[10] = a2_[2];
		_m[11] = a2_[3];
		_m[12] = a3_[0];
		_m[13] = a3_[1];
		_m[14] = a3_[2];
		_m[15] = a3_[3];
	}

	Matrix4D::~Matrix4D()
	{

	}

	Matrix4D::Matrix4D(double a00_, double a10_, double a20_, double a30_,
		double a01_, double a11_, double a21_, double a31_,
		double a02_, double a12_, double a22_, double a32_,
		double a03_, double a13_, double a23_, double a33_)
	{
		_m[0] = a00_;
		_m[1] = a10_;
		_m[2] = a20_;
		_m[3] = a30_;
		_m[4] = a01_;
		_m[5] = a11_;
		_m[6] = a21_;
		_m[7] = a31_;
		_m[8] = a02_;
		_m[9] = a12_;
		_m[10] = a22_;
		_m[11] = a32_;
		_m[12] = a03_;
		_m[13] = a13_;
		_m[14] = a23_;
		_m[15] = a33_;
	}

	Matrix4D Matrix4D::getInverse()const
	{
		double a0 = _m[0] * _m[5] - _m[1] * _m[4];
		double a1 = _m[0] * _m[6] - _m[2] * _m[4];
		double a2 = _m[0] * _m[7] - _m[3] * _m[4];
		double a3 = _m[1] * _m[6] - _m[2] * _m[5];
		double a4 = _m[1] * _m[7] - _m[3] * _m[5];
		double a5 = _m[2] * _m[7] - _m[3] * _m[6];
		double b0 = _m[8] * _m[13] - _m[9] * _m[12];
		double b1 = _m[8] * _m[14] - _m[10] * _m[12];
		double b2 = _m[8] * _m[15] - _m[11] * _m[12];
		double b3 = _m[9] * _m[14] - _m[10] * _m[13];
		double b4 = _m[9] * _m[15] - _m[11] * _m[13];
		double b5 = _m[10] * _m[15] - _m[11] * _m[14];

		// Calculate the determinant.
		double det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

		if (fabs(det) < math_tolerance)
		{
			return Identity;
		}
		Matrix4D mat4_;
		mat4_[0] = _m[5] * b5 - _m[6] * b4 + _m[7] * b3;
		mat4_[1] = -_m[1] * b5 + _m[2] * b4 - _m[3] * b3;
		mat4_[2] = _m[13] * a5 - _m[14] * a4 + _m[15] * a3;
		mat4_[3] = -_m[9] * a5 + _m[10] * a4 - _m[11] * a3;

		mat4_[4] = -_m[4] * b5 + _m[6] * b2 - _m[7] * b1;
		mat4_[5] = _m[0] * b5 - _m[2] * b2 + _m[3] * b1;
		mat4_[6] = -_m[12] * a5 + _m[14] * a2 - _m[15] * a1;
		mat4_[7] = _m[8] * a5 - _m[10] * a2 + _m[11] * a1;

		mat4_[8] = _m[4] * b4 - _m[5] * b2 + _m[7] * b0;
		mat4_[9] = -_m[0] * b4 + _m[1] * b2 - _m[3] * b0;
		mat4_[10] = _m[12] * a4 - _m[13] * a2 + _m[15] * a0;
		mat4_[11] = -_m[8] * a4 + _m[9] * a2 - _m[11] * a0;

		mat4_[12] = -_m[4] * b3 + _m[5] * b1 - _m[6] * b0;
		mat4_[13] = _m[0] * b3 - _m[1] * b1 + _m[2] * b0;
		mat4_[14] = -_m[12] * a3 + _m[13] * a1 - _m[14] * a0;
		mat4_[15] = _m[8] * a3 - _m[9] * a1 + _m[10] * a0;
		mat4_ = mat4_*(1.0 / det);
		return mat4_;
	}	

	const double* Matrix4D::getMatrix() const
	{
		return _m;
	}

	Matrix3D Matrix4D::getMatrix3() const
	{
		Matrix3D m3x3;
		m3x3.m[0][0] = m[0][0];
		m3x3.m[0][1] = m[0][1];
		m3x3.m[0][2] = m[0][2];
		m3x3.m[1][0] = m[1][0];
		m3x3.m[1][1] = m[1][1];
		m3x3.m[1][2] = m[1][2];
		m3x3.m[2][0] = m[2][0];
		m3x3.m[2][1] = m[2][1];
		m3x3.m[2][2] = m[2][2];
		return m3x3;

	}

	void Matrix4D::decompose(Vector3D& position, Vector3D& scale, Quaternion& orientation)
	{
		double lx = Vector3D(m[0][0], m[0][1], m[0][2]).length();
		double ly = Vector3D(m[1][0], m[1][1], m[1][2]).length();
		double lz = Vector3D(m[2][0], m[2][1], m[2][2]).length();

		double det = determinant();
		if (det < 0)
			lx = -lx;
		position.x = m[3][0];
		position.y = m[3][1];
		position.z = m[3][2];

		Matrix3D matrix = this->getMatrix3();
		
		double invLx = 1 / lx;
		double invLy = 1 / ly;
		double invLz = 1 / lz;

		matrix[0][1] *= invLx;
		matrix[0][2] *= invLx;
		matrix[0][3] *= invLx;

		matrix[1][4] *= invLy;
		matrix[1][5] *= invLy;
		matrix[1][6] *= invLy;

		matrix[2][7] *= invLz;
		matrix[2][8] *= invLz;
		matrix[2][9] *= invLz;

		orientation.fromRotationMatrix(matrix);

		scale.x = lx;
		scale.y = ly;
		scale.z = lz;

	}

	Vector4D Matrix4D::operator*(const Vector4D& v) const
	{
		return Vector4D(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
			m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
	}
	
	Vector3D Matrix4D::operator*(const Vector3D& v) const
	{
		Vector3D r;

		double fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);

		r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
		r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
		r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;

		return r;
	}

	Matrix4D Matrix4D::operator*(const Matrix4D& m2) const
	{
		return Matrix4D(
			 m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0],
			 m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1],
			 m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2],
			 m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3],

			 m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0],
			 m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1],
			 m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2],
			 m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3],

			 m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0],
			 m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1],
			 m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2],
			 m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3],

			 m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0],
			 m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1],
			 m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2],
			 m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3]
		);
	}

	inline void Matrix4D::operator = (const Matrix3D& mat3)
	{
		m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
		m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
		m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
	}

	double Matrix4D::determinant() const
	{
		double a0 = _m[0] * _m[5] - _m[1] * _m[4];
		double a1 = _m[0] * _m[6] - _m[2] * _m[4];
		double a2 = _m[0] * _m[7] - _m[3] * _m[4];
		double a3 = _m[1] * _m[6] - _m[2] * _m[5];
		double a4 = _m[1] * _m[7] - _m[3] * _m[5];
		double a5 = _m[2] * _m[7] - _m[3] * _m[6];
		double b0 = _m[8] * _m[13] - _m[9] * _m[12];
		double b1 = _m[8] * _m[14] - _m[10] * _m[12];
		double b2 = _m[8] * _m[15] - _m[11] * _m[12];
		double b3 = _m[9] * _m[14] - _m[10] * _m[13];
		double b4 = _m[9] * _m[15] - _m[11] * _m[13];
		double b5 = _m[10] * _m[15] - _m[11] * _m[14];
		double det = a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0;
		return det;
	}

	Matrix4D Matrix4D::getTranspose() const
	{
		return Matrix4D(
			_m[0], _m[4], _m[8], _m[12],
			_m[1], _m[5], _m[9], _m[13],
			_m[2], _m[6], _m[10], _m[14],
			_m[3], _m[7], _m[11], _m[15]
		);
	}
	
	Matrix4D Matrix4D::makeTransformMatrix(const Vector3D& position, const Vector3D& scale, const Quaternion& orientation)
	{
		// Ordering:
		//    1. Scale
		//    2. Rotate
		//    3. Translate
		Matrix4D r;
		Matrix3D rot3x3;
		orientation.toRotationMatrix(rot3x3);
		
		r.m[0][0] = scale.x * rot3x3[0][0]; r.m[0][1] = scale.y * rot3x3[0][1]; r.m[0][2] = scale.z * rot3x3[0][2]; r.m[0][3] = position.x;
		r.m[1][0] = scale.x * rot3x3[1][0]; r.m[1][1] = scale.y * rot3x3[1][1]; r.m[1][2] = scale.z * rot3x3[1][2]; r.m[1][3] = position.y;
		r.m[2][0] = scale.x * rot3x3[2][0]; r.m[2][1] = scale.y * rot3x3[2][1]; r.m[2][2] = scale.z * rot3x3[2][2]; r.m[2][3] = position.z;

		// No projection term
		r.m[3][0] = 0; r.m[3][1] = 0; r.m[3][2] = 0; r.m[3][3] = 1;

		return r;
	}

	Matrix4D Matrix4D::makeRotationMatrix(const Vector3D& axis, double angle_in_rad_)
	{

		double x = axis[0];
		double y = axis[1];
		double z = axis[2];

		// Make sure the input axis is normalized.
		double n = x*x + y*y + z*z;
		if (n != 1.0)
		{
			// Not normalized.
			n = sqrt(n);
			// Prevent divide too close to zero.
			if (n > 0.000001)
			{
				n = 1.0f / n;
				x *= n;
				y *= n;
				z *= n;
			}
		}

		double c = cos(angle_in_rad_);
		double s = sin(angle_in_rad_);

		double t_ = 1.0f - c;
		double tx = t_ * x;
		double ty = t_ * y;
		double tz = t_ * z;
		double txy = tx * y;
		double txz = tx * z;
		double tyz = ty * z;
		double sx = s * x;
		double sy = s * y;
		double sz = s * z;

		Matrix4D dst;
		dst[0] = c + tx*x;
		dst[1] = txy + sz;
		dst[2] = txz - sy;
		dst[3] = 0.0f;

		dst[4] = txy - sz;
		dst[5] = c + ty*y;
		dst[6] = tyz + sx;
		dst[7] = 0.0f;

		dst[8] = txz + sy;
		dst[9] = tyz - sx;
		dst[10] = c + tz*z;
		dst[11] = 0.0f;

		dst[12] = 0.0f;
		dst[13] = 0.0f;
		dst[14] = 0.0f;
		dst[15] = 1.0f;
		return dst;
	}

	Matrix4D Matrix4D::makeTranslateMatrix(const Vector3D& trans_)
	{
		Matrix4D r;

		r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = trans_.x;
		r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = trans_.y;
		r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = trans_.z;
		r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

		return r;
	}

	Matrix4D Matrix4D::makeScaleMatrix(const Vector3D& scale_)
	{
		Matrix4D r;
		r.m[0][0] = scale_.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
		r.m[1][0] = 0.0; r.m[1][1] = scale_.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
		r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = scale_.z; r.m[2][3] = 0.0;
		r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

		return r;
	}

	Matrix4D Matrix4D::createReflection(const Vector3D& normal_, double dist_to_origin_)
	{
		double k = -2.0f * dist_to_origin_;

		double nxy2 = -2 * normal_[0] * normal_[1];
		double nxz2 = -2 * normal_[0] * normal_[2];
		double nyz2 = -2 * normal_[1] * normal_[2];
		double one_xx2 = 1.0f - 2.0f*normal_[0] * normal_[0];
		double one_yy2 = 1.0f - 2.0f*normal_[1] * normal_[1];
		double one_zz2 = 1.0f - 2.0f*normal_[2] * normal_[2];
		return Matrix4D(
			one_xx2, nxy2, nxz2, k*normal_[0],
			nxy2, one_yy2, nyz2, k*normal_[1],
			nxz2, nyz2, one_zz2, k*normal_[2],
			0, 0, 0, 1.0
		);
	}

	Matrix4D Matrix4D::makeViewMatrix(const Vector3D& pos_, const Vector3D& target_, const Vector3D& up_)
	{
		Vector3D look = pos_ - target_;
		look.normalize();
		Vector3D up(up_);
		up.normalize();
		Vector3D right = up.cross(look);
		right.normalize();
		up = look.cross(right);
		return Matrix4D(
			right[0], up[0], look[0], 0,
			right[1], up[1], look[1], 0,
			right[2], up[2], look[2], 0,
			-(right.dot(pos_)), -(up.dot(pos_)), -(look.dot(pos_)), 1
		);
	}

	Matrix4D Matrix4D::makeProjectionMatrix(double view_degree_rad_, double aspect_ratio_, double near_distance_, double far_distance_)
	{
		double f_n = 1.0 / (far_distance_ - near_distance_);
		double theta = view_degree_rad_ * 0.5;

		double divisor = tan(theta);
		double factor = 1.0 / divisor;

		return Matrix4D(
			(1.0 / aspect_ratio_) * factor, 0, 0, 0,
			0, factor, 0, 0,
			0, 0, (-(far_distance_ + near_distance_)) * f_n, -1.0,
			0, 0, -2.0 * far_distance_ * near_distance_ * f_n, 0
		);
	}

	Matrix4D Matrix4D::makeOrthoMatrix(double width_, double height_, double near_dist_, double far_dist_)
	{
		double f_n = 1.0 / (far_dist_ - near_dist_);
		return Matrix4D(
			2.0 / width_, 0, 0, 0,
			0, 2.0 / height_, 0, 0,
			0, 0, -2.0*f_n, 0,
			0, 0, -(far_dist_ + near_dist_)*f_n, 1.0
		);
	}

	Matrix4D Matrix4D::makeOrthoMatrix(double left, double right, double bottom, double top, double n, double f)
	{
		Matrix4D OP;

		OP[0] = 2.0f / (right - left);
		OP[1] = 0.0f;
		OP[2] = 0.0f;
		OP[3] = 0.0f;

		OP[4] = 0.0f;
		OP[5] = 2.0f / (top - bottom);
		OP[6] = 0.0f;
		OP[7] = 0.0f;

		OP[8] = 0.0f;
		OP[9] = 0.0f;
		OP[10] = -2.0f / (f - n);
		OP[11] = 0.0f;

		OP[12] = -(right + left) / (right - left);
		OP[13] = -(top + bottom) / (top - bottom);
		OP[14] = -(f + n) / (f - n);
		OP[15] = 1.0f;

		return OP;
	}
}