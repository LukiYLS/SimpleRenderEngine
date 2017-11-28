#pragma once

#include "..\Core\Object.h"
#include "..\Core\HardwareBuffer\ColorValue.h"
namespace SRE {

	
	class Light : public Object{

	public:

		enum LightType {

			DirectionLightType,
			PointLightType,
			SpotLightType,
			HemisphereLightType,
			NoneType
		};
		
		Light();
		virtual ~Light();

	public:

		virtual Light* asLight() { return this; }
		virtual const Light* asLight() const { return this; }

		void setColor(ColorValue color) { _color = color; }
		ColorValue getColor()const { return _color; }

		void setIntensity(float intensity) { _intensity = intensity; }
		float getIntensity()const { return _intensity; }

		void setCastShadow(bool isCast) { _castShadow = isCast; }
		bool getCastShadow()const { return _castShadow; }

		virtual LightType getType()const { return NoneType; }

	protected:

		ColorValue _color;
		float _intensity;
		bool _castShadow;
	}
}