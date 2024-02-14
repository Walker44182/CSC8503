#pragma once
#include "GameScreen.h"

namespace NCL {
	namespace CSC8503 {
		class FinishedScreen : public GameScreen {
		public:
			FinishedScreen();
			~FinishedScreen();

			virtual int UpdateScreen(float dt) override;

			static FinishedScreen* GetScreen() {
				return instance;
			};

		protected:
			KeyboardMouseController controller;
			static FinishedScreen* instance;
		};
	}
}