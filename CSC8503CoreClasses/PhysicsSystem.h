#pragma once
#include "GameWorld.h"

namespace NCL {
	namespace CSC8503 {
		class PhysicsSystem	{
		public:
			PhysicsSystem(GameWorld& g);
			~PhysicsSystem();

			void Clear();

			void Update(float dt);

			void UseGravity(bool state) {
				applyGravity = state;
			}

			void SetGlobalDamping(float d) {
				globalDamping = d;
			}

			void SetGravity(const Vector3& g);
		protected:
			void BasicCollisionDetection();
			void BroadPhase();
			void NarrowPhase(float dt);

			void ClearForces();

			void IntegrateAccel(float dt);
			void UpdateAccel(GameObject& i, float dt);
			void IntegrateVelocity(float dt);

			void UpdateConstraints(float dt);

			void UpdateCollisionList();
			void UpdateObjectAABBs();
			void ProjectionResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;
			void ImpulseResolveCollision(GameObject& a , GameObject&b, CollisionDetection::ContactPoint& p) const;
			void PenaltyResolveCollision(GameObject& a, GameObject& b, CollisionDetection::ContactPoint& p) const;

			static Vector3 GetImpulseFriction(PhysicsObject* physObj, const Vector3& normal, const Vector3& v, float m, float j);

			GameWorld& gameWorld;

			bool	applyGravity;
			float	dTOffset;
			float	globalDamping;
			float	penaltyScale;

			std::set<CollisionDetection::CollisionInfo> allCollisions;
			std::set<CollisionDetection::CollisionInfo> broadphaseCollisions;
			std::vector<CollisionDetection::CollisionInfo> broadphaseCollisionsVec;
			bool useBroadPhase		= true;
			int numCollisionFrames	= 5;
		};
	}
}

