#pragma once
using namespace NCL::Maths;

namespace NCL {
	class CollisionVolume;
	
	namespace CSC8503 {
		class Transform;

		class PhysicsObject	{
		public:
			PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume);
			~PhysicsObject();

			Vector3 GetLinearVelocity() const {
				return linearVelocity;
			}

			Vector3 GetAngularVelocity() const {
				return angularVelocity;
			}

			Vector3 GetTorque() const {
				return torque;
			}

			Vector3 GetForce() const {
				return force;
			}

			Vector3 GetGravityValue() const {
				return gravityValue;
			}
			void SetGravityValue(Vector3 g) {
				gravityValue = g;
			}

			bool GetGravity() const {
				return gravity;
			}
			void SetGravity(bool g) {
				gravity = g;
			}

			bool GetResolve() const {
				return resolve;
			}
			void SetResolve(bool r) {
				resolve = r;
			}

			int GetChannel() const {
				return channel;
			}
			void SetChannel(int c) {
				channel = c;
			}

			float GetElasticity() const {
				return elasticity;
			}
			void SetElasticity(float e) {
				elasticity = e;
			}

			float GetFriction() const {
				return friction;
			}
			void SetFriction(float f) {
				friction = f;
			}

			void SetInverseMass(float invMass) {
				inverseMass = invMass;
			}

			float GetInverseMass() const {
				return inverseMass;
			}

			void ApplyAngularImpulse(const Vector3& force);
			void ApplyLinearImpulse(const Vector3& force);
			
			void AddForce(const Vector3& force);

			void AddForceAtPosition(const Vector3& force, const Vector3& position);

			void AddTorque(const Vector3& torque);


			void ClearForces();
			void ClearTorque();

			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

			void InitCubeInertia();
			void InitSphereInertia();

			void UpdateInertiaTensor();

			Matrix3 GetInertiaTensor() const {
				return inverseInteriaTensor;
			}
			Vector3 GetInertia() { 
				return inverseInertia; 
			}

		protected:
			const CollisionVolume* volume;
			Transform*		transform;

			float inverseMass;
			float elasticity;
			float friction;
			bool resolve;
			bool gravity;
			int  channel;

			Vector3 gravityValue;

			//linear stuff
			Vector3 linearVelocity;
			Vector3 force;
			
			//angular stuff
			Vector3 angularVelocity;
			Vector3 torque;
			Vector3 inverseInertia;
			Matrix3 inverseInteriaTensor;
		};
	}
}
