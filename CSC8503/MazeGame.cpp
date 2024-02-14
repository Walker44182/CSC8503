#pragma once
#include "MazeGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include "TextureLoader.h"
#include "StateGameObject.h"

using namespace NCL;
using namespace CSC8503;

MazeGame* MazeGame::instance = nullptr;

MazeGame::MazeGame() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
	renderer->Init();
	renderer->InitStructures();
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics = new PhysicsSystem(*world);

	mazeGrid = new NavigationGrid("MazeGrid2.txt");

	finished = false;

	world->GetMainCamera().SetController(controller);
	physics->UseGravity(true);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");

	InitialiseAssets();
	InitCamera();
	InitWorld();
	srand(time(0));

	instance = this;
}

void MazeGame::InitialiseAssets() {
	cubeMesh = renderer->LoadMesh("cube.msh");
	sphereMesh = renderer->LoadMesh("sphere.msh");
	charMesh = renderer->LoadMesh("goat.msh");
	enemyMesh = renderer->LoadMesh("Keeper.msh");
	guideMesh = renderer->LoadMesh("courier.msh");
	securityMesh = renderer->LoadMesh("security.msh");
	bonusMesh = renderer->LoadMesh("coin.msh");
	capsuleMesh = renderer->LoadMesh("capsule.msh");
	coneMesh = renderer->LoadMesh("Cone.msh");
	treeMesh = renderer->LoadMesh("tree.msh");
	gooseMesh = renderer->LoadMesh("goose.msh");

	basicTex = renderer->LoadTexture("checkerboard.png");
	treeTex = renderer->LoadTexture("tree_diffuse.png");
	dogeTex = renderer->LoadTexture("Default.png");
	waterTex = renderer->LoadTexture("water.tga");
	plankTex = renderer->LoadTexture("birch_planks.png");

	basicShader = renderer->LoadShader("scene.vert", "scene.frag");
}

void MazeGame::InitCamera() {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(700.0f);
	world->GetMainCamera().SetPitch(-90.0f);
	world->GetMainCamera().SetYaw(-90.0f);
	world->GetMainCamera().SetPosition(Vector3(630, 550, 0));
}

