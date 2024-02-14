#include "BridgeObject.h"

using namespace NCL;
using namespace CSC8503;

void BridgeObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1 && !hasCollided) {
		renderObject->SetColour(Vector4(RandomValue(0, 1), RandomValue(0, 1), RandomValue(0, 1), 1));
		hasCollided = true;
	}
}