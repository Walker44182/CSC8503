#include "TreeObject.h"

using namespace NCL;
using namespace CSC8503;

void TreeObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetDefaultTexture(nullptr);
		renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
		physicsObject->SetResolve(false);
	}

	if (otherObject->GetType() == 50) {
		SetActive(false);
	}
}