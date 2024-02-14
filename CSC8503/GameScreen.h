#pragma once
#include "KeyboardMouseController.h"
#include "GameTechRenderer.h"
#include "PhysicsSystem.h"

namespace NCL {
	namespace CSC8503 {
		class GameScreen {
		public:
			GameScreen();
			~GameScreen();

			virtual int UpdateScreen(float dt) { return 0; };

		protected:
			GameObject* AddGoatButton(const Vector3& position, int type);
			GameObject* AddCubeButton(const Vector3& position, int type, const Vector4& colour = Vector4(1, 0, 0, 1));
			GameObject* AddGooseButton(const Vector3& position, int type);

#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem* physics;
			GameWorld* world;

			KeyboardMouseController controller;

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

			GameObject* selectionObject = nullptr;
		};
	}
}