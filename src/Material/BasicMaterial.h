#pragma once


#include "Material.h"

namespace SRE {

	class BasicMaterial : public Material {

	public:

		virtual BasicMaterial* asBasicMaterial() { return this; }
	};
}