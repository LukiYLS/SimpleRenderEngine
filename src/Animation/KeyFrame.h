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

		NumericKeyFrame(const AnimationTrack* parentTrack, double time);
		~NumericKeyFrame() {}

		virtual const AnyValue& getValue(void) const { return _value; }
		virtual void setValue(const AnyValue& value) { _value = value; }

	protected:
		AnyValue _value;
	};

	class TransformKeyFrame : public KeyFrame
	{
	public:
		typedef std::shared_ptr<TransformKeyFrame> ptr;
		TransformKeyFrame(const AnimationTrack* parentTrack, float time);
		~TransformKeyFrame() {}
		
		virtual void setTranslate(const Vector3D& trans) { _translate = trans; }
		
		const Vector3D& getTranslate(void) const { return _translate; }
	
		virtual void setScale(const Vector3D& scale) { _scale = scale; }
		
		virtual const Vector3D& getScale(void) const { return _scale; }
		
		virtual void setRotation(const Quaternion& rotation) { _rotation = rotation; }	
		
		virtual const Quaternion& getRotation(void) const { return _rotation; }
		
	protected:

		Vector3D _translate;
		Vector3D _scale;
		Quaternion _rotation;


	};
}