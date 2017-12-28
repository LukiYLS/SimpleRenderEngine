#pragma once
#include "..\Math\Vector3D.h"
#include "..\Math\Vector4D.h"
using namespace Math;
namespace SRE {

	class Billboard {

		friend class BillboardSet;
	public:

		Billboard();
		Billboard(const Vector3D& position, BillboardSet* owner, const Vector3D& color = Vector3D(1.0,1.0,1.0));


		Vector3D getPosition()const { return _position; }
		Vector3D getColor()const { return _color; }
		float getRotateAngle()const { return _rotateAngle; }
		//left,right,top,bottom
		Vector4D getTexCoordRect()const { return _texCoordRect; }
		Vector3D getDirection()const { return _direction; }
	protected:

		Vector3D _position;
		Vector3D _direction;
		Vector3D _color;
		Vector4D _texCoordRect;

		float _rotateAngle;
		float _width;
		float _height;
		BillboardSet* _owerSet;
		unsigned short _texIndex;

		bool _ownDimensions;

	};
}