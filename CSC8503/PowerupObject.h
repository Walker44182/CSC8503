#pragma once
#include "GameObject.h"
#include "PlayerObject.h"
#include "GooseObject.h"
#include "Window.h"

namespace NCL {
	namespace CSC8503 {
		class PlayerPowerup : public GameObject {
		public:
			PlayerPowerup(std::string name);
			virtual ~PlayerPowerup() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetPlayerObject(PlayerObject* player) {
				playerObject = player;
			};

			void SetColour(Vector4 c) {
				colour = c;
			};

		protected:
			bool putDown;
			Vector4 colour;
			PlayerObject* playerObject;
		};

		class GoosePowerup : public GameObject {
		public:
			GoosePowerup();
			virtual ~GoosePowerup() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetGooseObject(GooseObject* goose) {
				gooseObject = goose;
			}

		protected:
			bool putDown;
			GooseObject* gooseObject;
		};
	}
}