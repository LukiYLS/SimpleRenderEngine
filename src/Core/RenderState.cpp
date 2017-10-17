#include "RenderState.h"

namespace Core {


	RenderState::RenderState()
	{

	}

	void RenderState::use()
	{		
		for (auto it = _state_map.begin(); it != _state_map.end(); it++)
		{
			if (it->second)
				glEnable(it->first);
			else
				glDisable(it->first);
		}

		glCullFace(mode);
		glDepthFunc(depthFunc);
		//glStencilFunc(stencilFunc);
	}
}