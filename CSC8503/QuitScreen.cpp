#pragma once
#include "QuitScreen.h"
#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

QuitScreen* QuitScreen::instance = nullptr;

QuitScreen::QuitScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	AddCubeButton(Vector3(0, 20, 0), 1);

	instance = this;
}

QuitScreen::~QuitScreen() {
	if (instance = this) instance = nullptr;
}

int QuitScreen::UpdateScreen(float dt) {
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
	Debug::Print("Your Score: " + std::to_string(TutorialGame::GetGame()->GetPlayer()->GetScore()), Vector2(40, 30), Debug::WHITE);
	Debug::Print("Back To Menu", Vector2(50, 70), Debug::WHITE);

	return choice;
}