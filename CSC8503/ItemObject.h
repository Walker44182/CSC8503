#pragma once
#include "GameObject.h"
#include "PlayerObject.h"

namespace NCL {
	namespace CSC8503 {
		class ItemObject : public GameObject {
		public:
			ItemObject();
			virtual ~ItemObject() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetPlayerObject(PlayerObject* player) {
				playerObject = player;
			};

		protected:
			bool putDown;
			bool finish;
			PlayerObject* playerObject;
		};

		class MazeItem :public ItemObject {
		public:
			MazeItem();
			virtual ~MazeItem() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			void SetMazePlayer(MazePlayer* player) {
				mazePlayer = player;
			};
			GameObject* GetMazePlayer() const {
				return mazePlayer;
			};

		protected:
			GameObject* mazePlayer;
		};
	}
}