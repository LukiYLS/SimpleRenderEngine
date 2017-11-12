#pragma once
#include "../Math/Vector3D.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix4D.h"
#include "../Utils/AnyValue.h"
#include "RayCaster.h"
#include <memory>
#include <vector>
using namespace Math;
namespace Core {
	class Mesh;
	class Light;
	class Plugin;
	class Sprite;
	class BillboardCollection;
	class ParticleSystem;
	class Object {
	public:
		typedef std::shared_ptr<Object> ptr;
		Object() :_position(Vector3D(0.0, 0.0, 0.0)), _scale(Vector3D(1.0, 1.0, 1.0)) {}
		Object(const Vector3D& pos) :_position(pos), _scale(Vector3D(1.0, 1.0, 1.0)) {}
		Object(const Vector3D& pos, const Quaternion& quat) :_position(pos), _orientation(quat), _scale(Vector3D(1.0, 1.0, 1.0)) {}
	public:
		virtual Mesh* asMesh() { return 0; }
		virtual const Mesh* asMesh() const { return 0; }

		virtual Light* asLight() { return 0; }
		virtual const Light* asLight() const { return 0; }

		virtual Plugin* asPlugin() { return 0; }
		virtual const Plugin* asPlugin() const { return 0; }

		virtual Sprite* asSprite() { return 0; }
		virtual const Sprite* asSprite() const { return 0; }

		virtual BillboardCollection* asBillboardCollection() { return 0; }
		virtual const BillboardCollection* asBillboardCollection() const { return 0; }

		virtual ParticleSystem* asParticleSystem() { return 0; }
		virtual const ParticleSystem* asParticleSystem() const { return 0; }

		virtual Object* asObject() { return this; }
		virtual const Object* asObject() const { return this; }

		virtual void raycast(RayCaster* raycaster, AnyValue& intersects) { return; }

	public:
		inline Object* getParent() {
			if (!_parent)return _parent;
			else
				return NULL;
		}
		inline const Object* getParent() const { return _parent; }
		inline void setParent(Object* object) { _parent = object; }
		inline const Vector3D& getPosition()const { return _position; }
		inline void setPosition(const Vector3D& pos) { _position = pos; }
		inline const Vector3D& getScale()const { return _scale; }
		inline void setScale(const Vector3D& scale) { _scale = scale; }
		inline const Quaternion& getOrientation()const { return _orientation; }
		inline void setOrientation(const Quaternion& orientation) { _orientation = orientation; }

		void applyMatrix(const Matrix4D& matrix);
		//_orientation set
		void setRotationFromAxisAngle(const Vector3D& axis, double angle);
		void setRotationFromMatrix(const Matrix4D& rotate);
		//_orientation change
		void rotateOnAxis(const Vector3D& axis, double angle);
		void rotateOnX(double angle) { rotateOnAxis(Vector3D(1.0, 0.0, 0.0), angle); }
		void rotateOnY(double angle) { rotateOnAxis(Vector3D(0.0, 1.0, 0.0), angle); }
		void rotateOnZ(double angle) { rotateOnAxis(Vector3D(0.0, 0.0, 1.0), angle); }
		void translateOnAxis(const Vector3D& axis, double distance);	
		void translateOnX(double distance) { translateOnAxis(Vector3D(1.0, 0.0, 0.0), distance); }
		void translateOnY(double distance) { translateOnAxis(Vector3D(0.0, 1.0, 0.0), distance); }
		void translateOnZ(double distance) { translateOnAxis(Vector3D(0.0, 0.0, 1.0), distance); }
		//useful function
		void localToWorld(Vector3D& vector);
		void worldToLocal(Vector3D& vector);
		//
		bool add(Object* object);
		bool remove(Object* object);
		Object* getChild(int index);	
		typedef std::vector<Object*> Children;
		Children getChildren() { return _children; }
		void updateMatrixLocal();
		void updateMatrixWorld();
		//
		Matrix4D getLocalMatrix();
		Matrix4D getWorldMatrix();
		Vector3D getWorldPosition();
		Quaternion getWorldQuaternion();
		Vector3D getWorldScale();
	public:
		
		bool visible;
	protected:
		Object* _parent;
		Vector3D _position;
		Vector3D _scale;
		Quaternion _orientation;		
		
		Children _children;

		Matrix4D _matrix_local;
		Matrix4D _matrix_world;
	};
}