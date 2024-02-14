#pragma once
#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "HingeConstraint.h"
#include "StateGameObject.h"

using namespace NCL;
using namespace CSC8503;

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject();

	Vector3 floorSize = Vector3(200, 2, 200);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(floor);

	return floor;
}

GameObject* TutorialGame::AddFloorToWorld(const Vector3& position, const Vector3& size, const Vector4& colour) {
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

GameObject* TutorialGame::AddRespawnPointToWorld() {
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

GameObject* TutorialGame::AddWallToWorld(const Vector3& position, const Vector3& size, const Vector4& colour) {
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

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple'
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject();

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddPoolToWorld(const Vector3& position, const Vector3& dimensions) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, waterTex, basicShader, Vector4(0,1,1,1)));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetType(38);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddPlankToWorld(const Vector3& position, const Vector3& dimensions) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, plankTex, basicShader, Vector4(1, 1, 1, 1)));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetFriction(2);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddTriggerToWorld(const Vector3& position, const Vector3& dimensions, int objectType) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(nullptr);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetGravity(false);
	cube->GetPhysicsObject()->SetResolve(false);

	cube->SetType(objectType);
	cube->SetTrigger(true);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddAirWallToWorld(const Vector3& position, const Vector3& dimensions) {
	GameObject* cube = new GameObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(nullptr);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetGravity(false);

	cube->SetType(50);
	cube->SetTrigger(true);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddGuideToWorld(const Vector3& position, int objectType) {
	float meshSize = 10.0f;
	float inverseMass = 0;
	Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);

	GameObject* guide = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	guide->SetBoundingVolume((CollisionVolume*)volume);

	guide->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position)
		.SetOrientation(rotation);

	guide->SetRenderObject(new RenderObject(&guide->GetTransform(), guideMesh, nullptr, basicShader, Vector4(0.24f, 0.70f, 0.44f, 1)));
	guide->SetPhysicsObject(new PhysicsObject(&guide->GetTransform(), guide->GetBoundingVolume()));

	guide->GetPhysicsObject()->SetInverseMass(inverseMass);
	guide->GetPhysicsObject()->InitSphereInertia();
	guide->GetPhysicsObject()->SetChannel(3);

	guide->SetType(objectType);
	guide->SetHooked(false);

	world->AddGameObject(guide);

	return guide;
}

GameObject* TutorialGame::AddPlayerToWorld0(const Vector3& position) {
	float meshSize = 1.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();
	SphereVolume* volume = new SphereVolume(1.0f);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), charMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position) {
	GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(0.5, 0.5, 0.5))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

TreeObject* TutorialGame::AddTreeToWorld(const Vector3& position, const Vector3& size) {
	float inverseMass = 0.4f;

	TreeObject* tree = new TreeObject();

	OBBVolume* volume = new OBBVolume(Vector3(0.3f, 1, 0.3f) * size);
	tree->SetBoundingVolume((CollisionVolume*)volume);

	tree->GetTransform()
		.SetScale(size)
		.SetPosition(position);

	tree->SetRenderObject(new RenderObject(&tree->GetTransform(), treeMesh, treeTex, basicShader));
	tree->SetPhysicsObject(new PhysicsObject(&tree->GetTransform(), tree->GetBoundingVolume()));

	tree->GetPhysicsObject()->SetInverseMass(inverseMass);
	tree->GetPhysicsObject()->InitSphereInertia();

	tree->SetType(11);

	world->AddGameObject(tree);

	return tree;
}

PlayerObject* TutorialGame::AddPlayerToWorld(const Vector3& position) {
	float inverseMass = 1;
	Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);

	PlayerObject* goat = new PlayerObject("goat");
	OBBVolume* volume = new OBBVolume(Vector3(2.5f, 4.2f, 2.5f));

	goat->SetType(1);
	goat->SetBoundingVolume((CollisionVolume*)volume);

	goat->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetPosition(position)
		.SetOrientation(rotation);

	goat->SetRenderObject(new RenderObject(&goat->GetTransform(), charMesh, nullptr, basicShader, Vector4(0,0,1,1)));
	goat->SetPhysicsObject(new PhysicsObject(&goat->GetTransform(), goat->GetBoundingVolume()));

	goat->GetPhysicsObject()->SetInverseMass(inverseMass);
	goat->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(goat);

	return goat;
}

