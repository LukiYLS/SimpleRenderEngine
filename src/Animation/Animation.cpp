#include "Animation.h"


namespace SRE {

	void Animation::collectTrackToKeyFrames(std::vector<float>& keyFrameTimes)
	{
		for (KeyFrameTimeList::iterator it = keyFrameTimes.begin(); it != keyFrameTimes.end(); it++)
		{
			float timePos = *it;
		}
	}
}