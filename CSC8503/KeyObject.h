#pragma once
#include "GameObject.h"
#include "PlayerObject.h"
#include "Window.h"
#include <string>

namespace NCL {
	namespace CSC8503 {
		class KeyObject : public GameObject {
		public:
			KeyObject(std::string name) ;
			virtual ~KeyObject() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetPlayerObject(PlayerObject* player) {
				playerObject = player;
			};

		protected:
			bool putDown;
			PlayerObject* playerObject;
		};
	}
}