PlayerPowerup* TutorialGame::AddPlayerPowerupToWorld(const Vector3& position, int objectType, std::string objectName, const Vector4& colour) {
	PlayerPowerup* pill = new PlayerPowerup(objectName);
	AABBVolume* volume = new AABBVolume(Vector3(1, 2, 1));

	pill->SetType(objectType);
	pill->SetBoundingVolume((CollisionVolume*)volume);
	pill->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(position);
	pill->SetInitPosition(position);

	pill->SetRenderObject(new RenderObject(&pill->GetTransform(), capsuleMesh, nullptr, basicShader, colour));
	pill->SetPhysicsObject(new PhysicsObject(&pill->GetTransform(), pill->GetBoundingVolume()));
	pill->SetPlayerObject(playerGoat);
	pill->SetColour(colour);

	pill->GetPhysicsObject()->InitCubeInertia();
	pill->GetPhysicsObject()->SetGravity(false);
	pill->GetPhysicsObject()->SetResolve(false);
	pill->GetPhysicsObject()->SetChannel(3);
	pill->SetTrigger(true);

	world->AddGameObject(pill);

	return pill;
}

GoosePowerup* TutorialGame::AddGoosePowerupToWorld() {
	GoosePowerup* pill = new GoosePowerup();
	AABBVolume* volume = new AABBVolume(Vector3(15, 2, 15));

	pill->SetType(38);
	pill->SetBoundingVolume((CollisionVolume*)volume);
	pill->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(Vector3(800, 10, 160));
	pill->SetInitPosition(Vector3(800, 10, 160));

	pill->SetRenderObject(new RenderObject(&pill->GetTransform(), capsuleMesh, nullptr, basicShader, Vector4(1,1,1,1)));
	pill->SetPhysicsObject(new PhysicsObject(&pill->GetTransform(), pill->GetBoundingVolume()));
	pill->SetGooseObject(goose);

	pill->GetPhysicsObject()->InitCubeInertia();
	pill->GetPhysicsObject()->SetGravity(false);
	pill->GetPhysicsObject()->SetResolve(false);
	pill->GetPhysicsObject()->SetChannel(3);
	pill->SetHooked(false);

	world->AddGameObject(pill);

	return pill;
}

BridgeObject* TutorialGame::AddBridgeCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass) {
	BridgeObject* cube = new BridgeObject();

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->SetType(100);

	world->AddGameObject(cube);

	return cube;
}

GameObject* TutorialGame::AddGateCubeToWorld(const Vector3& position, Vector3 dimensions, const Vector4& colour) {
	GameObject* gate = new GameObject();

	OBBVolume* gateVolume = new OBBVolume(dimensions);
	gate->SetBoundingVolume((CollisionVolume*)gateVolume);

	gate->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	gate->SetRenderObject(new RenderObject(&gate->GetTransform(), cubeMesh, basicTex, basicShader, colour));
	gate->SetPhysicsObject(new PhysicsObject(&gate->GetTransform(), gate->GetBoundingVolume()));

	gate->GetPhysicsObject()->SetInverseMass(0);
	gate->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(gate);

	return gate;
}

PushObject* TutorialGame::AddPushObjectToWorld(const Vector3& position) {
	PushObject* capsule = new PushObject();
	Vector3 dimensions = Vector3(5, 15, 5);

	AABBVolume* volume = new AABBVolume(Vector3(5, 5, 30));
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(90, 0, 0));

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(0);
	capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;
}

JumpObject* TutorialGame::AddJumpObjectToWorld(const Vector3& position) {
	JumpObject* cube = new JumpObject();
	Vector3 dimensions = Vector3(10, 1.5f, 10);

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->SetElasticity(18);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->SetType(26);

	world->AddGameObject(cube);

	return cube;
}

PenaltyObject* TutorialGame::AddPenaltyObjectToWorld(const Vector3& position, int objectType) {
	PenaltyObject* cube = new PenaltyObject();
	Vector3 dimensions = Vector3(10, 10, 10);

	AABBVolume* volume = new AABBVolume(dimensions);
	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, dogeTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(0);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetChannel(2);

	cube->SetType(objectType);

	world->AddGameObject(cube);

	return cube;
}

KeyObject* TutorialGame::AddKeyObjectToWorld(const Vector3& position, int objectType, std::string objectName, const Vector4& colour) {
	KeyObject* key = new KeyObject(objectName);
	AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));

	key->SetType(objectType);
	key->SetBoundingVolume((CollisionVolume*)volume);
	key->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(position)
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(-90, 0, 0));
	key->SetInitPosition(position);

	key->SetRenderObject(new RenderObject(&key->GetTransform(), coneMesh, nullptr, basicShader, colour));
	key->SetPhysicsObject(new PhysicsObject(&key->GetTransform(), key->GetBoundingVolume()));
	key->SetPlayerObject(playerGoat);

	key->GetPhysicsObject()->InitCubeInertia();
	key->GetPhysicsObject()->SetGravity(false);
	key->GetPhysicsObject()->SetResolve(false);
	key->SetHooked(false);
	key->GetPhysicsObject()->SetChannel(3);

	world->AddGameObject(key);

	return key;
}

