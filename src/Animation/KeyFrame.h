#pragma once

#include "../Utils/AnyValue.h"
#include "../Math/Vector3D.h"
#include "../Math/Quaternion.h"
#include <memory>
using namespace Utils;
using namespace Math;
namespace SRE {

	class AnimationTrack;
	class KeyFrame {

	public:
		typedef std::shared_ptr<KeyFrame> ptr;
		KeyFrame(const AnimationTrack* parent, double time);

		virtual ~KeyFrame() {}

		
		double getTime(void) const { return _time; }
		


	protected:
		double _time;
		const AnimationTrack* _parentTrack;
	};

	class NumericKeyFrame : public KeyFrame {

	public:
		typedef std::shared_ptr<NumericKeyFrame> ptr;
		NumericKeyFrame(const AnimationTrack* parent, double time);
		~NumericKeyFrame() {}
		virtual const AnyValue& getValue(void) const;
		virtual void setValue(const AnyValue& val);

	protected:
		AnyValue _value;
	};

	class TransformKeyFrame : public KeyFrame
	{
	public:
		typedef std::shared_ptr<TransformKeyFrame> ptr;
		TransformKeyFrame(const AnimationTrack* parent, float time);
		~TransformKeyFrame() {}
		
		virtual void setTranslate(const Vector3D& trans);
		
		const Vector3D& getTranslate(void) const;
	
		virtual void setScale(const Vector3D& scale);
		
		virtual const Vector3D& getScale(void) const;
		
		virtual void setRotation(const Quaternion& rot);
		
		virtual const Quaternion& getRotation(void) const;	
		
	protected:

		Vector3D _translate;
		Vector3D _scale;
		Quaternion _quaternion;


	};
}