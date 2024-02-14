#pragma once
#include "GameScreen.h"

namespace NCL {
	namespace CSC8503 {
		class QuitScreen : public GameScreen {
		public:
			QuitScreen();
			~QuitScreen();

			virtual int UpdateScreen(float dt) override;

			static QuitScreen* GetScreen() {
				return instance;
			};

		protected:
			KeyboardMouseController controller;
			static QuitScreen* instance;
		};
	}
}