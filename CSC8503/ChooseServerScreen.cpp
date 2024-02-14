#pragma once
#include "ChooseServerScreen.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

ChooseServerScreen* ChooseServerScreen::instance = nullptr;

ChooseServerScreen::ChooseServerScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	AddCubeButton(Vector3(0, 40, 0), 1, Vector4(0, 0, 1, 1));
	AddCubeButton(Vector3(0, 20, 0), 2, Vector4(1, 1, 0, 1));

	instance = this;
}

ChooseServerScreen::~ChooseServerScreen() {
	if (instance = this) instance = nullptr;
}

int ChooseServerScreen::UpdateScreen(float dt) {
	int choice = 0;
	float counter = 0.0f;
	Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());
	RayCollision closestCollision;
	if (world->Raycast(ray, closestCollision, true)) {
		selectionObject = (GameObject*)closestCollision.node;
		selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 1, 1));
		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Left)) {
			choice = selectionObject->GetType();
		}
	}
	else {
		if (selectionObject) {
			selectionObject->GetRenderObject()->SetColour(selectionObject->GetInitColour());
		}
	}

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
	Debug::Print("Start As Server", Vector2(50, 30), Debug::WHITE);
	Debug::Print("Start As Client", Vector2(50, 70), Debug::WHITE);

	return choice;
}
