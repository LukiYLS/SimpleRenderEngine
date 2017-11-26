#include "PhongMaterial.h"

namespace SRE {

	PhongMaterial::PhongMaterial()
	:_color(ColorValue(1.0,1.0,1.0,1.0)),
	 _specular(ColorValue(0.1, 0.1, 0.1, 1.0)),
	 _emissive(ColorValue(0.0, 0.0, 0.0, 1.0)),
	 _lightMap(NULL),
	 _displacementMap(NULL),
	 _normalMap(NULL),
	 _shininess(30),
	 _lineWidth(1.0),
	 _wireframe(false)	
	 {
	}
}