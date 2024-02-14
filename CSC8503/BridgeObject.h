#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class BridgeObject : public GameObject {
		public:
			BridgeObject() {};
			virtual ~BridgeObject() {};

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;
		};
	}
}