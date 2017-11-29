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
	
	const char* vertex_attribute = "layout(location=0)in vec3 position;\nlayout(location =1) in vec3 normal;\nlayout(location=2)in vec3 color;\nlayout(location=3)in vec2 texCoord;\nuniform mat4 modelMatrix;\nuniform mat4 viewMatrix;\nuniform mat4 projectionMatrix;\n";

	const char* uv_vertex_define = "#if define (USE_MAP) || define (USE_NORMAL_MAP)\nvaying vec2 uv;\nuniform vec3 uvTransform;\n#endif";

	const char* uv2_vertex_define = "#if define(USE_LIGHT_MAP) || define(USE_ AOMAP)\nvarying vec2 uv2;\n#endif";

	const char* envmap_vertex_define = "#ifdef USE_ENVMAP\n#if defined(USE_BUMPMAP) || defined(USE_NORMALMAP) || defined(PHONG)\n\tvarying vec3 worldPosition;\n#else\n\tvarying vec3 reflect;\n\tuniform float refractionRatio;\n\t#endif\n#endif\n";

	const char* color_vertex_define = "#ifdef USE_COLOR\n\tvarying vec3 Color;\n#endif\n";

	const char* uv_vertex = "#if define (USE_MAP) || define (USE_NORMAL_MAP)\nuv = (uvTransform * vec3(uv, 1)).xy;\n#endif";

	const char* uv2_vertex = "#if define(USE_LIGHT_MAP) || define(USE_ AOMAP)\nuv2 = uv2;\n#endif";

	const char* color_vertex = "#ifdef USE_COLOR\n\tColor = color;\n#endif\n";

}