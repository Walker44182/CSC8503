#pragma once
#include "GameObject.h"
#include "RenderObject.h"
#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class PenaltyObject :public GameObject {
		public:
			PenaltyObject();
			virtual ~PenaltyObject() {};

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;

		protected:
			Texture* basicTex;
		};
	}
}