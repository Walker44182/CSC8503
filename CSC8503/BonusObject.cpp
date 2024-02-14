#include "BonusObject.h"

using namespace NCL;
using namespace CSC8503;

void BonusObject::Update(float dt) {
	physicsObject->AddTorque(Vector3(0, 0.5f, 0));

	if (useCounter) counter -= dt;
	if (counter < 0) {
		hasCollided = true;
		useCounter = false;
	}
	if (hasCollided && isActive) {
		isActive = false;
	}
}

void BonusObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		physicsObject->AddForce(Vector3(0, 1000, 0));
		useCounter = true;
		counter = 0.1f;
	}
}

void MazeBonus::Update(float dt) {
	physicsObject->AddTorque(Vector3(0, 0.5f, 0));
}
