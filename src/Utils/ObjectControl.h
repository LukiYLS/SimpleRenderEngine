#pragma once
#include "Event.h"

namespace Utils {

	class ObjectControl
		:public EventReceiver{
	public:

	public:
		virtual void receive(Event::ptr event);
	};
}