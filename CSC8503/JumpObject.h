#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class JumpObject :public GameObject {
		public:
			JumpObject() {};
			virtual ~JumpObject() {};

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;
		};
	}
}