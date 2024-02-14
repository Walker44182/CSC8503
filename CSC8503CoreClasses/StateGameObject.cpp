#include "StateGameObject.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject() {}

StateGameObject::~StateGameObject() {}

void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}