#pragma once
#include "StateGameObject.h"
#include "PlayerObject.h"
#include "ItemObject.h"
#include "RenderObject.h"
#include "BehaviourNode.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "MazeGrid.h"

namespace NCL {
	namespace CSC8503 {
		class GooseObject : public StateGameObject {
		public:
			GooseObject();
			virtual ~GooseObject();

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;

			float GetVelocity() const {
				return velocity;
			};
			void SetVelocity(float v) {
				velocity = v;
			};

			void SetPlayerObject(PlayerObject* p) {
				player = p;
			};

			int GetPowerup() const {
				return powerup;
			}

			bool GetAttack() const {
				return attack;
			}

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void MoveBack();
			void Attack();
			float GetAngle();
			void UpdateOrientation();
			bool InMaze();
			bool OutMaze();

			float counter1;
			float counter2;
			bool useCounter2;
			bool back;
			bool attack;
			bool foundPlayer;
			int powerup;
			int life;
			float velocity;
			BehaviourSequence* sequence;
			PlayerObject* player;
			GameObject* objClosest;
			MazeGrid2* mazeGrid;
			vector<Vector3> testNodes;
			Vector3 direction;
			Vector4 colour;

			std::string gooseNotice;
		};

		class MazeGoose :public StateGameObject {
		public:
			MazeGoose();
			virtual ~MazeGoose();

			virtual void Update(float dt) override;

			void SetPlayerObject(MazePlayer* p){
				player = p;
			};

			void SetItemObject(MazeItem* i) {
				item = i;
			};

		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
			void Attack();
			float GetAngle();
			void UpdateOrientation();

			GameObject* player;
			MazeItem* item;
			GameObject* objClosest;
			MazeGrid2* mazeGrid;
			vector<Vector3> testNodes;
			Vector3 direction;
		};
	}
}