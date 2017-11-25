#pragma once


namespace SRE {

	class TextureUnit {
	public:

		enum TextureAddressingMode
		{
			/// Texture wraps at values over 1.0.
			TAM_WRAP,
			/// Texture mirrors (flips) at joins over 1.0.
			TAM_MIRROR,
			/// Texture clamps at 1.0.
			TAM_CLAMP,
			/// Texture coordinates outside the range [0.0, 1.0] are set to the border colour.
			TAM_BORDER,
			/// Unknown
			TAM_UNKNOWN = 99
		};
		enum TextureCubeFace
		{
			CUBE_FRONT = 0,
			CUBE_BACK = 1,
			CUBE_LEFT = 2,
			CUBE_RIGHT = 3,
			CUBE_UP = 4,
			CUBE_DOWN = 5
		};


	};
}