#pragma once
#include "KeyboardMouseController.h"
#include "GameTechRenderer.h"
#ifdef USEVULKAN
#include "GameTechVulkanRenderer.h"
#endif
#include "PhysicsSystem.h"
#include "NavigationGrid.h"
#include "NavigationMesh.h"
#include "StateGameObject.h"
#include "PlayerObject.h"
#include "PowerupObject.h"
#include "BridgeObject.h"
#include "PushObject.h"
#include "JumpObject.h"
#include "KeyObject.h"
#include "TreeObject.h"
#include "TowerObject.h"
#include "BonusObject.h"
#include "PenaltyObject.h"
#include "TargetObject.h"
#include "ItemObject.h"
#include "WanderObject.h"
#include "ShootObject.h"
#include "CannonObject.h"
#include "BombObject.h"
#include "SecurityObject.h"
#include "GooseObject.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame		{
		public:
			TutorialGame();
			~TutorialGame();

			virtual void UpdateGame(float dt);

			static TutorialGame* GetGame() {
				return instance;
			};

			int GetCameraMode() const {
				return cameraMode;
			};

			BombObject* AddBomb(const Vector3& position, int type) {
				BombObject* bomb = AddBombObjectToWorld(position, type);
				return bomb;
			};

			GameWorld* GetGameWorld() const {
				return world;
			};

			PlayerObject* GetPlayer() const {
				return playerGoat;
			};

		protected:
			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();

			/*
			These are some of the world/object creation functions I created when testing the functionality
			in the module. Feel free to mess around with them to see different objects being created in different
			test scenarios (constraints, collision types, and so on). 
			*/
			void InitGameExamples();

			void InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius);
			void InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing);
			void InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims);

			void InitDefaultFloor();
			void InitGround();
			void InitWall();
			void InitTree();
			void InitConstraint();
			void InitPlayer();
			void InitPowerup();
			void InitCollision();
			void InitKey();
			void InitTower();
			void InitTarget();
			void InitBonus();
			void InitTrigger();
			void InitAirWall();
			void InitItem();
			void InitNPC();

			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void GoatMovement1();
			void GoatMovement2();
			void BridgeConstraintTest();
			void TestPathfinding();
			void DisplayPathfinding();

			void CameraUpdate1();
			void CameraUpdate2();

			GameObject* AddSphereToWorld(const Vector3& position, float radius, float inverseMass = 10.0f);
			GameObject* AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddFloorToWorld(const Vector3& position);
			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& size, const Vector4& colour);
			GameObject* AddRespawnPointToWorld();
			GameObject* AddWallToWorld(const Vector3& position, const Vector3& size, const Vector4& colour);
			GameObject* AddPoolToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddPlankToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddTriggerToWorld(const Vector3& position, const Vector3& size, int type);
			GameObject* AddAirWallToWorld(const Vector3& position, const Vector3& size);
			GameObject* AddGuideToWorld(const Vector3& posiiton, int type);
			TreeObject* AddTreeToWorld(const Vector3& position, const Vector3& size);
			PlayerObject* AddPlayerToWorld(const Vector3& position);
			PlayerPowerup* AddPlayerPowerupToWorld(const Vector3& position, int type, std::string name, const Vector4& colour);
			GoosePowerup* AddGoosePowerupToWorld();
			BridgeObject* AddBridgeCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass = 10.0f);
			GameObject* AddGateCubeToWorld(const Vector3& position, Vector3 dimensions, const Vector4& colour);
			PushObject* AddPushObjectToWorld(const Vector3& position);
			JumpObject* AddJumpObjectToWorld(const Vector3& position);
			PenaltyObject* AddPenaltyObjectToWorld(const Vector3& position, int objectType);
			KeyObject* AddKeyObjectToWorld(const Vector3& position, int type, std::string name, const Vector4& colour);
			BonusObject* AddHiddenBonusObjectToWorld(const Vector3& position);
			ItemObject* AddItemObjectToWorld();
			BombObject* AddBombObjectToWorld(const Vector3& position, int type);
			GooseObject* AddGooseToWorld();
			void AddRopeBridgeToWorld(const Vector3& startPos, const Vector3& endPos);
			void AddGateToWorld(const Vector3& position, GameObject* gate);
			void AddMazeToWorld(const Vector3& position, const Vector4& colour);
			void AddTowerToWorld(const Vector3& position);
			void AddTargetToWorld(const Vector3& position, bool type);
			void AddBonusObjectToWorld();
			void AddWanderNPCToWorld();
			void AddShootNPCToWorld();
			void AddCannonNPCToWorld();
			void AddSecurityNPCToWorld();
			GameObject* AddPlayerToWorld0(const Vector3& position);
			GameObject* AddEnemyToWorld(const Vector3& position);
			GameObject* AddBonusToWorld(const Vector3& position);

