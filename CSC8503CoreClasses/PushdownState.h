#pragma once

namespace NCL {
	namespace CSC8503 {
		class PushdownState
		{
		public:
			enum PushdownResult {
				Push, Pop, Pop2 ,Pop3, NoChange
			};
			PushdownState()  {
			}
			virtual ~PushdownState() {}

			virtual PushdownResult OnUpdate(float dt, PushdownState** pushFunc) = 0;
			virtual void OnAwake() {}
			virtual void OnSleep() {}
			
		protected:
		};
	}
}