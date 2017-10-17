#pragma once
#include <assert.h>
namespace Core {
	class Vector2D
	{
	public:
		double x, y;
	public:
		Vector2D() :x(0), y(0)
		{

		}
		Vector2D(const double& x_, const double& y_) :x(x_), y(y_)
		{

		}
		~Vector2D() = default;
	public:		
		double operator*(const Vector2D& vec) const {
			return x * vec.x + y * vec.y;
		}
		Vector2D operator*(double scaler) const {
			return Vector2D(scaler*x, scaler*y);
		}

		Vector2D operator+ (const Vector2D& vec) const {
			return Vector2D(x + vec.x, y + vec.y);
		}
		Vector2D operator- (const Vector2D& vec) const {
			return Vector2D(x - vec.x, y - vec.y);
		}
		void operator+= (const Vector2D& vec) {
			x += vec.x;
			y += vec.y;
		}
		void operator-= (const Vector2D& vec) {
			x -= vec.x;
			y -= vec.y;
		}
		const double* ptr() const
		{
			return &x;
		}
		static Vector2D lerp(Vector2D v1, Vector2D v2, double scale)
		{
			double one_scale = 1.0 - scale;
			return Vector2D(
				v1.x * one_scale + v2.x * scale,
				v1.y * one_scale + v2.y * scale
			);
		}
		//inline bool operator==(const Vector2D& rhs) const;

	public:
		//useful when you need a triangle's area
		//inline double cross_product(Vector2D& vec_) const;
		//inline double normalize();
		//inline double length() const;
	
	};
}