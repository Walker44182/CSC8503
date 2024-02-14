#include "WanderObject.h"

using namespace NCL;
using namespace CSC8503;

WanderObject::WanderObject() {
	counter = 0.0f;
	stateMachine = new StateMachine();
	velocity = 0.0f;

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
		{   if (transform.GetPosition().z > 100) {
		transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
	}
			return transform.GetPosition().z > 100;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{   if (transform.GetPosition().z < -100) {
		transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));
	}
			return transform.GetPosition().z < -100;
		}
	));
}

WanderObject::~WanderObject() {
	delete stateMachine;
}

void WanderObject::Update(float dt) {
	if(!hasCollided)stateMachine->Update(dt);
}

void WanderObject::MoveLeft(float dt) {
	physicsObject->AddForce(Vector3(0,0,velocity));
}

void WanderObject::MoveRight(float dt) {
	physicsObject->AddForce(Vector3(0, 0, -velocity));
}

void WanderObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		renderObject->SetDefaultTexture(nullptr);
		renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
		physicsObject->SetResolve(false);
	}

	if (otherObject->GetType() == 50) {
		SetActive(false);
	}
}