#include "Quaternion.h"
#include "Matrix3D.h"
#include "Vector3D.h"
#include <math.h>
namespace Core {

	void Quaternion::fromRotationMatrix(const Matrix3D& rot)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		double fTrace = rot[0][0] + rot[1][1] + rot[2][2];
		double fRoot;

		if (fTrace > 0.0)
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = sqrt(fTrace + 1.0f);  // 2w
			w = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;  // 1/(4w)
			x = (rot[2][1] - rot[1][2])*fRoot;
			y = (rot[0][2] - rot[2][0])*fRoot;
			z = (rot[1][0] - rot[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if (rot[1][1] > rot[0][0])
				i = 1;
			if (rot[2][2] > rot[i][i])
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = sqrt(rot[i][i] - rot[j][j] - rot[k][k] + 1.0f);
			double* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			w = (rot[k][j] - rot[j][k])*fRoot;
			*apkQuat[j] = (rot[j][i] + rot[i][j])*fRoot;
			*apkQuat[k] = (rot[k][i] + rot[i][k])*fRoot;
		}		
	}
	void Quaternion::toRotationMatrix(Matrix3D& rot) const
	{
		double fTx = x + x;
		double fTy = y + y;
		double fTz = z + z;
		double fTwx = fTx*w;
		double fTwy = fTy*w;
		double fTwz = fTz*w;
		double fTxx = fTx*x;
		double fTxy = fTy*x;
		double fTxz = fTz*x;
		double fTyy = fTy*y;
		double fTyz = fTz*y;
		double fTzz = fTz*z;

		rot[0][0] = 1.0f - (fTyy + fTzz);
		rot[0][1] = fTxy - fTwz;
		rot[0][2] = fTxz + fTwy;
		rot[1][0] = fTxy + fTwz;
		rot[1][1] = 1.0f - (fTxx + fTzz);
		rot[1][2] = fTyz - fTwx;
		rot[2][0] = fTxz - fTwy;
		rot[2][1] = fTyz + fTwx;
		rot[2][2] = 1.0f - (fTxx + fTyy);
	}
	void Quaternion::fromAngleAxis(const double& angle, const Vector3D& axis)
	{
		double fHalfAngle(0.5*angle);
		double fSin =sin(fHalfAngle);
		w = cos(fHalfAngle);
		x = fSin*axis.x;
		y = fSin*axis.y;
		z = fSin*axis.z;
	}