BonusObject* TutorialGame::AddHiddenBonusObjectToWorld(const Vector3& position) {
	BonusObject* coin = new BonusObject();

	AABBVolume* volume = new AABBVolume(Vector3(0.25f, 2, 0.25f));
	coin->SetBoundingVolume((CollisionVolume*)volume);
	coin->GetTransform()
		.SetScale(Vector3(0.5, 0.5, 0.5))
		.SetPosition(position);

	coin->SetRenderObject(new RenderObject(&coin->GetTransform(), bonusMesh, nullptr, basicShader, Vector4(1, 1, 0, 1)));
	coin->SetPhysicsObject(new PhysicsObject(&coin->GetTransform(), coin->GetBoundingVolume()));

	coin->GetPhysicsObject()->SetInverseMass(1.0f);
	coin->GetPhysicsObject()->InitSphereInertia();
	coin->GetPhysicsObject()->SetGravity(false);
	coin->GetPhysicsObject()->SetResolve(false);

	coin->SetType(20);
	coin->SetHooked(false);

	world->AddGameObject(coin);

	return coin;
}

ItemObject* TutorialGame::AddItemObjectToWorld() {
	ItemObject* item = new ItemObject();
	AABBVolume* volume = new AABBVolume(Vector3(1, 1, 1));

	item->SetBoundingVolume((CollisionVolume*)volume);
	item->GetTransform()
		.SetScale(Vector3(2, 2, 2))
		.SetPosition(Vector3(800, 10, 0))
		.SetOrientation(Quaternion::EulerAnglesToQuaternion(-90, 0, 0));
	item->SetInitPosition(Vector3(800, 10, 0));

	item->SetRenderObject(new RenderObject(&item->GetTransform(), cubeMesh, nullptr, basicShader, Vector4(0, 1, 1, 1)));
	item->SetPhysicsObject(new PhysicsObject(&item->GetTransform(), item->GetBoundingVolume()));
	item->SetPlayerObject(playerGoat);

	item->GetPhysicsObject()->InitCubeInertia();
	item->GetPhysicsObject()->SetGravity(false);
	item->GetPhysicsObject()->SetResolve(false);
	item->SetHooked(false);
	item->GetPhysicsObject()->SetChannel(3);

	item->SetType(30);

	world->AddGameObject(item);

	return item;
}

BombObject* TutorialGame::AddBombObjectToWorld(const Vector3& position, int type) {
	if (type == 0) {
		BombObject* cube = new BombObject();
		float halfLength = RandomValue(2, 4);
		Vector3 dimensions = Vector3(halfLength, halfLength, halfLength);
		Vector4 colour = Vector4(RandomValue(0, 1), RandomValue(0, 1), RandomValue(0, 1), 1);

		OBBVolume* volume = new OBBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);

		cube->GetTransform()
			.SetPosition(position)
			.SetScale(dimensions * 2);

		cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, nullptr, basicShader, colour));
		cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

		cube->GetPhysicsObject()->SetInverseMass(2);
		cube->GetPhysicsObject()->InitCubeInertia();

		cube->SetType(37);
		cube->SetShot(true);
		cube->SetHooked(false);

		world->AddGameObject(cube);

		return cube;
	}
	if (type == 1) {
		BombObject* sphere = new BombObject();
		float radius = RandomValue(2, 4);
		Vector3 sphereSize = Vector3(radius, radius, radius);
		Vector4 colour = Vector4(RandomValue(0, 1), RandomValue(0, 1), RandomValue(0, 1), 1);

		SphereVolume* volume = new SphereVolume(radius);
		sphere->SetBoundingVolume((CollisionVolume*)volume);

		sphere->GetTransform()
			.SetScale(sphereSize)
			.SetPosition(position);

		sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, nullptr, basicShader, colour));
		sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

		sphere->GetPhysicsObject()->SetInverseMass(2);
		sphere->GetPhysicsObject()->InitSphereInertia();

		sphere->SetType(37);
		sphere->SetShot(true);

		world->AddGameObject(sphere);

		return sphere;
	}
	if (type == 2) {
		BombObject* capsule = new BombObject();
		float halfLength = RandomValue(2, 3);
		Vector3 dimensions = Vector3(halfLength, halfLength, halfLength);
		Vector4 colour = Vector4(RandomValue(0, 1), RandomValue(0, 1), RandomValue(0, 1), 1);

		OBBVolume* volume = new OBBVolume(Vector3(halfLength, 2 * halfLength, halfLength));
		capsule->SetBoundingVolume((CollisionVolume*)volume);

		capsule->GetTransform()
			.SetScale(dimensions * 2)
			.SetPosition(position)
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 90));

		capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, nullptr, basicShader, colour));
		capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

		capsule->GetPhysicsObject()->SetInverseMass(2);
		capsule->GetPhysicsObject()->InitCubeInertia();

		capsule->SetType(37);
		capsule->SetShot(true);

		world->AddGameObject(capsule);

		return capsule;
	}
}

