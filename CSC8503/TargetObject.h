#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class TargetObject : public GameObject {
		public:
			TargetObject() {};
			virtual ~TargetObject() {};

			void OnCollisionBegin(GameObject* otherObject, float penetration) override;

		};
	}
}