#ifdef USEVULKAN
			GameTechVulkanRenderer*	renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem*		physics;
			GameWorld*			world;

			KeyboardMouseController controller;

			bool useGravity;
			bool inSelectionMode;
			bool flag1;
			bool flag2;
			bool flag3;
			bool flag4;
			bool flag5;
			bool enterLeft;
			bool enterRight;
			int cameraMode;

			float		forceMagnitude;

			vector<Vector3> testNodes;

			GameObject* selectionObject = nullptr;
			GameObject* objClosest = nullptr;
			GameObject* hookObject = nullptr;

			Shader* basicShader = nullptr;

			Mesh*	capsuleMesh   = nullptr;
			Mesh*	cubeMesh	  = nullptr;
			Mesh*	sphereMesh	  = nullptr;
			Mesh*   coneMesh      = nullptr;
			Mesh*   charMesh      = nullptr;
			Mesh*   enemyMesh     = nullptr;
			Mesh*   guideMesh     = nullptr;
			Mesh*   securityMesh  = nullptr;
			Mesh*   bonusMesh     = nullptr;
			Mesh*   treeMesh      = nullptr;
			Mesh*   gooseMesh     = nullptr;

			Texture* basicTex = nullptr;
			Texture* treeTex = nullptr;
			Texture* dogeTex = nullptr;
			Texture* waterTex = nullptr;
			Texture* plankTex = nullptr;

			NavigationGrid* mazeGrid;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(-15, 14, 0);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

			PlayerObject* playerGoat = nullptr;
			PlayerObject* playerGoat2 = nullptr;

			PlayerPowerup* velocityPowerup = nullptr;
			PlayerPowerup* jumpPowerup = nullptr;
			PlayerPowerup* shootPowerup = nullptr;
			PlayerPowerup* hookPowerup = nullptr;
			PlayerPowerup* flyPowerup = nullptr;
			PlayerPowerup* invisibilityPowerup = nullptr;
			PlayerPowerup* gravityPowerup = nullptr;
			GoosePowerup* goosePowerup = nullptr;

			GameObject* mazeWalls = nullptr;

			GameObject* leftGate = nullptr;
			GameObject* rightGate = nullptr;

			GameObject* RespawnPoint = nullptr;

			PushObject* push1 = nullptr;
			PushObject* push2 = nullptr;

			JumpObject* jump1 = nullptr;
			JumpObject* jump2 = nullptr;

			PenaltyObject* penalty1 = nullptr;
			PenaltyObject* penalty2 = nullptr;

			KeyObject* leftKey = nullptr;
			KeyObject* rightKey = nullptr;

			TowerObject* towerObjects = nullptr;

			BonusObject* coins = nullptr;
			BonusObject* hideCoin1 = nullptr;
			BonusObject* hideCoin2 = nullptr;

			ItemObject* item = nullptr;

			WanderObject* wanderNPCs = nullptr;
			ShootObject* shootNPCs = nullptr;
			CannonObject* cannonNPCs = nullptr;
			SecurityObject* securityNPCs = nullptr;
			GooseObject* goose = nullptr;

			static TutorialGame* instance;
		};
	}
}