#pragma once
#include "../Math/Vector2D.h"
#include "../Math/Vector3D.h"
#include "../Math/Vector4D.h"
#include "../Math/Matrix3D.h"
#include "../Math/Matrix4D.h"
#include <string>
using std::string;

namespace SRE {



	class StringHelper {

	public:
		//convert float to string,
		static string toString(float val, unsigned short precision = 6,	unsigned short width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		//convert double to string
		static string toString(double val, unsigned short precision = 6, unsigned short width = 0, char fill = ' ',	std::ios::fmtflags flags = std::ios::fmtflags(0));
		//convert an unsigned int to a string.
		static string toString(unsigned int val, unsigned short width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		// convert an unsigned long to a string.
		static string toString(unsigned long val, unsigned short width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
				
		static string toString(const Math::Vector2D& val);
		
		static string toString(const Math::Vector3D& val);
		
		static string toString(const Math::Vector4D& val);
		
		static string toString(const Math::Matrix3D& val);
		
		static string toString(const Math::Matrix4D& val);
		
	};

}