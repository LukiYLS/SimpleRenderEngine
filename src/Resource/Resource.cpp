#include "Resource.h"


namespace Core{


	Resource::Resource()
	{

	}
	Resource::~Resource()
	{

	}
	void Resource::prepare()
	{
		if (_load_state != LOADSTATE_UNLOADED)return;

		if (_is_manual)
		{
			//手动加载器加载
			/*if (_manual_loader)
			{
				_manual_loader->prepare();
			}
			else
			{

			}*/
		}
		else
		{
			prepareImpl();
		}
	}
	void Resource::load()
	{
		bool ask_is_ready_to_load = false;

		while (!ask_is_ready_to_load)
		{
			//1.轮询是否一直到在preparing，主要是针对多线程加载
			if (_load_state == LOADSTATE_PREPARING)
			{
				while (_load_state == LOADSTATE_LOADING)
				{
					//auto_lock
				}
				//_load_state = 
			}

			
		}
	}
}