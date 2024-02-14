#pragma once
#include "StateGameObject.h"
#include "PlayerObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class ShootObject : public StateGameObject {
		public:
			ShootObject();
			virtual ~ShootObject();

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetPlayerObject(PlayerObject* p) {
				player = p;
			}

			bool CanShoot() const {
				return canShoot;
			};
		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void Attack(float dt);
			float GetAngle();

			bool canShoot;
			PlayerObject* player;
			GameObject* objClosest;
		};
	}
}