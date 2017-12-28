#include "AnimationTrack.h"
#include "Animation.h"
#include <algorithm>
namespace SRE {

	
	struct KeyFrameTimeLess
	{
		bool operator() (const KeyFrame* kf, const KeyFrame* kf2) const
		{
			return kf->getTime() < kf2->getTime();
		}
	};

	AnimationTrack::AnimationTrack(Animation* parent, unsigned short index)
	:_parent(parent),_index(index){

	}
	AnimationTrack::~AnimationTrack()
	{

	}

	KeyFrame* AnimationTrack::getKeyFrame(unsigned short index) const
	{
		if (index < _keyFrames.size())
			return _keyFrames[index].get();
		return NULL;
	}

	KeyFrame* AnimationTrack::createKeyFrame(float timePos)
	{
		KeyFrame* keyframe = createKeyFrameImpl(timePos);

		KeyFrameList::iterator it = std::upper_bound(_keyFrames.begin(), _keyFrames.end(), KeyFrameTimeLess());

		_keyFrames.insert(it, (KeyFrame::ptr)keyframe);

		return keyframe;
	}
	/*

	*/
	void AnimationTrack::collectKeyFrameTimes(std::vector<float>& keyFrameTimes)
	{
		for (KeyFrameList::const_iterator it = _keyFrames.begin(); it != _keyFrames.end(); it++)
		{
			float timePos = (*it)->getTime();

			std::vector<float>::iterator it_lower = std::lower_bound(keyFrameTimes.begin(), keyFrameTimes.end(), timePos);

			if (it_lower == keyFrameTimes.end() || *it_lower != timePos)
				keyFrameTimes.insert(it_lower, timePos);
		}
	}
	/*
	根据所给的时间，得到该时间的前后两帧或者一帧（当该时间有对应的关键帧时）
	*/
	float AnimationTrack::getKeyFramesAtTime(const TimeIndex& timeIndex, KeyFrame** keyFrame1, KeyFrame** keyFrame2,
		unsigned short* firstKeyIndex) const
	{
		float t1, t2;
		float timePos = timeIndex.getTimePos();

		KeyFrameList::const_iterator i;
		//如果有关键帧
		if (timeIndex.hasKeyIndex())
		{
			if(timeIndex.getKeyIndex() < _keyFrameIndexList.size())

		}
		else
		{
			// Wrap time
			float totalAnimationLength = _parent->getLength();
			assert(totalAnimationLength > 0.0f && "Invalid animation length!");

			if (timePos > totalAnimationLength && totalAnimationLength > 0.0f)
				timePos = fmod(timePos, totalAnimationLength);

			// No global keyframe index, need to search with local keyframes.
			KeyFrame timeKey(0, timePos);
			i = std::lower_bound(_keyFrames.begin(), _keyFrames.end(), &timeKey, KeyFrameTimeLess());
		}
	}


	//===========================================================================================================

	ObjectAnimationTrack::ObjectAnimationTrack(Animation* parent, unsigned short index)
	:AnimationTrack(parent, index){

	}

	ObjectAnimationTrack::ObjectAnimationTrack(Animation* parent, unsigned short index, Object::ptr target)
	: AnimationTrack(parent, index), _target(target){

	}
	/*
	根据时间插值出帧信息，主要是对object的个参数进行插值计算
	*/
	void ObjectAnimationTrack::getInterpolatedKeyFrame(const TimeIndex& timeIndex, KeyFrame* kf) const
	{
		TransformKeyFrame* kret = static_cast<TransformKeyFrame*>(kf);

		KeyFrame *base1, *base2;
		TransformKeyFrame *k1, *k2;
	}

	void ObjectAnimationTrack::applyToTarget(Object* target, const TimeIndex& timeIndex, float weight, float scale)
	{
		if (_keyFrames.empty() || !weight || !target)
			return;

		TransformKeyFrame kf(0, timeIndex.getTimePos());
		getInterpolatedKeyFrame(timeIndex, &kf);

		Vector3D translate = kf.getTranslate() * weight * scale;
		target->translate(translate);

		Quaternion rotate = Quaternion::slerp(weight, Quaternion(), kf.getRotation());

		target->rotate(rotate);

		Vector3D scale = kf.getScale();

		//weight scale

		target->scale(scale);
	}
}