	//-----------------------------------------------------------------------
	float InvSqrt(float x)
	{
		float xhalf = 0.5f*x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1);
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x);
		return x;
	}
	void Quaternion::toAngleAxis(double& angle, Vector3D& rkAxis) const
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		double fSqrLength = x*x + y*y + z*z;
		if (fSqrLength > 0.0)
		{
			angle = 2.0*acos(w);
			double fInvLength = InvSqrt(fSqrLength);
			rkAxis.x = x*fInvLength;
			rkAxis.y = y*fInvLength;
			rkAxis.z = z*fInvLength;
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			angle = double(0.0);
			rkAxis.x = 1.0;
			rkAxis.y = 0.0;
			rkAxis.z = 0.0;
		}
	}
	//-----------------------------------------------------------------------
	void Quaternion::fromAxes(const Vector3D* axis)
	{
		Matrix3D kRot;

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			kRot[0][iCol] = axis[iCol].x;
			kRot[1][iCol] = axis[iCol].y;
			kRot[2][iCol] = axis[iCol].z;
		}

		fromRotationMatrix(kRot);
	}
	//-----------------------------------------------------------------------
	void Quaternion::fromAxes(const Vector3D& xaxis, const Vector3D& yaxis, const Vector3D& zaxis)
	{
		Matrix3D kRot;

		kRot[0][0] = xaxis.x;
		kRot[1][0] = xaxis.y;
		kRot[2][0] = xaxis.z;

		kRot[0][1] = yaxis.x;
		kRot[1][1] = yaxis.y;
		kRot[2][1] = yaxis.z;

		kRot[0][2] = zaxis.x;
		kRot[1][2] = zaxis.y;
		kRot[2][2] = zaxis.z;

		fromRotationMatrix(kRot);

	}
	//-----------------------------------------------------------------------
	void Quaternion::toAxes(Vector3D* akAxis) const
	{
		Matrix3D kRot;

		toRotationMatrix(kRot);

		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			akAxis[iCol].x = kRot[0][iCol];
			akAxis[iCol].y = kRot[1][iCol];
			akAxis[iCol].z = kRot[2][iCol];
		}
	}
	//-----------------------------------------------------------------------
	Vector3D Quaternion::xAxis(void) const
	{
		//double fTx  = 2.0*x;
		double fTy = 2.0f*y;
		double fTz = 2.0f*z;
		double fTwy = fTy*w;
		double fTwz = fTz*w;
		double fTxy = fTy*x;
		double fTxz = fTz*x;
		double fTyy = fTy*y;
		double fTzz = fTz*z;

		return Vector3D(1.0f - (fTyy + fTzz), fTxy + fTwz, fTxz - fTwy);
	}
	//-----------------------------------------------------------------------
	Vector3D Quaternion::yAxis(void) const
	{
		double fTx = 2.0f*x;
		double fTy = 2.0f*y;
		double fTz = 2.0f*z;
		double fTwx = fTx*w;
		double fTwz = fTz*w;
		double fTxx = fTx*x;
		double fTxy = fTy*x;
		double fTyz = fTz*y;
		double fTzz = fTz*z;

		return Vector3D(fTxy - fTwz, 1.0f - (fTxx + fTzz), fTyz + fTwx);
	}
	//-----------------------------------------------------------------------
	Vector3D Quaternion::zAxis(void) const
	{
		double fTx = 2.0f*x;
		double fTy = 2.0f*y;
		double fTz = 2.0f*z;
		double fTwx = fTx*w;
		double fTwy = fTy*w;
		double fTxx = fTx*x;
		double fTxz = fTz*x;
		double fTyy = fTy*y;
		double fTyz = fTz*y;

		return Vector3D(fTxz + fTwy, fTyz - fTwx, 1.0f - (fTxx + fTyy));
	}
	//-----------------------------------------------------------------------
	void Quaternion::toAxes(Vector3D& xaxis, Vector3D& yaxis, Vector3D& zaxis) const
	{
		Matrix3D kRot;

		toRotationMatrix(kRot);

		xaxis.x = kRot[0][0];
		xaxis.y = kRot[1][0];
		xaxis.z = kRot[2][0];

		yaxis.x = kRot[0][1];
		yaxis.y = kRot[1][1];
		yaxis.z = kRot[2][1];

		zaxis.x = kRot[0][2];
		zaxis.y = kRot[1][2];
		zaxis.z = kRot[2][2];
	}

	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator+ (const Quaternion& rkQ) const
	{
		return Quaternion(w + rkQ.w, x + rkQ.x, y + rkQ.y, z + rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator- (const Quaternion& rkQ) const
	{
		return Quaternion(w - rkQ.w, x - rkQ.x, y - rkQ.y, z - rkQ.z);
	}
	//-----------------------------------------------------------------------
	Quaternion Quaternion::operator* (const Quaternion& rkQ) const
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return Quaternion
		(
			w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
			w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
			w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
			w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
		);
	}

	Vector3D Quaternion::operator* (const Vector3D& v) const
	{
		// nVidia SDK implementation
		Vector3D uv, uuv;
		Vector3D qvec(x, y, z);
		uv = qvec.cross(v);
		uuv = qvec.cross(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return v + uv + uuv;

	}

	double Quaternion::getRoll(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// roll = atan2(localx.y, localx.x)
			// pick parts of xAxis() implementation that we need
			//			double fTx  = 2.0*x;
			double fTy = 2.0f*y;
			double fTz = 2.0f*z;
			double fTwz = fTz*w;
			double fTxy = fTy*x;
			double fTyy = fTy*y;
			double fTzz = fTz*z;

			// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

			return double(atan2(fTxy + fTwz, 1.0f - (fTyy + fTzz)));

		}
		else
		{
			return double(atan2(2 * (x*y + w*z), w*w + x*x - y*y - z*z));
		}
	}
	//-----------------------------------------------------------------------
	double Quaternion::getPitch(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// pitch = atan2(localy.z, localy.y)
			// pick parts of yAxis() implementation that we need
			double fTx = 2.0f*x;
			//			double fTy  = 2.0f*y;
			double fTz = 2.0f*z;
			double fTwx = fTx*w;
			double fTxx = fTx*x;
			double fTyz = fTz*y;
			double fTzz = fTz*z;

			// Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
			return double(atan2(fTyz + fTwx, 1.0f - (fTxx + fTzz)));
		}
		else
		{
			// internal version
			return double(atan2(2 * (y*z + w*x), w*w - x*x - y*y + z*z));
		}
	}
	//-----------------------------------------------------------------------
	double Quaternion::getYaw(bool reprojectAxis) const
	{
		if (reprojectAxis)
		{
			// yaw = atan2(localz.x, localz.z)
			// pick parts of zAxis() implementation that we need
			double fTx = 2.0f*x;
			double fTy = 2.0f*y;
			double fTz = 2.0f*z;
			double fTwy = fTy*w;
			double fTxx = fTx*x;
			double fTxz = fTz*x;
			double fTyy = fTy*y;

			// Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

			return double(atan2(fTxz + fTwy, 1.0f - (fTxx + fTyy)));

		}
		else
		{
			// internal version
			return double(asin(-2 * (x*z - w*y)));
		}
	}

	double Quaternion::normalise(void)
	{
		double len = norm();
		double factor = 1.0f / sqrt(len);
		*this = *this * factor;
		return len;
	}
	
}