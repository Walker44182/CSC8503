#include "KeyObject.h"

using namespace NCL;
using namespace CSC8503;

KeyObject::KeyObject(std::string objectName) {
	name = objectName;
	playerObject = nullptr;
	putDown = true;
}

void KeyObject::Update(float dt) {
	if (!putDown) {
		Vector3 playerPosition = playerObject->GetTransform().GetPosition();
		transform.SetPosition(Vector3(playerPosition.x, playerPosition.y, playerPosition.z - 10));
	}
	if (playerObject->GetDie()) {
		if (!putDown) {
			transform.SetPosition(initPosition);
			putDown = true;
		}
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
		if (!putDown) {
			putDown = true;
			Vector3 position = transform.GetPosition();
			transform.SetPosition(Vector3(position.x + 10, position.y, position.z));
			physicsObject->SetResolve(true);
			physicsObject->SetGravity(true);
		}
	}
}

void KeyObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (putDown && otherObject->GetType() == 1) {
		if (!playerObject->GetKey())
			putDown = false;
	}
	if (otherObject->GetType() == 50) {
		transform.SetPosition(initPosition)
			     .SetOrientation(Quaternion::EulerAnglesToQuaternion(-90, 0, 0));
	}
	physicsObject->SetResolve(false);
	physicsObject->SetGravity(false);
	physicsObject->SetLinearVelocity(Vector3());
}