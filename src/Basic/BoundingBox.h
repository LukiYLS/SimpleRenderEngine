#pragma once
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
namespace Core {

	class BoundingBox {

	public:
		enum Extent
		{
			EXTENT_NULL,
			EXTENT_FINITE,
			EXTENT_INFINITE
		};
	protected:
		glm::vec3 _minimum;
		glm::vec3 _maximum;
		Extent mExtent;

		BoundingBox();
		BoundingBox(glm::vec3 min, glm::vec3 max):_minimum(min),_maximum(max){}

		inline const glm::vec3& getMinimum(void) const
		{
			return _minimum;
		}		
		inline glm::vec3& getMinimum(void)
		{
			return _minimum;
		}
		inline const glm::vec3& getMaximum(void) const
		{
			return _maximum;
		}		
		inline glm::vec3& getMaximum(void)
		{
			return _maximum;
		}
		inline void setMinimum(const glm::vec3& vec)
		{
			mExtent = EXTENT_FINITE;
			_minimum = vec;
		}
		inline void setMaximum(const glm::vec3& vec)
		{
			mExtent = EXTENT_FINITE;
			_maximum = vec;
		}

	};
}