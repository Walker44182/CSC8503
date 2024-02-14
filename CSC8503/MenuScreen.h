#pragma once
#include "GameScreen.h"

namespace NCL {
	namespace CSC8503 {
		class MenuScreen : public GameScreen {
		public:
			MenuScreen();
			~MenuScreen();

			virtual int UpdateScreen(float dt) override;

			static MenuScreen* GetScreen() {
				return instance;
			};

		protected:
			KeyboardMouseController controller;
			static MenuScreen* instance;
		};
	}
}