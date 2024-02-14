#pragma once
#include "GameScreen.h"

namespace NCL {
	namespace CSC8503 {
		class ChooseServerScreen : public GameScreen {
		public:
			ChooseServerScreen();
			~ChooseServerScreen();

			virtual int UpdateScreen(float dt) override;

			static ChooseServerScreen* GetScreen() {
				return instance;
			};

		protected:
			KeyboardMouseController controller;
			static ChooseServerScreen* instance;
		};
	}
}