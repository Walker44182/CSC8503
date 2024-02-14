#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class BombObject : public GameObject {
		public:
			BombObject() {};
			virtual ~BombObject() {};

			void OnCollisionEnd(GameObject* otherObject, float penetration) override;
		};
	}
}