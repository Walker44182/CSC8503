#pragma once
#include "KeyboardMouseController.h"
#include "GameTechRenderer.h"
#include "PhysicsSystem.h"
#include "NavigationGrid.h"
#include "NavigationMesh.h"
#include "StateGameObject.h"
#include "PlayerObject.h"
#include "BonusObject.h"
#include "GooseObject.h"
#include "ItemObject.h"

namespace NCL {
	namespace CSC8503 {
		class MazeGame {
		public:
			MazeGame();
			~MazeGame();

			virtual void UpdateGame(float dt);

			static MazeGame* GetGame() {
				return instance;
			}

			GameWorld* GetGameWorld() const {
				return world;
			};

			MazePlayer* GetPlayer1() {
				return player1;
			};
			MazePlayer* GetPlayer2() {
				return player2;
			};

			bool HasFinished() const {
				return finished;
			};

		protected:
			void InitialiseAssets();

			void InitCamera();
			void InitWorld();
			void UpdateKeys() {};

			void GoatMovement(MazePlayer* player);

			GameObject* AddFloorToWorld(const Vector3& position, const Vector3& size, const Vector4& colour);
			GameObject* AddRespawnPointToWorld();
			GameObject* AddWallToWorld(const Vector3& position, const Vector3& size, const Vector4& colour);
			GameObject* AddFinishPoint(const Vector3& position, const Vector4& colour, int id);
			MazePlayer* AddPlayerToWorld(const Vector3& position, const Vector4& colour, std::string name, int id, bool spawn = false);
			MazeGoose* AddGooseToWorld(bool spawn = false);
			MazeItem* AddItemToWorld(bool spawn = false);
			void AddMazeToWorld(const Vector3& position, const Vector4& colour); 
			void AddBonusObjectToWorld(bool spawn = false);

#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem* physics;
			GameWorld* world;

			KeyboardMouseController controller;

			vector<Vector3> testNodes;

			Shader* basicShader = nullptr;

			Mesh* capsuleMesh = nullptr;
			Mesh* cubeMesh = nullptr;
			Mesh* sphereMesh = nullptr;
			Mesh* coneMesh = nullptr;
			Mesh* charMesh = nullptr;
			Mesh* enemyMesh = nullptr;
			Mesh* guideMesh = nullptr;
			Mesh* securityMesh = nullptr;
			Mesh* bonusMesh = nullptr;
			Mesh* treeMesh = nullptr;
			Mesh* gooseMesh = nullptr;

			Texture* basicTex = nullptr;
			Texture* treeTex = nullptr;
			Texture* dogeTex = nullptr;
			Texture* waterTex = nullptr;
			Texture* plankTex = nullptr;

			NavigationGrid* mazeGrid;

			GameObject* mazeWalls = nullptr;

			MazePlayer* player1 = nullptr;
			MazePlayer* player2 = nullptr;

			MazeGoose* goose = nullptr;

			MazeBonus* coins = nullptr;
			MazeItem* item = nullptr;

			static MazeGame* instance;

			bool finished;
		};
	}
}
