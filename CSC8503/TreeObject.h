#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class TreeObject :public GameObject {
		public:
			TreeObject() {};
			virtual ~TreeObject() { hasCollided = false; };

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;
		};
	}
}