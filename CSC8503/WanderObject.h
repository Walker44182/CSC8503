#pragma once
#include "StateGameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class WanderObject : public StateGameObject {
		public:
			WanderObject();
			virtual ~WanderObject();

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			float GetVelocity() const {
				return velocity;
			};
			void SetVelocity(float v) {
				velocity = v;
			};
		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);

			float velocity;
		};
	}
}