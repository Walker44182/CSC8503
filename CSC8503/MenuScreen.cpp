#pragma once
#include "MenuScreen.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

MenuScreen* MenuScreen::instance = nullptr;

MenuScreen::MenuScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world->GetMainCamera().SetNearPlane(0.1f);
	world->GetMainCamera().SetFarPlane(500.0f);
	world->GetMainCamera().SetPitch(0.0f);
	world->GetMainCamera().SetYaw(90.0f);
	world->GetMainCamera().SetPosition(Vector3(90, 40, -10));
	
	AddGoatButton(Vector3(0, 60, 0), 1);
	AddCubeButton(Vector3(0, 20, 0), 2);
	AddGooseButton(Vector3(0, 40, 0), 3);

	instance = this;
}

MenuScreen::~MenuScreen() {
	if (instance = this) instance = nullptr;
}

int MenuScreen::UpdateScreen(float dt) {
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
	Debug::Print("Goat Game", Vector2(50, 20), Debug::WHITE);
	Debug::Print("Maze Game", Vector2(50, 50), Debug::WHITE);
	Debug::Print("Quit Game", Vector2(50, 80), Debug::WHITE);

	return choice;
}