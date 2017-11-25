#pragma once

#include "TextureUnitState.h"
namespace SRE {

	class Material {

	public:
		typedef std::shared_ptr<Material> ptr;
		//should be technique
		TextureUnitState::ptr getTexture(int index)const;
		int getTextureCount()const;

		void setAmbient(const ColorValue& diffuse);
		void setAmbient(float r, float g, float b, float a);
		void getAmbient(float& r, float& g, float& b, float &a)const;

		void setDiffuse(const ColorValue& diffuse);
		void setDiffuse(float r, float g, float b, float a) ;
		void getDiffuse(float& r, float& g, float& b, float &a)const;

		void setSpecular(const ColorValue& diffuse);
		void setSpecular(float r, float g, float b, float a);
		void getSpecular(float& r, float& g, float& b, float &a)const;		

		void setShininess(float shininess);
		float getShininess()const;

		void setEmissiveColor(const ColorValue& diffuse);
		void setEmissiveColor(float r, float g, float b, float a);
		void getEmissiveColor(float& r, float& g, float& b, float& a)const;

		//void setReceiveShadows(bool enabled) { mReceiveShadows = enabled; }
		//bool getReceiveShadows(void) const { return mReceiveShadows; }
		//void setTransparencyCastsShadows(bool enabled) { mTransparencyCastsShadows = enabled; }
		//bool getTransparencyCastsShadows(void) const { return mTransparencyCastsShadows; }
		//unsigned short getNumLodLevels(unsigned short schemeIndex) const;
		//unsigned short getNumLodLevels(const String& schemeName) const;
		//void setDepthWriteEnabled(bool enabled);
		//void setLightingEnabled(bool enabled);
		//void setColourWriteEnabled(bool enabled);
		//void setDepthFunction(CompareFunction func);
		//void setCullingMode(CullingMode mode);
		//void setShadingMode(ShadeOptions mode);
		//void setLodLevels(const LodValueList& lodValues);
		//void setSceneBlending(const SceneBlendFactor sourceFactor, const SceneBlendFactor destFactor);
		//void setFog(bool overrideScene,	FogMode mode = FOG_NONE,const ColourValue& colour = ColourValue::White,
		//	Real expDensity = 0.001, Real linearStart = 0.0, Real linearEnd = 1.0);
		virtual void  setName(std::string name);
		virtual std::string getName() const;

		void setLoadState(bool state);
		bool getLoadState(void);

		//bool getTwoSided();
		//void setTwoSided(bool is_draw_two_sides_);

		bool isTransparent();
	protected:


	};
}