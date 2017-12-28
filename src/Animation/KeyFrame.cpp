#include "KeyFrame.h"



namespace SRE {

	/*
	KeyFrame 
	*/
	KeyFrame::KeyFrame(const AnimationTrack* parentTrack, double time)
	:_time(time), _parentTrack(parentTrack){



	}

	//===========================================================================

	NumericKeyFrame::NumericKeyFrame(const AnimationTrack* parentTrack, double time)
	:KeyFrame(parentTrack,time){

	}


	//============================================================================

	TransformKeyFrame::TransformKeyFrame(const AnimationTrack* parentTrack, float time)
		:KeyFrame(parentTrack, time) {

	}
}