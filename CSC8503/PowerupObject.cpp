#include "PowerupObject.h"

using namespace NCL;
using namespace CSC8503;

PlayerPowerup::PlayerPowerup(std::string objectName) {
	name = objectName;
	playerObject = nullptr;
	putDown = true;
}

void PlayerPowerup::Update(float dt) {
	if (!putDown) {
		if (!playerObject->GetRayMode()) renderObject->SetColour(colour);
		else renderObject->SetColour(Vector4());
		Vector3 playerPosition = playerObject->GetTransform().GetPosition();
		transform.SetPosition(Vector3(playerPosition.x, playerPosition.y + 10, playerPosition.z));
	}
	if(playerObject->GetDie()){ 
		if (!putDown) {
			transform.SetPosition(initPosition);
			putDown = true;
		}
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::R)) {
		if (!putDown) {
			putDown = true;
			Vector3 position = transform.GetPosition();
			transform.SetPosition(Vector3(position.x + 10, position.y, position.z));
			physicsObject->SetResolve(true);
			physicsObject->SetGravity(true);
		}
	}
}

void PlayerPowerup::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (putDown && otherObject->GetType() == 1) {
		if(!playerObject->GetPowerup())
		putDown = false;
	}
	if (otherObject->GetType() == 50) {
		if(putDown)
		transform.SetPosition(initPosition);
	}
	physicsObject->SetResolve(false);
	physicsObject->SetGravity(false);
	physicsObject->SetLinearVelocity(Vector3());
}

GoosePowerup::GoosePowerup() {
	gooseObject = nullptr;
	putDown = true;
}

void GoosePowerup::Update(float dt) {
	if (!gooseObject->IsActive()) {
		isActive = false;
	}
	if (!putDown) {
		Vector3 goosePosition = gooseObject->GetTransform().GetPosition();
		transform.SetPosition(Vector3(goosePosition.x, goosePosition.y + 10, goosePosition.z));
		if (gooseObject->GetPowerup() == 1) {
			renderObject->SetColour(Vector4(0, 0, 1, 1));
		}
		else if (gooseObject->GetPowerup() == 2) {
			renderObject->SetColour(Vector4(0, 1, 0, 1));
		}
		else if (gooseObject->GetPowerup() == 3) {
			renderObject->SetColour(Vector4(1, 1, 0, 1));
		}
	}
}

void GoosePowerup::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (putDown && otherObject->GetType() == 39) {
		if (gooseObject->GetAttack()) {
			putDown = false;
		}
	}
}