#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <memory>
namespace Core {

	class ViewPort {
	public:
		typedef std::shared_ptr<ViewPort> ptr;
	public:
		ViewPort() { setViewport(0, 0, 800, 600); }
		ViewPort(int x, int y, int width, int height):
			_x(x),_y(y),_width(width),_height(height){}

		ViewPort& operator = (const ViewPort& rhs)
		{
			if (&rhs == this) return *this;

			_x = rhs._x;
			_y = rhs._y;
			_width = rhs._width;
			_height = rhs._height;

			return *this;
		}

		inline void setViewport(int x, int y, int width, int height)
		{
			_x = x;
			_y = y;
			_width = width;
			_height = height;
		}

		inline int& x() { return _x; }
		inline int x() const { return _x; }

		inline int& y() { return _y; }
		inline int y() const { return _y; }

		inline int& width() { return _width; }
		inline int width() const { return _width; }

		inline int& height() { return _height; }
		inline int height() const { return _height; }

		inline bool valid() const { return _width>0 && _height>0; }		
		inline double aspectRatio() const { if (_height != 0) return (double)_width / (double)_height; else return 1.0; }
		inline const glm::mat4 computeWindowMatrix() const
		{
			return glm::translate(glm::mat4(), glm::vec3(1.0, 1.0, 1.0))*glm::scale(glm::mat4(), glm::vec3(0.5*width(), 0.5*height(), 0.5f))*glm::translate(glm::mat4(), glm::vec3(x(), y(), 0.0f));
		}
	protected:
		int _x;
		int _y;
		int _width;
		int _height;
	};
}