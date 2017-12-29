#pragma once
#include "..\Math\Vector3D.h"
#include "..\Math\Vector4D.h"
using namespace Math;
namespace SRE {

	class Billboard {

		friend class BillboardSet;
	public:
		typedef std::shared_ptr<Billboard> ptr;
		Billboard();
		Billboard(const Vector3D& position, BillboardSet* owner, const Vector3D& color = Vector3D(1.0,1.0,1.0));


		void setSize(float width, float height) { _width = width; _height = height; }

		float getHeight()const { return _height; }
		float getWidth()const { return _width; }

		Vector3D getPosition()const { return _position; }
		void setPosition(const Vector3D& position) { _position = position; }
		Vector3D getColor()const { return _color; }
		void setColor(const Vector3D& color) { _color = color; }
		float getRotationAngle()const { return _rotationAngle; }
		void setRotaionAngle(const float& angle) { _rotationAngle = angle; }
		//left,right,top,bottom
		Vector4D getTexCoordRect()const { return _texCoordRect; }
		void setTexCoordRect(const Vector4D& rect) { _texCoordRect = rect; }
		Vector3D getDirection()const { return _direction; }
		void setDiection(const Vector3D& direction) { _direction = direction; }
	protected:

		Vector3D _position;
		Vector3D _direction;
		Vector3D _color;
		Vector4D _texCoordRect;

		float _rotationAngle;
		float _width;
		float _height;
		BillboardSet* _owerSet;
		unsigned short _texIndex;

		bool _ownDimensions;

	};
}