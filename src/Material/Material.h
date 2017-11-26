#pragma once

#include "..\Core\TextureUnitState.h"
#include "..\Core\Constants.h"
namespace SRE {

	
	class Material {

	public:
		enum MaterialType {
			Basic,
			Shader,
			Phong,
			NO
		};
		typedef std::shared_ptr<Material> ptr;
		
		void setFog(bool fog) { _fog = fog; }
		bool getFog()const { return _fog; }

		void setBlendingMode(BlendingMode mode) { _blendingMode = mode; }
		BlendingMode getBlendingMode()const { return _blendingMode; }

		void setCullFaceMode(CullFaceMode mode) { _cullFaceMode = mode; }
		CullFaceMode getCullFaceMode()const { return _cullFaceMode; }

		void setOpacity(float opacity) { _opacity = opacity; }
		float getOpacity()const { return _opacity; }

		void setTransparent(bool transparent) { _transparaent = transparent; }
		bool getTransparent()const { return _transparaent; }

		void setDepthFunc(CompareFunc func) { _depthFunc = func; }
		CompareFunc gettDepthFunc()const { return _depthFunc; }

		void setDepthTest(bool depthTest) { _depthTest = depthTest; }
		bool getDepthTest()const { return _depthTest; }

		void setDepthWrite(bool depthWrite) { _depthWrite = depthWrite; }
		bool getDepthWrite()const { return _depthFunc; }

		void setColorWrite(bool colorWrite) { _colorWrite = colorWrite; }
		bool getColorWrite()const { return _colorWrite; }

		void setVisible(bool visible) { _visible = visible; }
		bool getVisible()const { return _visible; }

		void setShadeType(ShadeOptions type) { _shadeType = type; }
		ShadeOptions getShadeType()const { return _shadeType; }

		virtual MaterialType getType()const { return NO; }

		/*void setAmbient(const ColorValue& diffuse);
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
		void getEmissiveColor(float& r, float& g, float& b, float& a)const;*/

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



	protected:
		bool _fog, _transparaent, _depthTest, _depthWrite, _colorWrite, _visible;
		BlendingMode _blendingMode;
		CullFaceMode _cullFaceMode;
		CompareFunc _depthFunc;
		ShadeOptions _shadeType;
		float _opacity;
	};
}