#pragma once


namespace SRE {

	enum BlendingMode{
		NoBlending = 0,
		NormalBlending = 1,
		AdditiveBlending = 2,
		SubtractiveBlending = 3,											
		MultiplyBlending = 4,																 
		CustomBlending	= 5																				
	};

	enum Side {
		FrontSide = 0,
		BackSide = 1,
		DoubleSide = 2
	};

	enum CompareFunc
	{
		NeverDepth = GL_NEVER,
		AlwaysDepth = GL_ALWAYS,
		LessDepth = GL_LESS,
		LessEqualDepth = GL_LEQUAL,
		GreaterEqualDepth = GL_GEQUAL,
		GreaterDepth = GL_GREATER,
		NotEqualDepth = GL_NOTEQUAL
	};
	enum CullFaceMode
	{
		Front = GL_FRONT,
		Back = GL_BACK,
		DoubleSide = GL_FRONT_AND_BACK
	};

	enum ShadeOptions
	{
		SO_FLAT,
		SO_GOURAUD,
		SO_PHONG
	};
	enum FogMode
	{
		/// No fog. Duh.
		FOG_NONE,
		/// Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
		FOG_EXP,
		/// Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
		FOG_EXP2,
		/// Fog density increases linearly between the start and end distances
		FOG_LINEAR
	};
	

}