#include "PenaltyObject.h"

using namespace NCL;
using namespace CSC8503;

PenaltyObject::PenaltyObject() {
	basicTex = GameTechRenderer::GetRenderer()->LoadTexture("checkerboard.png");
}

void PenaltyObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetDefaultTexture(basicTex);
		renderObject->SetColour(Vector4(0, 1, 0, 1));
	}
}

void PenaltyObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetColour(Vector4(1, 1, 1, 1));
	}
}