#pragma once
#include "GameScreen.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

GameScreen::GameScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world = new GameWorld();
#ifdef USEVULKAN
	renderer = new GameTechVulkanRenderer(*world);
	renderer->Init();
	renderer->InitStructures();
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics = new PhysicsSystem(*world);

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

	world->GetMainCamera().SetController(controller);
	physics->UseGravity(true);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");

	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	world->GetMainCamera().SetPitch(0.0f);
	world->GetMainCamera().SetYaw(90.0f);
	world->GetMainCamera().SetPosition(Vector3(60, 30, -10));

	world->ClearAndErase();
	physics->Clear();

	selectionObject = nullptr;

	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

GameScreen::~GameScreen() {
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
}

GameObject* GameScreen::AddGoatButton(const Vector3& position, int type) {
	float inverseMass = 1;

	GameObject* goat = new GameObject();
	OBBVolume* volume = new OBBVolume(Vector3(2.5f, 4.2f, 2.5f));

	goat->SetType(type);
	goat->SetBoundingVolume((CollisionVolume*)volume);

	goat->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetPosition(position);

	goat->SetRenderObject(new RenderObject(&goat->GetTransform(), charMesh, nullptr, basicShader, Vector4(0, 0, 1, 1)));
	goat->SetPhysicsObject(new PhysicsObject(&goat->GetTransform(), goat->GetBoundingVolume()));

	goat->GetPhysicsObject()->SetInverseMass(inverseMass);
	goat->GetPhysicsObject()->InitCubeInertia();
	goat->GetPhysicsObject()->SetResolve(false);
	goat->GetPhysicsObject()->SetGravity(false);
	goat->SetInitColour(Vector4(0, 0, 1, 1));

	world->AddGameObject(goat);

	return goat;
}

GameObject* GameScreen::AddGooseButton(const Vector3& position, int type) {
	float inverseMass = 1;

	GameObject* goose = new GameObject();
	OBBVolume* volume = new OBBVolume(Vector3(2.5f, 4.2f, 2.5f));

	goose->SetType(type);
	goose->SetBoundingVolume((CollisionVolume*)volume);

	goose->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetPosition(position);

	goose->SetRenderObject(new RenderObject(&goose->GetTransform(), gooseMesh, nullptr, basicShader, Vector4(1, 0, 1, 1)));
	goose->SetPhysicsObject(new PhysicsObject(&goose->GetTransform(), goose->GetBoundingVolume()));

	goose->GetPhysicsObject()->SetInverseMass(inverseMass);
	goose->GetPhysicsObject()->InitCubeInertia();
	goose->GetPhysicsObject()->SetResolve(false);
	goose->GetPhysicsObject()->SetGravity(false);
	goose->SetInitColour(Vector4(1, 0, 1, 1));

	world->AddGameObject(goose);

	return goose;
}

GameObject* GameScreen::AddCubeButton(const Vector3& position, int type, const Vector4& colour) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(5, 5, 5));
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(Vector3(10, 10, 10));

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, nullptr, basicShader, colour));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(1);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetResolve(false);
	cube->GetPhysicsObject()->SetGravity(false);
	cube->SetInitColour(colour);
	cube->SetType(type);

	world->AddGameObject(cube);

	return cube;
}