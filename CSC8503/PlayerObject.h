#pragma once
#include "GameObject.h"
#include "RenderObject.h"
#include "Window.h"
#include "Debug.h"

namespace NCL {
	namespace CSC8503 {
		class PlayerObject : public GameObject {
		public:
			PlayerObject(const std::string& objectName);
			virtual ~PlayerObject() {};

			virtual void Update(float dt) override;

			void UpdateKeys();

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			virtual void OnCollisionEnd(GameObject* otherObject, float penetration) override;

			bool GetPowerup() const {
				return powerup;
			}
			void SetPowerup(bool p) {
				powerup = p;
			}

			int GetKey() const {
				return key;
			}
			void SetKey(int k) {
				key = k;
			}

			bool GetItem() const {
				return item;
			}
			void SetItem(bool i) {
				item = i;
			}

			bool GetInvisibility() const {
				return invisibilityPowerup;
			}
			void SetInvisibility(bool i) {
				invisibilityPowerup = i;
			}

			int GetScore() const {
				return score;
			}
			void SetScore(int s) {
				score = s;
			}

			float GetVelocity() const {
				return velocity;
			}
			void SetVelocity(float v) {
				velocity = v;
			}

			float GetJumpHeight() const {
				return jumpHeight;
			}
			void SetJumpHeight(float j) {
				jumpHeight = j;
			}

			bool GetJump() const {
				return canJump;
			}

			bool GetShoot() const {
				return canShoot;
			}

			bool GetHook() const {
				return canHook;
			}

			bool GetFly() const {
				return canFly;
			}

			bool GetHit() const {
				return isHit;
			}
			void SetHit(bool h) {
				isHit = h;
			}

			int GetLife() const {
				return life;
			}
			void SetLife(int l) {
				life = l;
			}

			void SetHurt(int h) {
				hurt = h;
			}

			bool GetHide1() const {
				return hidePowerup1;
			}
			void SetHide1(bool h) {
				hidePowerup1 = h;
			}

			bool GetHide2() const {
				return hidePowerup2;
			}
			void SetHide2(bool h) {
				hidePowerup2 = h;
			}

			bool GetHide3() const {
				return hidePowerup3;
			}
			void SetHide3(bool h) {
				hidePowerup3 = h;
			}

			bool GetHide4() const {
				return hideCoin1;
			}
			void SetHide4(bool h) {
				hideCoin1 = h;
			}

			bool GetHide5() const {
				return hideCoin2;
			}
			void SetHide5(bool h) {
				hideCoin2 = h;
			}

			bool GetDie() const {
				return die;
			}
			void SetDie(bool d) {
				die = d;
			}

			bool GetWin() const {
				return win;
			}
			void SetWin(bool w) {
				win = w;
			}

			void SetScoreNotice(std::string s) {
				scoreNotice = s;
			}
			void SetCounter1(float c) {
				counter1 = c;
			}
			void SetColour1(Vector4 c) {
				colour1 = c;
			}

			void SetWorldNotice(std::string w) {
				worldNotice = w;
			}
			void SetCounter2(float c) {
				counter2 = c;
			}
			void SetColour2(Vector4 c) {
				colour2 = c;
			};

			bool GetRayMode() const {
				return inRayMode;
			}
			void SetRayMode(bool r) {
				inRayMode = r;
			}

		protected:
			bool Shoot();
			bool Hook();
			bool InMaze();

			bool powerup;
			bool item;
			bool canJump;
			bool canShoot;
			bool canHook;
			bool canFly;
			bool isHit;
			bool velocityPowerup;
			bool gravityPowerup;
			bool invisibilityPowerup;
			bool hidePowerup1;
			bool hidePowerup2;
			bool hidePowerup3;
			bool hideCoin1;
			bool hideCoin2;
			bool die;
			bool win;
			bool inRayMode;
			int key;
			int score;
			int collideScore;
			int life;
			int hurt;
			float velocity;
			float jumpHeight;

			float counter1;
			float counter2;
			float counter3;
			float counter4;
			float counter5;
			Vector4 colour1;
			Vector4 colour2;
			Vector4 colour3;
			Vector4 colour4;
			std::string scoreNotice;
			std::string worldNotice;
			std::string powerupNotice;
			std::string guideNotice;
			
			GameObject* shootObject;
			GameObject* hookObject;
		};

		class MazePlayer :public GameObject {
		public:
			MazePlayer(const std::string& objectName);
			virtual ~MazePlayer() {};

			virtual void Update(float dt) override;

			virtual void OnCollisionBegin(GameObject* otherObject, float penetration) override;

			int GetPlayerID() const {
				return playerID;
			};
			void SetPlayerID(int id) {
				playerID = id;
			};

			int GetPlayerScore() const {
				return playerScore;
			};
			void SetPlayerScore(int s) {
				playerScore = s;
			};

			void SetInitColour(const Vector4& c) {
				initColour = c;
			};

			bool GetItem() const {
				return hasItem;
			}
			void SetItem(bool i) {
				hasItem = i;
			}

			bool HasFinished() const {
				return finish;
			};
			void SetFinished(bool f) {
				finish = f;
			}

		protected:
			float counter1;
			bool isHit;
			bool finish;
			bool hasItem;
			Vector4 initColour;
			int playerID;
			int playerScore;
		};
	}
}