GooseObject* TutorialGame::AddGooseToWorld() {
	float inverseMass = 1;
	Quaternion rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);

	GooseObject* goose = new GooseObject();
	OBBVolume* volume = new OBBVolume(Vector3(2.5f, 5.0f, 2.5f));

	goose->SetType(39);
	goose->SetBoundingVolume((CollisionVolume*)volume);

	goose->GetTransform()
		.SetScale(Vector3(5, 5, 5))
		.SetPosition(Vector3(800, 10, 0))
		.SetOrientation(rotation);

	goose->SetRenderObject(new RenderObject(&goose->GetTransform(), gooseMesh, nullptr, basicShader, Vector4(1, 0, 0, 1)));
	goose->SetPhysicsObject(new PhysicsObject(&goose->GetTransform(), goose->GetBoundingVolume()));

	goose->GetPhysicsObject()->SetInverseMass(inverseMass);
	goose->GetPhysicsObject()->InitCubeInertia();

	goose->SetShot(true);
	goose->SetHooked(false);
	goose->SetPlayerObject(playerGoat);
	goose->SetInitPosition(Vector3(800, 10, 0));

	world->AddGameObject(goose);

	return goose;
}

void TutorialGame::AddRopeBridgeToWorld(const Vector3& sPos, const Vector3& ePos) {
	Vector3 cubeSize = Vector3(35, 4, 8);

	float invCubeMass = 5;
	int numLinks = 10;
	float maxDistance = 29;
	float cubeDistance = 10;

	Vector3 startPos = sPos;
	Vector3 endPos = ePos;

	BridgeObject* start = AddBridgeCubeToWorld(startPos, cubeSize, 0);
	BridgeObject* end = AddBridgeCubeToWorld(endPos, cubeSize, 0);

	BridgeObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		BridgeObject* block = AddBridgeCubeToWorld(startPos + Vector3(0, 0, (i + 1) *
			cubeDistance), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous,
			block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous,
		end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::AddGateToWorld(const Vector3& position, GameObject* gate) {
	GameObject* hinge = new GameObject();

	Vector3 hingeSize = Vector3(1, 1, 1);
	AABBVolume* hingeVolume = new AABBVolume(hingeSize);
	hinge->SetBoundingVolume((CollisionVolume*)hingeVolume);
	hinge->GetTransform()
		.SetScale(hingeSize * 2)
		.SetPosition(position - Vector3(0, 10, 0));

	hinge->SetRenderObject(nullptr);
	hinge->SetPhysicsObject(new PhysicsObject(&hinge->GetTransform(), hinge->GetBoundingVolume()));
	hinge->GetPhysicsObject()->SetResolve(false);

	hinge->GetPhysicsObject()->SetInverseMass(0);
	hinge->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(hinge);

	HingeConstraint* hingeConstraint = new HingeConstraint(hinge, gate, 1);
	world->AddConstraint(hingeConstraint);
}

void TutorialGame::AddMazeToWorld(const Vector3& position, const Vector4& colour) {
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

void TutorialGame::AddTowerToWorld(const Vector3& position) {
	Vector3 positions[8] = {
		Vector3(0, 20, 25),
		Vector3(0, 20, -50),
		Vector3(0 ,40, -12.5f),
		Vector3(0, 63, 12.5f),
		Vector3(0, 63, -37.5f),
		Vector3(0, 83, -12.5f),
		Vector3(0, 73, -12.5f),
		Vector3(0, 100, -12.5f)
	};
	Vector3 dimensions[7] = {
		Vector3(5, 20, 5),
		Vector3(5, 20, 5),
		Vector3(5, 5, 50),
		Vector3(5, 20, 5),
		Vector3(5, 20, 5),
		Vector3(5, 5, 40),
		Vector3(7.5f, 7.5f, 7.5f),
	};
	int tag[8] = { 1,1,1,1,1,1,1,2 };

	towerObjects = new TowerObject[8];
	for (int i = 0; i < 8; ++i) {
		TowerObject& obj = towerObjects[i];
		int objTag = tag[i];
		if (objTag != 2) {
			Vector3 objSize = dimensions[i];
			OBBVolume* volume = new OBBVolume(objSize);
			obj.SetBoundingVolume((CollisionVolume*)volume);
			obj.GetTransform()
				.SetScale(objSize * 2)
				.SetPosition(position + positions[i]);

			obj.SetRenderObject(new RenderObject(&obj.GetTransform(), cubeMesh, basicTex, basicShader));
			obj.SetPhysicsObject(new PhysicsObject(&obj.GetTransform(), obj.GetBoundingVolume()));

			obj.GetPhysicsObject()->SetInverseMass(1);
			obj.GetPhysicsObject()->InitCubeInertia();

			obj.SetType(12);

			world->AddGameObject(&towerObjects[i]);
		}
		else {
			Vector3 objSize = Vector3(7.5f, 7.5f, 7.5f);
			SphereVolume* volume = new SphereVolume(7.5f);
			obj.SetBoundingVolume((CollisionVolume*)volume);

			obj.GetTransform()
				.SetScale(objSize)
				.SetPosition(position + positions[i]);

			obj.SetRenderObject(new RenderObject(&obj.GetTransform(), sphereMesh, basicTex, basicShader, Vector4(0, 1, 1, 1)));
			obj.SetPhysicsObject(new PhysicsObject(&obj.GetTransform(), obj.GetBoundingVolume()));

			obj.GetPhysicsObject()->SetInverseMass(1);
			obj.GetPhysicsObject()->InitSphereInertia();

			obj.SetType(13);

			world->AddGameObject(&towerObjects[i]);
		}
	}
}

void TutorialGame::AddTargetToWorld(const Vector3& position, bool objectType) {
	Vector3 positions[9] = {
		Vector3(0, 0, 0),
		Vector3(0, 0, 25),
		Vector3(0, 0, -25),
		Vector3(0, 25, 0),
		Vector3(0, -25, 0),
		Vector3(0, 25, 25),
		Vector3(0, 25, -25),
		Vector3(0, -25, 25),
		Vector3(0, -25, -25)
	};
	int type[9] = { -1,17,17,17,17,17,17,17,17 };
	if (objectType) type[0] = 15;
	else type[0] = 16;
	Vector3 dimensions = Vector3(10, 10, 10);

	for (int i = 0; i < 9; ++i) {
		TargetObject* cube = new TargetObject();

		OBBVolume* volume = new OBBVolume(dimensions);
		cube->SetBoundingVolume((CollisionVolume*)volume);

		cube->GetTransform()
			.SetPosition(position + positions[i])
			.SetScale(dimensions * 2);

		if (type[i] == 15)
			cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader, Vector4(0.24f, 0.70f, 0.55f, 1)));
		if (type[i] == 16)
			cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader, Vector4(1, 0, 1, 1)));
		if (type[i] == 17)
			cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));

		cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

		cube->GetPhysicsObject()->SetInverseMass(1);
		cube->GetPhysicsObject()->InitCubeInertia();
		cube->GetPhysicsObject()->SetGravity(false);
		cube->GetPhysicsObject()->SetResolve(false);
		cube->SetShot(true);
		cube->SetHooked(false);
		cube->SetType(type[i]);

		world->AddGameObject(cube);
	}
}

