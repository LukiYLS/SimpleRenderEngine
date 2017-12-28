#pragma once

#include "AnimationTrack.h"
#include <map>
namespace SRE {

	class Animation {

	public:

		typedef std::map<unsigned short, ObjectAnimationTrack::ptr> ObjectTrackMap;
		//多个动画轨迹的时候，应该collect到一个keyframe队列，根据时间
		

	protected:
		void collectTrackToKeyFrames(std::vector<float>& keyFrameTimes);

		typedef std::vector<float> KeyFrameTimeList;
		mutable KeyFrameTimeList mKeyFrameTimes;

		ObjectTrackMap _objectAnimationTracks;
	};

	
}