#pragma once

#include "Material.h" 
#include "..\Core\HardwareBuffer\ColorValue.h"
#include "..\Core\Texture.h"
namespace SRE {

	class PhongMaterial : public Material {
	public:
		PhongMaterial();
		~PhongMaterial();

	public:

		//should be technique
		TextureUnitState::ptr getMap()const { return _map; }
		void setMap(TextureUnitState::ptr map) { _map = map; }

		MaterialType getType()const { return Phong; }

		void setColor(const ColorValue& color) { _color = color; }
		ColorValue getColor()const { return _color; }

		void setSpecular(const ColorValue& specular) { _specular = specular; }
		ColorValue getSpecular() const { return _specular; }

		void setShininess(const float& shininess) { _shininess = shininess; }
		float getShininess()const { return _shininess; }

		void setLightMap(TextureUnitState::ptr lightMap) { _lightMap = lightMap; }
		TextureUnitState::ptr getLightMap()const { return _lightMap; }

		void setEmissive(const ColorValue& emissive) { _emissive = emissive; }
		ColorValue getEmissive()const { return _emissive; }

		void setDisplacementMap(TextureUnitState::ptr displacementMap) { _displacementMap = displacementMap; }
		TextureUnitState::ptr getDisplacementMap()const { return _displacementMap; }

		void setNormalMap(TextureUnitState::ptr normalMap) { _normalMap = normalMap; }
		TextureUnitState::ptr getNormalMap()const { return _normalMap; }

		

	protected:

		ColorValue _color, _specular, _emissive;
		TextureUnitState::ptr _map, _lightMap, _displacementMap, _normalMap;
		float _shininess;
	}