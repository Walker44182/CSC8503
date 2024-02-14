#include "ItemObject.h"

using namespace NCL;
using namespace CSC8503;

ItemObject::ItemObject() {
	playerObject = nullptr;
	putDown = true;
	finish = false;
}

void ItemObject::Update(float dt) {
	if (putDown) {
		physicsObject->AddTorque(Vector3(10, 10, 0));
	}
	else {
		Vector3 playerPosition = playerObject->GetTransform().GetPosition();
		transform.SetPosition(Vector3(playerPosition.x, playerPosition.y, playerPosition.z + 10));
	}
	if (playerObject->GetDie()) {
		if (!putDown) {
			transform.SetPosition(initPosition);
			putDown = true;
		}
	}
	if (playerObject->GetWin() && !finish) {
		putDown = true;
		finish = true;
		transform.SetPosition(Vector3(-130, 10, 0));
	}
}

void ItemObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (putDown && otherObject->GetType() == 1) {
		if (!playerObject->GetItem() && !finish) {
			physicsObject->SetAngularVelocity(Vector3());
			putDown = false;
		}
	}
}

MazeItem::MazeItem() {
	mazePlayer = nullptr;
}

void MazeItem::Update(float dt) {
	if (putDown) {
		physicsObject->AddTorque(Vector3(10, 10, 0));
	}
	else {
		Vector3 playerPosition = mazePlayer->GetTransform().GetPosition();
		transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 10, playerPosition.z));
	}
}

void MazeItem::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (putDown && otherObject->GetType() == 1) {
			physicsObject->SetAngularVelocity(Vector3());
			putDown = false;
			mazePlayer = otherObject;
	}
}
