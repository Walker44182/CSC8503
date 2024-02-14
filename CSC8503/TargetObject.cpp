#include "TargetObject.h"

using namespace NCL;
using namespace CSC8503;

void TargetObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 50) {
		this->SetActive(false);
	}
}