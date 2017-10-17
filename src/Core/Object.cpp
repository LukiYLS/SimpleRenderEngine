#include "Object.h"
#include "../Math/Vector4D.h"
namespace Core {

	void Object::applyMatrix(const Matrix4D& matrix)
	{
		_matrix_local = _matrix_local * matrix;
		_matrix_local.decompose(_position, _scale, _orientation);
	}
	void Object::setRotationFromAxisAngle(const Vector3D& axis, double angle)
	{
		_orientation.fromAngleAxis(angle, axis);
	}
	void Object::setRotationFromMatrix(const Matrix3D& rotate)
	{
		_orientation.fromRotationMatrix(rotate);
	}
	void Object::rotateOnAxis(const Vector3D& axis, double angle)
	{
		Quaternion quat;
		quat.fromAngleAxis(angle, axis);
		_orientation = _orientation * quat;
	}
	void Object::translateOnAxis(const Vector3D& axis, double distance)
	{
		Vector3D new_axis = axis * _orientation;
		_position += (distance * new_axis);		
	}
	void Object::localToWorld(Vector3D& vector)
	{
		vector = _matrix_world * vector;
	}
	void Object::worldToLocal(Vector3D& vector)
	{
		Matrix4D matrix_inverse = _matrix_world.getInverse();
		vector = matrix_inverse * vector;
	}
	bool Object::add(Object* object)
	{
		if (!object)return false;
		Object* parent = object->getParent();
		if (!parent)
			parent->remove(object);
		object->setParent(this);
		_children.push_back(object);
		return true;
	}
	bool Object::remove(Object* object)
	{
		for (Children::iterator it = _children.begin();it!=_children.end();it++)
		{
			if (*it == object)
			{
				_children.erase(it);
				return true;
			}
				
		}
		return false;
	}
	void Object::updateMatrixLocal()
	{
		_matrix_local = Matrix4D::makeTransformMatrix(_position, _scale, _orientation);
	}
	void Object::updateMatrixWorld()
	{
		this->updateMatrixLocal();
		if (!this->getParent())
			_matrix_world = _matrix_local;
		else
		{
			_matrix_world = _matrix_local * this->getParent()->getWorldMatrix();
		}

		for (auto child : _children)
			child->updateMatrixWorld();
	}

	Matrix4D Object::getLocalMatrix()
	{
		this->updateMatrixLocal();
		return _matrix_local;
	}

	Matrix4D Object::getWorldMatrix()
	{
		this->updateMatrixWorld();
		return _matrix_world;
	}

	Vector3D Object::getWorldPosition()
	{
		this->updateMatrixWorld();
		this->_matrix_world.decompose(_position, _scale, _orientation);
		return _position;
	}

	Quaternion Object::getWorldQuaternion()
	{
		this->updateMatrixWorld();
		this->_matrix_world.decompose(_position, _scale, _orientation);
		return _orientation;
	}

	Vector3D Object::getWorldScale()
	{
		this->updateMatrixWorld();
		this->_matrix_world.decompose(_position, _scale, _orientation);
		return _scale;
	}

}