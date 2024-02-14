#pragma once
#include "GameObject.h"
#include "RenderObject.h"

namespace NCL{
	namespace CSC8503 {
		class BonusObject :public GameObject {
		public:
			BonusObject() {};
			virtual ~BonusObject() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;
		};

		class MazeBonus : public BonusObject {
			public:
			MazeBonus() {};
			virtual ~MazeBonus() {};

			virtual void Update(float dt) override;
		};
	}
}