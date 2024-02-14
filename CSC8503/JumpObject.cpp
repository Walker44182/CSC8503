#include "JumpObject.h"

using namespace NCL;
using namespace CSC8503;

void JumpObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetColour(Vector4(1, 1, 0, 1));
	}
}

void JumpObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetColour(Vector4(1, 1, 1, 1));
	}
}