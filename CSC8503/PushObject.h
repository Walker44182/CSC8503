#pragma once
#include "StateGameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class PushObject : public StateGameObject {
		public:
			PushObject();
			virtual ~PushObject();

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;
		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
		};
	}
}