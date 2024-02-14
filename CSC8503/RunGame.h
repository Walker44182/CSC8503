#pragma once
#include "Window.h"
#include "Debug.h"
#include "PushdownMachine.h"
#include "PushdownState.h"
#include "TutorialGame.h"
#include "MazeGame.h"
#include "NetworkedGame.h"
#include "MenuScreen.h"
#include "PauseScreen.h"
#include "QuitScreen.h"
#include "ChooseServerScreen.h"
#include "FinishedScreen.h"

namespace NCL {
	namespace CSC8503 {
		class QuitUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				int choice = QuitScreen::GetScreen()->UpdateScreen(dt);
				if (choice == 1) {
					return PushdownResult::Pop3;
				}
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
			void OnAwake() override {
				QuitScreen* s = new QuitScreen();
			}
		};

		class FinishedUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				int choice = FinishedScreen::GetScreen()->UpdateScreen(dt);
				if (choice == 1) {
					return PushdownResult::Pop3;
				}
				return PushdownResult::NoChange;
			}
			void OnAwake() override {
				FinishedScreen* s = new FinishedScreen();
			}
		};

		class PauseUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				int choice = PauseScreen::GetScreen()->UpdateScreen(dt);
				if (choice==1 || Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					return PushdownResult::Pop;
				}
				if (choice == 2) {
					*newState = new QuitUI();
					return PushdownResult::Push;
				}
				return PushdownResult::NoChange;
			}
			void OnAwake() override {
				PauseScreen* s = new PauseScreen();
			}
		};

		class GoatGame :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				TutorialGame::GetGame()->UpdateGame(dt);
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					*newState = new PauseUI();
					return PushdownResult::Push;
				}
				return PushdownResult::NoChange;
			};
			void OnAwake() override {
			}
		};

		class MazeGameUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				NetworkedGame::GetNetworkedGame()->UpdateGame(dt);
				bool isServer = NetworkedGame::GetNetworkedGame()->IsServer();
				if (isServer) {
					Debug::Print("Player1, Server", Vector2(5, 20), Debug::WHITE);
					Debug::Print("Steal the ITEM and take it to the BLUE floor", Vector2(5, 5), Debug::CYAN);
				}
				else {
					Debug::Print("Player2, Client", Vector2(5, 20), Debug::WHITE);
					Debug::Print("Steal the ITEM and take it to the YELLOW floor", Vector2(5, 5), Debug::CYAN);
				}
				Debug::Print("Player1 Score: " + std::to_string(NetworkedGame::GetNetworkedGame()->GetPlayer1()->GetPlayerScore()), Vector2(5, 10), Debug::BLUE);
				Debug::Print("Player2 Score: " + std::to_string(NetworkedGame::GetNetworkedGame()->GetPlayer2()->GetPlayerScore()), Vector2(5, 15), Debug::YELLOW);
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					NetworkedGame::GetNetworkedGame()->SetPauseMode(!NetworkedGame::GetNetworkedGame()->GetPauseMode());
				}
				if (NetworkedGame::GetNetworkedGame()->GetPauseMode()) {
					Debug::Print("Press Esc to resume", Vector2(40, 35), Debug::WHITE);
					Debug::Print("Press P to quit", Vector2(40, 45), Debug::WHITE);
					if (Window::GetKeyboard()->KeyPressed(KeyCodes::P)) {
						return Pop2;
					}
				}
				else {
					Debug::Print("Press Esc to pause", Vector2(5, 25), Debug::WHITE);
				}
				if (NetworkedGame::GetNetworkedGame()->HasFinished()) {
					*newState = new FinishedUI();
					return PushdownResult::Push;
				}
				return PushdownResult::NoChange;
			};
			void OnAwake() override {
			}
		};

		class ChooseServerUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				int choice = ChooseServerScreen::GetScreen()->UpdateScreen(dt);
				if (choice == 1) {
					NetworkedGame* g = new NetworkedGame();
					g->StartAsServer();
					*newState = new MazeGameUI();
					return PushdownResult::Push;
				}
				if (choice == 2) {
					NetworkedGame* g = new NetworkedGame();
					g->StartAsClient(127, 0, 0, 1);
					*newState = new MazeGameUI();
					return PushdownResult::Push;
				}
				if (Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					return PushdownResult::Pop;
				}
				return PushdownResult::NoChange;
			}
			void OnAwake() override {
				ChooseServerScreen* s = new ChooseServerScreen();
			}
		};

		class MenuUI :public PushdownState {
			PushdownResult OnUpdate(float dt,
				PushdownState** newState) override {
				int choice = MenuScreen::GetScreen()->UpdateScreen(dt);
				if (choice == 1) {
					TutorialGame* g = new TutorialGame();
					*newState = new GoatGame();
					return PushdownResult::Push;
				}
				if (choice == 2 || Window::GetKeyboard()->KeyPressed(KeyCodes::ESCAPE)) {
					return PushdownResult::Pop;
				}
				if (choice == 3) {
					*newState = new ChooseServerUI();
					return PushdownResult::Push;
				}
				return PushdownResult::NoChange;
			}
			void OnAwake() override {
				MenuScreen* s = new MenuScreen();
			}
		};
	}
}

