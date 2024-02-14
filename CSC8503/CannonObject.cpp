#include "CannonObject.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

CannonObject::CannonObject() {
	counter = 0.0f;
	stateMachine = new StateMachine();
	canShoot = false;
	player = nullptr;

	State* StateA = new State([&](float dt)->void
		{
			MoveLeft(dt);
		}
	);
	State* StateB = new State([&](float dt)->void
		{
			MoveRight(dt);
		}
	);
	State* StateC = new State([&](float dt)->void
		{
			Attack(dt);
		}
	);

	stateMachine->AddState(StateA);
	stateMachine->AddState(StateB);

	stateMachine->AddTransition(new StateTransition(StateA, StateB,
		[&]()->bool
		{
			return counter >= 1;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{
			return counter <= -1;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateA, StateC,
		[&]()->bool
		{   bool flag = false;
	if (player->GetTransform().GetPosition().z < -200 &&
		player->GetTransform().GetPosition().y > -20 &&
		abs(player->GetTransform().GetPosition().x) < 35 &&
		!player->GetInvisibility()) {
		canShoot = true;
		counter = RandomValue(5, 10);
		flag = true;
	}
	return flag;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateB, StateC,
		[&]()->bool
		{   bool flag = false;
	if (player->GetTransform().GetPosition().z < -200 &&
		player->GetTransform().GetPosition().y > -20 &&
		abs(player->GetTransform().GetPosition().x) < 35 &&
		!player->GetInvisibility()) {
		canShoot = true;
		counter = RandomValue(5, 10);
		flag = true;
	}
	return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateC, StateA,
		[&]()->bool
		{   bool flag = false;
	if (player->GetTransform().GetPosition().z > -170 ||
		player->GetTransform().GetPosition().y < -30 ||
		abs(player->GetTransform().GetPosition().x) > 80 ||
		player->GetInvisibility()) {
		transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));
		counter = 0.0f;
		canShoot = false;
		flag = true;
	}
	return flag;
		}
	));
}

CannonObject::~CannonObject() {
	delete stateMachine;
}

void CannonObject::Update(float dt) {
	if (!hasCollided)stateMachine->Update(dt);
}

void CannonObject::MoveLeft(float dt) {
	physicsObject->SetAngularVelocity(Vector3(0, -1, 0));
	counter += dt;
}

void CannonObject::MoveRight(float dt) {
	physicsObject->SetAngularVelocity(Vector3(0, 1, 0));
	counter -= dt;
}

void CannonObject::Attack(float dt) {
	transform.SetOrientation
	(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), GetAngle()));

	if (canShoot) {
		counter -= dt;
		if (counter <= 0) {
			counter = RandomValue(2, 5);

			Vector3 random = Vector3(RandomValue(-3, 3), RandomValue(-10, -5), RandomValue(-3, 3));
			Vector3 direction = player->GetTransform().GetPosition() + random - transform.GetPosition();
			Vector3 torque = Vector3(RandomValue(5000, 10000), RandomValue(5000, 10000), RandomValue(5000, 10000));
			float force = RandomValue(45, 60);

			BombObject* bomb = new BombObject();
			if (player->GetTransform().GetPosition().z > transform.GetPosition().z)
				bomb = TutorialGame::GetGame()->AddBomb(Vector3(transform.GetPosition().x,
					transform.GetPosition().y + 10,
					transform.GetPosition().z + 30),
					rand() % 3);
			else
				bomb = TutorialGame::GetGame()->AddBomb(Vector3(transform.GetPosition().x,
					transform.GetPosition().y + 10,
					transform.GetPosition().z - 30),
					rand() % 3);

			bomb->GetPhysicsObject()->AddForce(direction * force);
			bomb->GetPhysicsObject()->AddTorque(torque);
		}
	}
}

float CannonObject::GetAngle() {
	Vector3 npcPosition = transform.GetPosition();
	Vector3 playerPosition = player->GetTransform().GetPosition();

	float z = npcPosition.z - playerPosition.z;
	float x = npcPosition.x - playerPosition.x;

	float tanValue = x / z;

	float angleValue = atan(tanValue);

	float floatValue = RadiansToDegrees(angleValue);
	if (z < 0)floatValue = 180 + floatValue;

	return  floatValue;
}

void CannonObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		canShoot = false;
		renderObject->SetDefaultTexture(nullptr);
		renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
		physicsObject->SetResolve(false);
	}

	if (otherObject->GetType() == 50) {
		SetActive(false);
	}
}