void MazeGame::InitWorld() {
	AddFloorToWorld(Vector3(630, 0, 0), Vector3(190, 2, 180), Vector4(1, 0.6f, 0.8f, 1));
	AddWallToWorld(Vector3(630, 30, 180), Vector3(190, 30, 2), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddWallToWorld(Vector3(630, 30, -180), Vector3(190, 30, 2), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddWallToWorld(Vector3(820, 30, 0), Vector3(2, 30, 180), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddWallToWorld(Vector3(440, 30, 0), Vector3(2, 30, 180), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddFinishPoint(Vector3(450, 2, 170), Vector4(0, 0, 1, 1), 26);
	AddFinishPoint(Vector3(450, 2, -170), Vector4(1, 1, 0, 1), 27);
	AddMazeToWorld(Vector3(430, -2.5f, 0), Vector4(0, 0.98f, 0.60f, 1));
	AddBonusObjectToWorld(false);
	player1 = AddPlayerToWorld(Vector3(450, -1000, -100), Vector4(0, 0, 1, 1), "player1", 1);
	player2 = AddPlayerToWorld(Vector3(450, 30, 100), Vector4(1, 1, 0, 1), "player2", 2);
	item = AddItemToWorld(false);
	goose = AddGooseToWorld(false);
}

MazeGame::~MazeGame() {
	delete  capsuleMesh;
	delete	cubeMesh;
	delete	sphereMesh;
	delete	coneMesh;
	delete	charMesh;
	delete	enemyMesh;
	delete	guideMesh;
	delete	securityMesh;
	delete	bonusMesh;
	delete	treeMesh;
	delete	gooseMesh;

	delete  basicTex;
	delete	treeTex;
	delete	dogeTex;
	delete	waterTex;
	delete	plankTex;

	delete basicShader;

	delete physics;
	delete renderer;
	delete world;

	if (instance = this) instance = nullptr;
}

void MazeGame::UpdateGame(float dt) {
	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void MazeGame::GoatMovement(MazePlayer* player) {
	if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		player->GetPhysicsObject()->AddForce(Vector3(0, 0, -100));
		player->GetTransform().SetOrientation(
			Quaternion::EulerAnglesToQuaternion(0, 0, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		player->GetPhysicsObject()->AddForce(Vector3(0, 0, 100));
		player->GetTransform().SetOrientation(
			Quaternion::EulerAnglesToQuaternion(0, 180, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		player->GetPhysicsObject()->AddForce(Vector3(100, 0, 0));
		player->GetTransform().SetOrientation(
			Quaternion::EulerAnglesToQuaternion(0, -90, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		player->GetPhysicsObject()->AddForce(Vector3(-100, 0, 0));
		player->GetTransform().SetOrientation(
			Quaternion::EulerAnglesToQuaternion(0, 90, 0));
	}
}

GameObject* MazeGame::AddFloorToWorld(const Vector3& position, const Vector3& size, const Vector4& colour) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = size;
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader, colour));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	floor->SetType(100);

	world->AddGameObject(floor);

	return floor;
}

GameObject* MazeGame::AddRespawnPointToWorld() {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(10, 2, 10);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(Vector3(-130, 2, 0));

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader, Vector4(0.2f, 0.2f, 0.2f, 1)));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	floor->SetType(101);

	world->AddGameObject(floor);

	return floor;
}

GameObject* MazeGame::AddWallToWorld(const Vector3& position, const Vector3& size, const Vector4& colour) {
	GameObject* wall = new GameObject();

	Vector3 wallSize = size;
	AABBVolume* volume = new AABBVolume(wallSize);
	wall->SetBoundingVolume((CollisionVolume*)volume);
	wall->GetTransform()
		.SetScale(wallSize * 2)
		.SetPosition(position);

	wall->SetRenderObject(new RenderObject(&wall->GetTransform(), cubeMesh, basicTex, basicShader, colour));
	wall->SetPhysicsObject(new PhysicsObject(&wall->GetTransform(), wall->GetBoundingVolume()));

	wall->GetPhysicsObject()->SetInverseMass(0);
	wall->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(wall);

	return wall;
}

void MazeGame::AddMazeToWorld(const Vector3& position, const Vector4& colour) {
	Vector3 positions[17] = {
	Vector3(40, 20, -20),
	Vector3(80, 20, 30),
	Vector3(180, 20, -90),
	Vector3(180, 20, 130),
	Vector3(220, 20, -120),
	Vector3(220, 20, 50),
	Vector3(260, 20, -40),
	Vector3(300, 20, 60),
	Vector3(340, 20, -10),
	Vector3(100, 20, -110),
	Vector3(130, 20, -30),
	Vector3(130, 20, 90),
	Vector3(160, 20, 30),
	Vector3(270, 20, -150),
	Vector3(300, 20, -90),
	Vector3(260, 20, 90),
	Vector3(320, 20, 130),
	};
	Vector3 dimensions[17]{
	Vector3(10, 15, 120),
	Vector3(10, 15, 70),
	Vector3(10, 15, 70),
	Vector3(10, 15, 50),
	Vector3(10, 15, 40),
	Vector3(10, 15, 90),
	Vector3(10, 15, 60),
	Vector3(10, 15, 80),
	Vector3(10, 15, 150),
	Vector3(50, 15, 10),
	Vector3(40, 15, 10),
	Vector3(40, 15, 10),
	Vector3(50, 15, 10),
	Vector3(40, 15, 10),
	Vector3(30, 15, 10),
	Vector3(30, 15, 10),
	Vector3(10, 15, 10),
	};

	mazeWalls = new GameObject[17];
	for (int i = 0; i < 17; ++i) {
		GameObject& wall = mazeWalls[i];
		Vector3 wallSize = dimensions[i];
		AABBVolume* volume = new AABBVolume(wallSize);
		wall.SetBoundingVolume((CollisionVolume*)volume);
		wall.GetTransform()
			.SetScale(wallSize * 2)
			.SetPosition(position + positions[i]);

		wall.SetRenderObject(new RenderObject(&wall.GetTransform(), cubeMesh, basicTex, basicShader, colour));
		wall.SetPhysicsObject(new PhysicsObject(&wall.GetTransform(), wall.GetBoundingVolume()));

		wall.GetPhysicsObject()->SetInverseMass(0);
		wall.GetPhysicsObject()->InitCubeInertia();

		world->AddGameObject(&mazeWalls[i]);
	}
}

MazePlayer* MazeGame::AddPlayerToWorld(const Vector3& position, const Vector4& colour, std::string name, int id, bool spawn) {
	float inverseMass = 1;
	Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);

	MazePlayer* goat = new MazePlayer(name);
	AABBVolume* volume = new AABBVolume(Vector3(2.5f, 4.2f, 2.5f));

	goat->SetType(1);
	goat->SetPlayerID(id);
	goat->SetBoundingVolume((CollisionVolume*)volume);

	if (spawn) goat->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetPosition(position)
		.SetOrientation(rotation);
	else        goat->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetOrientation(rotation);

	goat->SetRenderObject(new RenderObject(&goat->GetTransform(), charMesh, nullptr, basicShader, colour));
	goat->SetPhysicsObject(new PhysicsObject(&goat->GetTransform(), goat->GetBoundingVolume()));
	goat->SetNetworkObject(new NetworkObject(*goat, id));

	goat->SetInitColour(colour);

	goat->GetPhysicsObject()->SetInverseMass(inverseMass);
	goat->GetPhysicsObject()->InitCubeInertia();
	goat->GetNetworkObject()->SetPlayer(goat);

	world->AddGameObject(goat);

	return goat;
}

void MazeGame::AddBonusObjectToWorld(bool spawn) {
	Vector3 positions[20] = {
		Vector3(460, 10, -150),
		Vector3(480, 10, -150),
		Vector3(500, 10, -150),
		Vector3(520, 10, -150),
		Vector3(540, 10, -150),
		Vector3(460, 10, 150),
		Vector3(480, 10, 150),
		Vector3(500, 10, 150),
		Vector3(520, 10, 150),
		Vector3(540, 10, 150),
		Vector3(620, 10, 0),
		Vector3(600, 10, 0),
		Vector3(580, 10, 0),
		Vector3(560, 10, 0),
		Vector3(540, 10, 0),
		Vector3(620, 10, 60),
		Vector3(600, 10, 60),
		Vector3(580, 10, 60),
		Vector3(560, 10, 60),
		Vector3(540, 10, 60)
	};
	coins = new MazeBonus[20];

	for (int i = 0; i < 20; ++i) {
		MazeBonus& coin = coins[i];

		AABBVolume* volume = new AABBVolume(Vector3(0.25f, 2, 0.25f));
		coin.SetBoundingVolume((CollisionVolume*)volume);

		if (spawn) {
			coin.GetTransform()
				.SetScale(Vector3(0.5, 0.5, 0.5))
				.SetPosition(positions[i]);
		}
		else {
			coin.GetTransform()
				.SetScale(Vector3(0.5, 0.5, 0.5))
				.SetPosition(Vector3(positions[i].x, positions[i].y - 1000, positions[i].z));
		}

		coin.SetRenderObject(new RenderObject(&coin.GetTransform(), bonusMesh, nullptr, basicShader, Vector4(1, 1, 0, 1)));
		coin.SetPhysicsObject(new PhysicsObject(&coin.GetTransform(), coin.GetBoundingVolume()));
		coin.SetNetworkObject(new NetworkObject(coin, 4 + i));

		coin.GetPhysicsObject()->SetInverseMass(1.0f);
		coin.GetPhysicsObject()->InitSphereInertia();
		coin.GetPhysicsObject()->SetGravity(false);
		coin.GetPhysicsObject()->SetResolve(false);

		coin.SetType(20);
		coin.SetHooked(false);

		world->AddGameObject(&coins[i]);
	}
}

MazeGoose* MazeGame::AddGooseToWorld(bool spawn) {
	float inverseMass = 1;
	Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);

	MazeGoose* goose = new MazeGoose();
	AABBVolume* volume = new AABBVolume(Vector3(2.5f, 5.0f, 2.5f));

	goose->SetType(39);
	goose->SetBoundingVolume((CollisionVolume*)volume);

	if (spawn) {
		goose->GetTransform()
			.SetScale(Vector3(5, 5, 5))
			.SetPosition(Vector3(800, 10, 0))
			.SetOrientation(rotation);
	}
	else {
		goose->GetTransform()
			.SetScale(Vector3(5, 5, 5))
			.SetPosition(Vector3(800, -1000, 0))
			.SetOrientation(rotation);
	}

	goose->SetRenderObject(new RenderObject(&goose->GetTransform(), gooseMesh, nullptr, basicShader, Vector4(1, 0, 0, 1)));
	goose->SetPhysicsObject(new PhysicsObject(&goose->GetTransform(), goose->GetBoundingVolume()));
	goose->SetNetworkObject(new NetworkObject(*goose, 3));
	goose->SetItemObject(item);

	goose->GetPhysicsObject()->SetInverseMass(inverseMass);
	goose->GetPhysicsObject()->InitCubeInertia();

	goose->SetShot(true);
	goose->SetHooked(false);
	goose->SetInitPosition(Vector3(800, 10, 0));

	world->AddGameObject(goose);

	return goose;
}

MazeItem* MazeGame::AddItemToWorld(bool spawn) {
	MazeItem* item = new MazeItem();
	AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));

	item->SetBoundingVolume((CollisionVolume*)volume);

	if (spawn) {
		item->GetTransform()
			.SetScale(Vector3(2, 2, 2))
			.SetPosition(Vector3(800, 10, 0))
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(-90, 0, 0));
	}
	else {
		item->GetTransform()
			.SetScale(Vector3(2, 2, 2))
			.SetPosition(Vector3(800, -1000, 0))
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(-90, 0, 0));
	}

	item->SetRenderObject(new RenderObject(&item->GetTransform(), cubeMesh, nullptr, basicShader, Vector4(0, 1, 1, 1)));
	item->SetPhysicsObject(new PhysicsObject(&item->GetTransform(), item->GetBoundingVolume()));
	item->SetNetworkObject(new NetworkObject(*item, 25));

	item->GetPhysicsObject()->InitCubeInertia();
	item->GetPhysicsObject()->SetGravity(false);
	item->GetPhysicsObject()->SetResolve(false);
	item->GetPhysicsObject()->SetChannel(3);

	item->SetType(30);

	world->AddGameObject(item);

	return item;
}

GameObject* MazeGame::AddFinishPoint(const Vector3& position, const Vector4& colour, int id) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(10, 2, 10);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader, colour));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	floor->SetType(id);

	world->AddGameObject(floor);

	return floor;
}