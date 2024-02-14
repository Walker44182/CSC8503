#pragma once
#include "StateGameObject.h"
#include "PlayerObject.h"
#include "RenderObject.h"
#include "MazeGrid.h"

namespace NCL {
	namespace CSC8503 {
		class SecurityObject : public StateGameObject {
		public:
			SecurityObject();
			virtual ~SecurityObject();

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;

			void SetPlayerObject(PlayerObject* p) {
				player = p;
			}

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void Attack();
			void MoveBack();
			float GetAngle();
			void UpdateOrientation();
			bool InMaze();
			bool OutMaze();
			
			PlayerObject* player;
			GameObject* objClosest;
			MazeGrid1* mazeGrid;
			vector<Vector3> testNodes;
			Vector3 direction;
			float velocity;
			bool back;		
		};
	}
}