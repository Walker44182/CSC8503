#pragma once
#include "GameScreen.h"

namespace NCL {
	namespace CSC8503 {
		class PauseScreen: public GameScreen {
		public:
			PauseScreen();
			~PauseScreen();

			virtual int UpdateScreen(float dt) override;

			static PauseScreen* GetScreen() {
				return instance;
			};

			protected:
			KeyboardMouseController controller;
			static PauseScreen* instance;
		};
	}
}