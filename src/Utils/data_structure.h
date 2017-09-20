#ifndef __DATA_STRUCTURE_H__
#define __DATA_STRUCTURE_H__
#include "glm\glm.hpp"

namespace Utils
{
	class RenderParams
	{
	public:
		RenderParams() {}
		~RenderParams() {}

		void  setM(const glm::mat4& m) { this->_m = m; }
		void  setV(const glm::mat4& v) { this->_v = v; }
		void  setP(const glm::mat4& p) { this->_p = p; }
		void  setCurrTime(float time) { _currentTime = time; }
		void  setEye(glm::vec3 eye) { _eye = eye; }

		glm::mat4x4&  m() { return _m; }
		glm::mat4x4&  v() { return _v; }
		glm::mat4x4&  p() { return _p; }
		float  currTime() { return _currentTime; }
		glm::vec3&  eye() { return _eye; }
	private:
		glm::mat4  _m;
		glm::mat4  _v;
		glm::mat4  _p;
		glm::vec3  _eye;
		float _currentTime;

	};



}

#endif