void TutorialGame::AddBonusObjectToWorld() {
	Vector3 positions[30] = {
		Vector3(50, 10, -150),
		Vector3(65, 10, -150),
		Vector3(80, 10, -150),
		Vector3(95, 10, -150),
		Vector3(110, 10, -150),
		Vector3(50, 10, 150),
		Vector3(65, 10, 150),
		Vector3(80, 10, 150),
		Vector3(95, 10, 150),
		Vector3(110, 10, 150),
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
	coins = new BonusObject[30];

	for (int i = 0; i < 30; ++i) {
		BonusObject& coin = coins[i];

		AABBVolume* volume = new AABBVolume(Vector3(0.25f, 2, 0.25f));
		coin.SetBoundingVolume((CollisionVolume*)volume);
		coin.GetTransform()
			.SetScale(Vector3(0.5, 0.5, 0.5))
			.SetPosition(positions[i]);

		coin.SetRenderObject(new RenderObject(&coin.GetTransform(), bonusMesh, nullptr, basicShader, Vector4(1, 1, 0, 1)));
		coin.SetPhysicsObject(new PhysicsObject(&coin.GetTransform(), coin.GetBoundingVolume()));

		coin.GetPhysicsObject()->SetInverseMass(1.0f);
		coin.GetPhysicsObject()->InitSphereInertia();
		coin.GetPhysicsObject()->SetGravity(false);
		coin.GetPhysicsObject()->SetResolve(false);

		coin.SetType(20);
		coin.SetHooked(false);

		world->AddGameObject(&coins[i]);
	}
}

void TutorialGame::AddWanderNPCToWorld() {
	Vector3 positions[5] = {
		Vector3(50, 10, 0),
		Vector3(70, 10, 0),
		Vector3(90, 10, 0),
		Vector3(110, 10, 0),
		Vector3(130, 10, 0),
	};

	float velocity[5] = { 10,20,40,20,30 };

	wanderNPCs = new WanderObject[5];

	for (int i = 0; i < 5; ++i) {
		WanderObject& npc = wanderNPCs[i];

		float meshSize = 10.0f;
		float inverseMass = 1;

		OBBVolume* volume = new OBBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
		npc.SetBoundingVolume((CollisionVolume*)volume);

		npc.GetTransform()
			.SetScale(Vector3(meshSize, meshSize, meshSize))
			.SetPosition(positions[i])
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));

		npc.SetRenderObject(new RenderObject(&npc.GetTransform(), enemyMesh, nullptr, basicShader, Vector4(0, 0, 1, 1)));
		npc.SetPhysicsObject(new PhysicsObject(&npc.GetTransform(), npc.GetBoundingVolume()));

		npc.GetPhysicsObject()->SetInverseMass(inverseMass);
		npc.GetPhysicsObject()->InitSphereInertia();

		npc.SetVelocity(velocity[i]);
		npc.SetShot(true);
		npc.SetHooked(false);
		npc.SetType(36);

		world->AddGameObject(&wanderNPCs[i]);
	}
}

