#pragma once
#include "KeyFrame.h"
#include "../Core/Object.h"
#include <vector>
namespace SRE {


	/*
	TimeIndex主要是为了判断在某个时间点内是否有关键帧
	*/
	class TimeIndex {

	public:
		TimeIndex(float timePos) :_timePos(timePos), _keyIndex(INVALID_KEY_INDEX) {}
		TimeIndex(float timePos, unsigned int keyIndex) :_timePos(timePos), _keyIndex(keyIndex) {}


		bool hasKeyIndex(void) const
		{
			return _keyIndex != INVALID_KEY_INDEX;
		}

		float getTimePos(void) const
		{
			return _timePos;
		}

		unsigned int getKeyIndex(void) const
		{
			return _keyIndex;
		}
	protected:

		float _timePos;
		unsigned int _keyIndex;
		static const unsigned int INVALID_KEY_INDEX = (unsigned int)-1;
	};


	//==============================================================
	class Animation;
	class AnimationTrack {
	public:

		AnimationTrack(Animation* parent, unsigned short index);
		~AnimationTrack();

	public:
		virtual KeyFrame* getKeyFrame(unsigned short index) const;

		virtual KeyFrame* createKeyFrame(float timePos);

		/*
		根据时间
		*/
		virtual float getKeyFramesAtTime(const TimeIndex& timeIndex, KeyFrame** keyFrame1, KeyFrame** keyFrame2,
			unsigned short* firstKeyIndex = 0) const;
	
		virtual void removeKeyFrame(unsigned short index);
		
		virtual void removeAllKeyFrames(void);

		virtual void getInterpolatedKeyFrame(const TimeIndex& timeIndex, KeyFrame* kf) const = 0;

		virtual void apply(const TimeIndex& timeIndex, float weight = 1.0, float scale = 1.0f) = 0;

	protected:

		void collectKeyFrameTimes(std::vector<float>& keyFrameTimes);
		virtual KeyFrame* createKeyFrameImpl(float timePos) = 0;

		typedef std::vector<KeyFrame::ptr> KeyFrameList;
		KeyFrameList _keyFrames;	
		
		std::vector<unsigned short> _keyFrameIndexList;//有效关键帧
		Animation* _parent;
		unsigned short _index;
	};

	class NumericAnimationTrack : public AnimationTrack
	{
	public:

		NumericAnimationTrack(Animation* parent, unsigned short index);
		NumericAnimationTrack(Animation* parent, unsigned short index, Object::ptr object);
		virtual void getInterpolatedKeyFrame(const TimeIndex& timeIndex, KeyFrame* kf) const;
		virtual void apply(const TimeIndex& timeIndex, float weight = 1.0, float scale = 1.0f);
	};

	class ObjectAnimationTrack : public AnimationTrack {

	public:
		typedef std::shared_ptr<ObjectAnimationTrack> ptr;
		ObjectAnimationTrack(Animation* parent, unsigned short index);
		ObjectAnimationTrack(Animation* parent, unsigned short index, Object::ptr target);
		virtual void getInterpolatedKeyFrame(const TimeIndex& timeIndex, KeyFrame* kf) const;
		virtual void apply(const TimeIndex& timeIndex, float weight = 1.0, float scale = 1.0f);
		void applyToTarget(Object* node, const TimeIndex& timeIndex, float weight = 1.0,
			float scale = 1.0f);

	protected:
		KeyFrame* createKeyFrameImpl(float time);

		Object::ptr _target;
	};

	class VertexAnimationTrack : AnimationTrack {

	public:

		typedef std::shared_ptr<VertexAnimationTrack> ptr;
	};
	
}