#include "Billboard.h"

namespace SRE {

	Billboard::Billboard()
	:_ownDimensions(false),
	_position(Vector3D(0.0,0.0,0.0)),
	_direction(Vector3D(0.0, 0.0, 0.0)),
	_color(Vector3D(1.0,1.0,1.0)),
	_rotationAngle(0.0){

	}

	Billboard::~Billboard()
	{
	}

	Billboard::Billboard(const Vector3D& position, BillboardSet* owner, const Vector3D& color /* = Vector3D(1.0,1.0,1.0) */)
	:_position(position),
	_owerSet(owner),
	_color(color),
	_rotationAngle(0.0),
	_direction(Vector3D(0.0, 0.0, 0.0)){

	}
}