#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL {
	namespace CSC8503 {
		class TowerObject : public GameObject {
		public:
			TowerObject() {};
			virtual ~TowerObject() {};
			
			void OnCollisionBegin(GameObject* otherObject, float penetration) override;

		};
	}
}