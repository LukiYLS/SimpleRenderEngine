#pragma once
#include "../Math/Vector3D.h"
#include "../Math/Quaternion.h"
#include "../Math/Matrix4D.h"
#include <memory>
#include <vector>
namespace Core {
	class Mesh;
	class Light;
	class Plugin;
	class Sprite;
	class Billboard;
	class ParticleSystem;
	class Object {
	public:
		typedef std::shared_ptr<Object> ptr;
	public:
		virtual Mesh* asMesh() { return 0; }
		virtual const Mesh* asMesh() const { return 0; }

		virtual Light* asLight() { return 0; }
		virtual const Light* asLight() const { return 0; }

		virtual Plugin* asPlugin() { return 0; }
		virtual const Plugin* asPlugin() const { return 0; }

		virtual Sprite* asSprite() { return 0; }
		virtual const Sprite* asSprite() const { return 0; }

		virtual Billboard* asBillboard() { return 0; }
		virtual const Billboard* asBillboard() const { return 0; }

		virtual ParticleSystem* asParticleSystem() { return 0; }
		virtual const ParticleSystem* asParticleSystem() const { return 0; }

		virtual Object* asObject() { return this; }
		virtual const Object* asObject() const { return this; }

	public:
		inline Object* getParent() { return _parent; }
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
		void setRotationFromMatrix(const Matrix3D& rotate);
		//_orientation change
		void rotateOnAxis(const Vector3D& axis, double angle);
		void translateOnAxis(const Vector3D& axis, double distance);
		//useful function
		void localToWorld(Vector3D& vector);
		void worldToLocal(Vector3D& vector);
		//
		bool add(Object* object);
		bool remove(Object* object);
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
		typedef std::vector<Object*> Children;
		Children _children;

		Matrix4D _matrix_local;
		Matrix4D _matrix_world;
	};
}