void TutorialGame::AddShootNPCToWorld() {
	Vector3 positions[3] = {
		Vector3(-60, 45, 540),
		Vector3(0, 45, 540),
		Vector3(60, 45, 540),
	};

	shootNPCs = new ShootObject[3];

	for (int i = 0; i < 3; ++i) {
		ShootObject& npc = shootNPCs[i];

		float meshSize = 10.0f;
		float inverseMass = 1;

		OBBVolume* volume = new OBBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
		npc.SetBoundingVolume((CollisionVolume*)volume);

		npc.GetTransform()
			.SetScale(Vector3(meshSize, meshSize, meshSize))
			.SetPosition(positions[i])
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));

		npc.SetRenderObject(new RenderObject(&npc.GetTransform(), securityMesh, nullptr, basicShader, Vector4(0, 1, 1, 1)));
		npc.SetPhysicsObject(new PhysicsObject(&npc.GetTransform(), npc.GetBoundingVolume()));

		npc.GetPhysicsObject()->SetInverseMass(inverseMass);
		npc.GetPhysicsObject()->InitSphereInertia();

		npc.SetType(34);
		npc.SetShot(true);
		npc.SetHooked(false);
		npc.SetPlayerObject(playerGoat);

		world->AddGameObject(&shootNPCs[i]);
	}
}

void TutorialGame::AddCannonNPCToWorld() {
	Vector3 positions[3] = {
		Vector3(-60, 45, -540),
		Vector3(0, 45, -540),
		Vector3(60, 45, -540),
	};

	cannonNPCs = new CannonObject[3];

	for (int i = 0; i < 3; ++i) {
		CannonObject& npc = cannonNPCs[i];

		float meshSize = 10.0f;
		float inverseMass = 1;

		OBBVolume* volume = new OBBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
		npc.SetBoundingVolume((CollisionVolume*)volume);

		npc.GetTransform()
			.SetScale(Vector3(meshSize, meshSize, meshSize))
			.SetPosition(positions[i])
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));

		npc.SetRenderObject(new RenderObject(&npc.GetTransform(), securityMesh, nullptr, basicShader, Vector4(0, 0, 0, 1)));
		npc.SetPhysicsObject(new PhysicsObject(&npc.GetTransform(), npc.GetBoundingVolume()));

		npc.GetPhysicsObject()->SetInverseMass(inverseMass);
		npc.GetPhysicsObject()->InitSphereInertia();

		npc.SetType(34);
		npc.SetShot(true);
		npc.SetHooked(false);
		npc.SetPlayerObject(playerGoat);

		world->AddGameObject(&cannonNPCs[i]);
	}
}

void TutorialGame::AddSecurityNPCToWorld() {
	Vector3 positions[2] = {
		Vector3(720, 10, 170),
		Vector3(720, 10, -170),
	};

	securityNPCs = new SecurityObject[2];

	for (int i = 0; i < 2; ++i) {
		SecurityObject& npc = securityNPCs[i];

		float meshSize = 10.0f;
		float inverseMass = 1;

		OBBVolume* volume = new OBBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
		npc.SetBoundingVolume((CollisionVolume*)volume);

		npc.GetTransform()
			.SetScale(Vector3(meshSize, meshSize, meshSize))
			.SetPosition(positions[i])
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -90, 0));
		npc.SetInitPosition(positions[i]);

		npc.SetRenderObject(new RenderObject(&npc.GetTransform(), securityMesh, nullptr, basicShader, Vector4(1, 0.39f, 0.28f, 1)));
		npc.SetPhysicsObject(new PhysicsObject(&npc.GetTransform(), npc.GetBoundingVolume()));

		npc.GetPhysicsObject()->SetInverseMass(inverseMass);
		npc.GetPhysicsObject()->InitSphereInertia();

		npc.SetType(35);
		npc.SetShot(true);
		npc.SetHooked(false);
		npc.SetPlayerObject(playerGoat);

		world->AddGameObject(&securityNPCs[i]);
	}
}

