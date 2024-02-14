#pragma once
#include "FinishedScreen.h"
#include "NetworkedGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"

using namespace NCL;
using namespace CSC8503;

FinishedScreen* FinishedScreen::instance = nullptr;

FinishedScreen::FinishedScreen() :controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	AddCubeButton(Vector3(0, 20, 0), 1);

	instance = this;
}

FinishedScreen::~FinishedScreen() {
	if (instance = this) instance = nullptr;
}

int FinishedScreen::UpdateScreen(float dt) {
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

	int score1 = NetworkedGame::GetNetworkedGame()->GetPlayer1()->GetPlayerScore();
	int score2 = NetworkedGame::GetNetworkedGame()->GetPlayer2()->GetPlayerScore();

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
	if (score1 > score2) Debug::Print("Winner: Player1!", Vector2(40, 20), Debug::CYAN);
	else if (score1 < score2) Debug::Print("Winner: Player2!", Vector2(40, 20), Debug::CYAN);
	else if (score1 == score2) Debug::Print("Draw!" + std::to_string(score1), Vector2(40, 20), Debug::CYAN);
	Debug::Print("Player1 Score: " + std::to_string(score1), Vector2(40, 30), Debug::BLUE);
	Debug::Print("Player2 Score: " + std::to_string(score2), Vector2(40, 40), Debug::YELLOW);
	Debug::Print("Back To Menu", Vector2(50, 70), Debug::WHITE);

	return choice;
}