#include "CamerControl.h"
#include "InputDefine.h"

namespace Utils
{
	CameraControl::CameraControl(Camera* camera)
	{
		_camera = (Camera::ptr)camera;
		
		_last_x_pos = 0;
		_last_y_pos = 0;

		_lb_pressing = false;
		_mb_pressing = false;
		_rb_pressing = false;
	}
	void CameraControl::receive(Event::ptr event)
	{		
		if (strcmp(event->getName(), "mouse.event") == 0)
		{
			mouse_event_vector events = event->getValue("mouse.event");
			for (unsigned int i = 0; i < events._event_num; ++i)
			{
				mouse_event mouse_work = events._mouse_events[i];

				switch (mouse_work.mouse_status)
				{
				case LB_DOWN:
				{
					_lb_pressing = true;
					break;
				}
				case LB_UP:
				{
					_lb_pressing = false;
					break;
				}
				case M_MOVE:
				{
					if (_lb_pressing)
					{
						int dx = mouse_work.xpos - _last_x_pos;
						int dy = mouse_work.ypos - _last_y_pos;
						_camera->translate(1, 1, 1);
						_camera->yaw(100);
						_camera->pitch(100);
					}
					else if (_mb_pressing)
					{

					}
					else if (_rb_pressing)
					{

					}
					break;
				}
					
				}
				_last_x_pos = mouse_work.xpos;
				_last_y_pos = mouse_work.ypos;
			}
		}
		else if (strcmp(event->getName(), "keyboard_event") == 0)
		{

		}
	}
}