void TutorialGame::InitCamera() {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	world->GetMainCamera().SetPitch(-15.0f);
	world->GetMainCamera().SetYaw(-90.0f);
	world->GetMainCamera().SetPosition(Vector3(-60, 40, 60));
	lockedObject = playerGoat;
}

void TutorialGame::InitWorld() {
	world->ClearAndErase();
	physics->Clear();

	InitGround();
	InitWall();
	InitTree();
	InitConstraint();
	InitPlayer();
	InitNPC();
	InitPowerup();
	InitCollision();
	InitKey();
	InitTower();
	InitTarget();
	InitBonus();
	InitTrigger();
	InitAirWall();
	InitItem();

	//InitMixedGridWorld(15, 15, 3.5f, 3.5f);
    //InitGameExamples();
	//BridgeConstraintTest();
	//testStateObject = AddStateObjectToWorld(Vector3(0, 30, 0), Vector3(1, 1, 1), 10.0f);
}

void TutorialGame::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -20, 0));
}

void TutorialGame::InitGround() {
	AddFloorToWorld(Vector3(0, 0, 0), Vector3(200, 2, 200), Vector4(1, 1, 1, 1));
	AddFloorToWorld(Vector3(0, 40, 600), Vector3(75, 2, 75), Vector4(1, 0.84f, 0, 1));
	AddFloorToWorld(Vector3(0, 40, -600), Vector3(75, 2, 75), Vector4(1, 0.84f, 0, 1));
	AddFloorToWorld(Vector3(300, 0, 0), Vector3(100, 2, 125), Vector4(1, 0.5f, 0, 1));
	AddFloorToWorld(Vector3(610, 0, 0), Vector3(210, 2, 180), Vector4(1, 0.6f, 0.8f, 1));
	AddRespawnPointToWorld();
	AddPoolToWorld(Vector3(540, 2, -70), Vector3(60, 0.2f, 30));
	AddPlankToWorld(Vector3(700, 2, -120), Vector3(40, 0.2f, 20));
}

void TutorialGame::InitWall() {
	AddWallToWorld(Vector3(125, 30, 200), Vector3(75, 30, 2), Vector4(0.2f, 0.8f, 0.8f, 1));
	AddWallToWorld(Vector3(-125, 30, 200), Vector3(75, 30, 2), Vector4(0.2f, 0.8f, 0.8f, 1));
	AddWallToWorld(Vector3(125, 30, -200), Vector3(75, 30, 2), Vector4(0.2f, 0.8f, 0.8f, 1));
	AddWallToWorld(Vector3(-125, 30, -200), Vector3(75, 30, 2), Vector4(0.2f, 0.8f, 0.8f, 1));
	AddWallToWorld(Vector3(-200, 30, 0), Vector3(2, 30, 200), Vector4(0.2f, 0.8f, 0.8f, 1));
	AddWallToWorld(Vector3(300, 30, 125), Vector3(100, 30, 2), Vector4(1, 0.39f, 0.28f, 1));
	AddWallToWorld(Vector3(300, 30, -125), Vector3(100, 30, 2), Vector4(1, 0.39f, 0.28f, 1));
	AddWallToWorld(Vector3(300, 30, 0), Vector3(100, 30, 2), Vector4(1, 0.39f, 0.28f, 1));
	AddWallToWorld(Vector3(610, 30, 180), Vector3(210, 30, 2), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddWallToWorld(Vector3(610, 30, -180), Vector3(210, 30, 2), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddWallToWorld(Vector3(820, 30, 0), Vector3(2, 30, 180), Vector4(0.93f, 0.51f, 0.93f, 1));
	AddMazeToWorld(Vector3(430, -2.5f, 0), Vector4(0, 0.98f, 0.60f, 1));
}

void TutorialGame::InitTree() {
	AddTreeToWorld(Vector3(0, 2, 0), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(20, 2, 0), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(0, 2, 100), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(-80, 2, 0), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(0, 2, -65), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(38, 2, 92), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(-97, 2, 23), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(26, 2, 28), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(-50, 2, 78), Vector3(10, 10, 10));
	AddTreeToWorld(Vector3(-90, 2, 28), Vector3(10, 10, 10));
}

void TutorialGame::InitConstraint() {
	AddRopeBridgeToWorld(Vector3(0, -10, 208), Vector3(0, 40, 517));
	AddRopeBridgeToWorld(Vector3(0, 40, -517), Vector3(0, -10, -208));
	leftGate = AddGateCubeToWorld(Vector3(300, 30, -60), Vector3(2, 30, 60), Vector4(1, 0, 0, 1));
	rightGate = AddGateCubeToWorld(Vector3(300, 30, 60), Vector3(2, 30, 60), Vector4(0, 0, 1, 1));
	AddGateToWorld(Vector3(300, 30, -60), leftGate);
	AddGateToWorld(Vector3(300, 30, 60), rightGate);
}

void TutorialGame::InitPlayer() {
	playerGoat = AddPlayerToWorld(Vector3(-130, 10, 0));
	lockedObject = playerGoat;
}

void TutorialGame::InitPowerup() {
	velocityPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, -100), 2, "Velocity", Vector4(0, 0, 1, 1));
	jumpPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, 0), 3, "Jump", Vector4(1, 1, 0, 1));
	shootPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, -50), 4, "Shoot", Vector4(1, 0, 0, 1));
	hookPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, 50), 5, "Hook", Vector4(0, 0, 0, 1));
	if (playerGoat->GetHide1()) { flyPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, 100), 14, "Fly", Vector4(0, 1, 1, 1)); }
	if (playerGoat->GetHide2()) { invisibilityPowerup = AddPlayerPowerupToWorld(Vector3(-50, 10, 50), 19, "Invisibility", Vector4(1, 0, 1, 0.5f));	}
	if (playerGoat->GetHide3()) { gravityPowerup = AddPlayerPowerupToWorld(Vector3(-50, 10, 100), 18, "Vision", Vector4(0.24f, 0.70f, 0.55f, 1)); }
	goosePowerup = AddGoosePowerupToWorld();
}

