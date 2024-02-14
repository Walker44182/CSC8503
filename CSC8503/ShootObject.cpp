#include "ShootObject.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

ShootObject::ShootObject() {
	counter = 0.0f;
	stateMachine = new StateMachine();
	canShoot = false;
	player = nullptr;
	objClosest = nullptr;

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
	stateMachine->AddState(StateC);

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
	        if (player->GetTransform().GetPosition().z > 200 &&
				player->GetTransform().GetPosition().y > -20 &&
		        abs(player->GetTransform().GetPosition().x) < 35 &&
				!player->GetInvisibility()) {
				canShoot = true;
				counter = RandomValue(1.5f, 2.5f);
		        flag = true;
	        }
	       return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateC,
		[&]()->bool
		{   bool flag = false;
	if (player->GetTransform().GetPosition().z > 200 &&
		player->GetTransform().GetPosition().y > -20 &&
		abs(player->GetTransform().GetPosition().x) < 35 &&
		!player->GetInvisibility()) {
		canShoot = true;
		counter = RandomValue(1.5f, 2.5f);
		flag = true;
	}
	return flag;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateC, StateA,
		[&]()->bool
		{   bool flag = false;
			if (player->GetTransform().GetPosition().z < 170 ||
				player->GetTransform().GetPosition().y < -30 ||
				abs(player->GetTransform().GetPosition().x) > 80 ||
				player->GetInvisibility()) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
				counter = 0.0f;
				canShoot = false;
				flag = true;
			}
			return flag;
		}
	));
}

ShootObject::~ShootObject() {
	delete stateMachine;
}

void ShootObject::Update(float dt) {
	if (!hasCollided)stateMachine->Update(dt);
}

void ShootObject::MoveLeft(float dt) {
	physicsObject->SetAngularVelocity(Vector3(0, -1, 0));
	counter += dt;
}

void ShootObject::MoveRight(float dt) {
	physicsObject->SetAngularVelocity(Vector3(0, 1, 0));
	counter -= dt;
}

void ShootObject::Attack(float dt) {
	transform.SetOrientation
	  (Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), GetAngle()));

	if (canShoot) {
		counter -= dt;
		if (counter <= 0) {
			counter = RandomValue(1.5f, 2.5f);

			RayCollision closestCollision;
			Vector3 rayPos;
			Vector3 rayDir;
			Vector3 random = Vector3(RandomValue(-10, 10), RandomValue(-10, -5), RandomValue(-10, 10));

			rayDir = player->GetTransform().GetPosition() + random - transform.GetPosition();

			rayPos = Vector3(transform.GetPosition().x,
				transform.GetPosition().y + 10,
				transform.GetPosition().z);

			Ray r = Ray(rayPos, rayDir);

			if (TutorialGame::GetGame()->GetGameWorld()->Raycast(r, closestCollision, true, this)) {
				objClosest = (GameObject*)closestCollision.node;
				if (objClosest) {
					if (objClosest->GetType() == 1) {
						player->SetHurt(1);
						player->SetHit(true);
					}
					Debug::DrawLine(rayPos, closestCollision.collidedAt, Vector4(1, 1, 0, 1), 2.0f);
				}
			}
		}
	}
}

float ShootObject::GetAngle() {
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

void ShootObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
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