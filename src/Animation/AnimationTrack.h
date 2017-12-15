#pragma once
#include "KeyFrame.h"
#include "../Core/Object.h"
#include <vector>
namespace SRE {

	class Animation;
	class AnimationTrack {

	public:
		virtual KeyFrame* getKeyFrame(unsigned short index) const;

		virtual KeyFrame* createKeyFrame(float timePos);
	
		virtual void removeKeyFrame(unsigned short index);
		
		virtual void removeAllKeyFrames(void);

	protected:
		typedef std::vector<KeyFrame::ptr> KeyFrameList;
		KeyFrameList _keyFrames;	
		Animation* _parent;
	};

	class ObjectAnimationTrack : public AnimationTrack {

	public:
		void applyToTarget();

	protected:

		Object::ptr _target
	};
	
}