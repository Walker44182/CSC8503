#pragma once
#include "PauseScreen.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

PauseScreen* PauseScreen::instance = nullptr;

PauseScreen::PauseScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {	
	AddGoatButton(Vector3(0, 40, 0), 1);
	AddCubeButton(Vector3(0, 20, 0), 2);

	instance = this;
}

PauseScreen::~PauseScreen() {
	if (instance = this) instance = nullptr;
}

int PauseScreen::UpdateScreen(float dt) {
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
	Debug::Print("Resume Game", Vector2(50, 30), Debug::WHITE);
	Debug::Print("Quit Game", Vector2(50, 70), Debug::WHITE);

	return choice;
}
