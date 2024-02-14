#include "PushObject.h"

using namespace NCL;
using namespace CSC8503;

PushObject::PushObject() {
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* StateA = new State([&](float dt)->void
		{
			MoveLeft(dt);
		}
	);
	State* StateB = new State([&](float dt)->void
		{
			MoveRight(dt);
		}
	);

	stateMachine->AddState(StateA);
	stateMachine->AddState(StateB);

	stateMachine->AddTransition(new StateTransition(StateA, StateB,
		[&]()->bool
		{
			return transform.GetPosition().x - 180 > 30;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{
			return transform.GetPosition().x - 180 < -30;
		}
	));
}

PushObject::~PushObject() {
	delete stateMachine;
}

void PushObject::Update(float dt) {
	stateMachine->Update(dt);
}

void PushObject::MoveLeft(float dt) {
	physicsObject->SetLinearVelocity(Vector3(160, 0, 0));
	counter += dt;
}

void PushObject::MoveRight(float dt) {
	physicsObject->SetLinearVelocity(Vector3(-30, 0, 0));
	counter -= dt;
}

void PushObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetColour(Vector4(1, 0, 0, 1));
	}
}

void PushObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetColour(Vector4(1, 1, 1, 1));
	}
}