void TutorialGame::InitCollision() {
	push1 = AddPushObjectToWorld(Vector3(180, 4, 165));
	push2 = AddPushObjectToWorld(Vector3(180, 4, -165));
	jump1 = AddJumpObjectToWorld(Vector3(420, 2, 150));
	jump2 = AddJumpObjectToWorld(Vector3(420, 2, -150));
	penalty1 = AddPenaltyObjectToWorld(Vector3(-110, 12, 190), 21);
	penalty2 = AddPenaltyObjectToWorld(Vector3(-75, 12, 190), 22);
}

void TutorialGame::InitKey() {
	leftKey = AddKeyObjectToWorld(Vector3(0, 50, -650), 6, "LeftKey", Vector4(1, 0, 0, 1));
	rightKey = AddKeyObjectToWorld(Vector3(0, 50, 650), 7, "RightKey", Vector4(0, 0, 1, 1));
}

void TutorialGame::InitItem() {
	item = AddItemObjectToWorld();
}

void TutorialGame::InitTower() {
	AddTowerToWorld(Vector3(-150, 0, -100));
}

void TutorialGame::InitTarget() {
	AddTargetToWorld(Vector3(-300, 40, 350), true);
	AddTargetToWorld(Vector3(-300, 40, -350), false);
}

void TutorialGame::InitBonus() {
	AddBonusObjectToWorld();
}

void TutorialGame::InitTrigger() {
	AddTriggerToWorld(Vector3(300, 27, -60), Vector3(0.5f, 27, 60), 23);
	AddTriggerToWorld(Vector3(300, 27, 60), Vector3(0.5f, 27, 60), 23);
	AddTriggerToWorld(Vector3(300, 200, -155), Vector3(2, 200, 30), 24);
	AddTriggerToWorld(Vector3(300, 200, 155), Vector3(2, 200, 30), 24);
	AddTriggerToWorld(Vector3(0, 90, -525), Vector3(75, 50, 2), 25);
	AddTriggerToWorld(Vector3(0, 90, 525), Vector3(75, 50, 2), 25);
}

void TutorialGame::InitAirWall() {
	AddAirWallToWorld(Vector3(-500, 125, 0), Vector3(1, 275, 800));
	AddAirWallToWorld(Vector3(1000, 125, 0), Vector3(1, 275, 800));
	AddAirWallToWorld(Vector3(250, 125, -800), Vector3(750, 275, 1));
	AddAirWallToWorld(Vector3(250, 125, 800), Vector3(750, 275, 1));
	AddAirWallToWorld(Vector3(250, -150, 0), Vector3(750, 1, 800));
	AddAirWallToWorld(Vector3(250, 400, 0), Vector3(750, 1, 800));
}

void TutorialGame::InitNPC() {
	AddGuideToWorld(Vector3(-130, 10, -180), 31);
	AddGuideToWorld(Vector3(-110, 10, -180), 32);
	AddGuideToWorld(Vector3(-90, 10, -180), 33);
	AddWanderNPCToWorld();
	AddShootNPCToWorld();
	AddCannonNPCToWorld();
	AddSecurityNPCToWorld();
	goose = AddGooseToWorld();
}

void TutorialGame::InitGameExamples() {
	AddPlayerToWorld0(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	AddBonusToWorld(Vector3(10, 5, 0));
}

void TutorialGame::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void TutorialGame::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);

			if (rand() % 2) {
				AddCubeToWorld(position, cubeDims);
			}
			else {
				AddSphereToWorld(position, sphereRadius);
			}
		}
	}
}

void TutorialGame::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols + 1; ++x) {
		for (int z = 1; z < numRows + 1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}