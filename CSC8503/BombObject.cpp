#include "BombObject.h"

using namespace NCL;
using namespace CSC8503;

void BombObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	isActive = false;
}