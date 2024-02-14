#include "PlayerObject.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

PlayerObject::PlayerObject(const std::string& objectName) {
	name = objectName;
	powerup = false;
	item = false;
	canJump = false;
	canShoot = false;
	canHook = false;
	canFly = false;
	isHit = false;
	velocityPowerup = false;
	gravityPowerup = false;
	invisibilityPowerup = false;
	hidePowerup1 = false;
	hidePowerup2 = false;
	hidePowerup3 = false;
	die = false;
	win = false;
	inRayMode = false;
	hurt = 0;
	key = 0;
	score = 0;
	life = 5;
	velocity = 30.0f;
	jumpHeight = 1500.0f;
	counter1 = 0.0f;
	counter2 = 0.0f;
	counter3 = 0.0f;
	counter4 = 0.0f;
	counter5 = 0.0f;
	colour1 = Vector4(1, 1, 1, 1);
	colour2 = Vector4(1, 1, 1, 1);
	colour3 = Vector4(1, 1, 1, 1);
	colour4 = Vector4(0.24f, 0.70f, 0.44f, 1);
	scoreNotice = " ";
	worldNotice = " ";
	powerupNotice = " ";
	guideNotice = " ";
	shootObject = nullptr;
	hookObject = nullptr;
}

void PlayerObject::Update(float dt) {
	counter1 -= dt;
	counter2 -= dt;
	counter3 -= dt;
	counter4 -= dt;
	counter5 -= dt;
	if (counter1 <= 0) {
		colour1 = Vector4(1, 1, 1, 1);
		scoreNotice = " ";
	}
	if (counter2 <= 0) {
		colour2 = Vector4(1, 1, 1, 1);
		worldNotice = " ";
	}
	if (counter3 <= 0) {
		colour3 = Vector4(1, 1, 1, 1);
		powerupNotice = " ";
	}
	if (counter4 <= 0) {
		guideNotice = " ";
	}
	if (useCounter) {
		counter -= dt;
		if (counter <= 0) {
			if (!invisibilityPowerup) renderObject->SetColour(Vector4(0, 0, 1, 1));
			else renderObject->SetColour(Vector4(0, 0, 1, 0.5f));
			counter = 0.0f;
			useCounter = false;
		}
	}

	if (die) {
		powerup = false;
		item = false;
		key = 0;
		velocity = 30.0f;
		jumpHeight = 1500.0f;
		canShoot = false;
		canHook = false;
		canFly = false;
		physicsObject->SetGravity(true);
		physicsObject->SetGravityValue(Vector3(0, -20, 0));
		gravityPowerup = false;
		invisibilityPowerup = false;
		inRayMode = false;
		renderObject->SetColour(Vector4(0, 0, 1, 1));
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
		transform
			.SetPosition(Vector3(-130, 10, 0))
			.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -90, 0));
		counter2 = 3.0f;
		worldNotice = "Please Try Again :)";
		life = 5;
		die = false;
	}

	UpdateKeys();

	if (!inRayMode) {
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}

	if (isHit) {
		life -= hurt;
		if (!invisibilityPowerup) renderObject->SetColour(Vector4(1, 0, 0, 1));
		else renderObject->SetColour(Vector4(1, 0, 0, 0.5f));
		useCounter = true;
		counter = 1.0f;
		isHit = false;
		hurt = 0;
	}

	if (life <= 0) {
		die = true;
	}

	if (canShoot) {
		Shoot();
	}

	if (canHook) {
		Hook();
	}

	Debug::Print("Score:" + std::to_string(score), Vector2(5, 95), Debug::RED);
	Debug::Print(scoreNotice, Vector2(5, 80), colour1);
	Debug::Print(worldNotice, Vector2(5, 75), colour2);
	Debug::Print(powerupNotice, Vector2(5, 70), colour3);
	Debug::Print(guideNotice, Vector2(5, 65), colour4);
	Debug::Print(std::to_string((int)transform.GetPosition().x) + "," + 
		         std::to_string((int)transform.GetPosition().y) + "," + 
		         std::to_string((int)transform.GetPosition().z), Vector2(5,15), Debug::BLACK);
};

void PlayerObject::UpdateKeys() {
	if (powerup) {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::R)) {
			powerup = false;
			velocity = 30.0f;
			jumpHeight = 1500.0f;
			canShoot = false;
			canHook = false;
			canFly = false;
			inRayMode = false;
			physicsObject->SetGravity(true);
			physicsObject->SetGravityValue(Vector3(0, -20, 0));
			gravityPowerup = false;
			invisibilityPowerup = false;
			renderObject->SetColour(Vector4(0, 0, 1, 1));
		}
	}
	if (key != 0) {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::T)) {
			key = 0;
		}
	}
	if (gravityPowerup) {
		Debug::Print("Press G to swap gravity direction!", Vector2(5, 85));
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::G)) {
			physicsObject->SetGravityValue(Vector3(0, -physicsObject->GetGravityValue().y, 0));
		}
	}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() >= 100) {
		canJump = true;
	}

	if (otherObject->GetType() == 11) {
		if (!otherObject->GetCollide()) {
			collideScore = 400 * penetration;
			if (collideScore > 100)collideScore = 100;
			score += collideScore;
			otherObject->SetCollide(true);
			counter1 = 3.0f;
			colour1 = Vector4(0, 1, 1, 1);
			scoreNotice = "Colliding!!!" + std::to_string(collideScore) + "points";
		}
	}

	if (otherObject->GetType() == 12) {
		if (!otherObject->GetCollide()) {
			collideScore = 400 * penetration;
			if (collideScore > 100)collideScore = 100;
			score += collideScore;
			otherObject->SetCollide(true);
			counter1 = 3.0f;
			colour1 = Vector4(0, 1, 1, 1);
			scoreNotice = "Colliding!!!" + std::to_string(collideScore) + "points";
		}
	}

	if (otherObject->GetType() == 13) {
		if (!otherObject->GetCollide()) {
			collideScore = 400 * penetration;
			if (collideScore > 100)collideScore = 100;
			score += collideScore;
			otherObject->SetCollide(true);
			counter1 = 3.0f;
			colour1 = Vector4(0, 1, 1, 1);
			scoreNotice = "Colliding!!!" + std::to_string(collideScore) + "points";
			counter2 = 3.0f;
			colour2 = Vector4(0, 1, 1, 1);
			worldNotice = "A new powerup has been added to the world!!";

			if (!hidePowerup1) {
				hidePowerup1 = true;
			}
		}
	}

	if (otherObject->GetType() == 34 || otherObject->GetType() == 36) {
		if (!otherObject->GetCollide()) {
			collideScore = 400 * penetration;
			if (collideScore > 100)collideScore = 100;
			score += collideScore;
			otherObject->SetCollide(true);
			counter1 = 3.0f;
			colour1 = Vector4(0, 1, 1, 1);
			scoreNotice = "Colliding!!!" + std::to_string(collideScore) + "points";
		}
	}

	if (otherObject->GetType() == 35) {
		if (invisibilityPowerup) {
			if (!otherObject->GetCollide()) {
				collideScore = 800 * penetration;
				if (collideScore > 200)collideScore = 200;
				score += collideScore;
				otherObject->SetCollide(true);
				counter1 = 3.0f;
				colour1 = Vector4(0, 1, 1, 1);
				scoreNotice = "Sneaking!!!" + std::to_string(collideScore) + "points";
			}
		}
		else {
			if (counter5 <= 0) {
				hurt = 1;
				isHit = true;
				counter5 = 3.0f;
			}
		}
	}

	if (otherObject->GetType() == 39) {
		if (counter5 <= 0) {
			if (otherObject->GetFlag()) {
				hurt = 2;
			}
			else {
				hurt = 1;
			}
			isHit = true;
			counter5 = 3.0f;
		}
	}

	if (otherObject->GetType() == 20) {
		if (!otherObject->GetCollect()) {
			score += 100;
			otherObject->SetCollect(true);
			counter1 = 3.0f;
			colour1 = Vector4(1, 1, 0, 1);
			scoreNotice = "Coin! 100 points!!";
		}
	}

	if (otherObject->GetType() == 21) {
		if (!otherObject->GetCollide()) {
			counter2 = 3.0f;
			colour2 = Vector4(1, 1, 0, 1);
			worldNotice = "You found the hidden coin!!";
			otherObject->SetCollide(true);
			if (!hideCoin1) {
				hideCoin1 = true;
			}
		}
	}

	if (otherObject->GetType() == 22) {
		if (!otherObject->GetCollide()) {
			counter2 = 3.0f;
			colour2 = Vector4(1, 1, 0, 1);
			worldNotice = "You found the hidden coin!!";
			otherObject->SetCollide(true);
			if (!hideCoin2) {
				hideCoin2 = true;
			}
		}
	}

	if (otherObject->GetType() == 23) {
		if (!key) {
			counter1 = 3.0f;
			colour1 = Vector4(1, 0, 0, 1);
			scoreNotice = "The gate is locked, please find a key!";
		}
		else {
			if (!otherObject->GetCollide()) {
				score += 200;
				counter1 = 3.0f;
				colour1 = Vector4(1, 0, 1, 1);
				scoreNotice = "Passing The Gate! 200 points!!";
				otherObject->SetCollide(true);
			}
		}
	}

	if (otherObject->GetType() == 24) {
		if (!otherObject->GetCollide()) {
			score += 300;
			counter1 = 3.0f;
			colour1 = Vector4(1, 0, 1, 1);
			scoreNotice = "Crossing The Gap! 300 points!!";
			otherObject->SetCollide(true);
		}
	}

	if (otherObject->GetType() == 25) {
		if (!otherObject->GetCollide()) {
			score += 150;
			counter1 = 3.0f;
			colour1 = Vector4(1, 0, 1, 1);
			scoreNotice = "Reaching the island! 150 points!!";
			otherObject->SetCollide(true);
		}
	}

	if (otherObject->GetType() == 26) {
		if (!otherObject->GetCollide()) {
			score += 120;
			counter1 = 3.0f;
			colour1 = Vector4(1, 0, 1, 1);
			scoreNotice = "Take Off! 120 points!!";
			otherObject->SetCollide(true);
		}
	}

	if (otherObject->GetType() == 31) {
		if (!invisibilityPowerup) {
			if (!hidePowerup1) {
				counter4 = 3.0f;
				guideNotice = "Guide: Try to hit that tower!";
			}
			else {
				counter4 = 3.0f;
				guideNotice = "Congratulations! You found the hidden powerup!";
			}
		}
		else {
			counter4 = 3.0f;
			guideNotice = "Guide: ??Is anyone here?";
		}
	}
	if (otherObject->GetType() == 32) {
		if (!invisibilityPowerup) {
			if (!hidePowerup2 && !hidePowerup3) {
				counter4 = 3.0f;
				guideNotice = "Guide: Try to shoot at the targets!";
			}
			else if (hidePowerup2 || hidePowerup3) {
				counter4 = 3.0f;
				guideNotice = "Congratulations! You found the hidden powerup!";
			}
		}
		else {
			counter4 = 3.0f;
			guideNotice = "Guide: ??Is anyone here?";
		}
	}
	if (otherObject->GetType() == 33) {
		if (!invisibilityPowerup) {
			if (!hideCoin1 && !hideCoin2) {
				counter4 = 3.0f;
				guideNotice = "Guide: Try to hit the two blocks on the opposite!";
			}
			else if (hideCoin1 || hideCoin2) {
				counter4 = 3.0f;
				guideNotice = "Congratulations! You found the hidden coin!";
			}
		}
		else {
			counter4 = 3.0f;
			guideNotice = "Guide: ??Is anyone here?";
		}
	}

	if (otherObject->GetType() == 101) {
		if (item && !win) {
			win = true;
			item = false;
			score += 500;
			counter1 = 3.0f;
			colour1 = Vector4(1, 0, 0, 1);
			scoreNotice = "Finish! 500 points!!";
			counter2 = 3.0f;
			colour2 = Vector4(1, 0, 0, 1);
			worldNotice = "You win!!!";
		}
	}

	if (otherObject->GetType() == 38) {
		if (!velocityPowerup)velocity = 4.0f;
		else velocity = 20.0f;
	}

	if (otherObject->GetType() == 50) {
		die = true;
	}

	if (!powerup) {
		if (otherObject->GetType() == 2) {
			powerup = true;
			SetVelocity(100);
			counter3 = 3.0f;
			colour3 = Vector4(0, 0, 1, 1);
			powerupNotice = "Velocity Powerup!!";
		}
		else if (otherObject->GetType() == 3) {
			powerup = true;
			SetJumpHeight(4000);
			counter3 = 3.0f;
			colour3 = Vector4(1, 1, 0, 1);
			powerupNotice = "Jump Powerup!!";
		}
		else if (otherObject->GetType() == 4) {
			powerup = true;
			canShoot = true;
			counter3 = 3.0f;
			colour3 = Vector4(1, 0, 0, 1);
			powerupNotice = "Shooting Powerup!!";
		}
		else if (otherObject->GetType() == 5) {
			powerup = true;
			canHook = true;
			counter3 = 3.0f;
			colour3 = Vector4(0, 0, 0, 1);
			powerupNotice = "Hook Powerup!!";
		}
		else if (otherObject->GetType() == 14) {
			powerup = true;
			canFly = true;
			physicsObject->SetGravity(false);
			counter3 = 3.0f;
			colour3 = Vector4(0, 1, 1, 1);
			powerupNotice = "Fly Powerup!!";
		}
		else if (otherObject->GetType() == 18) {
			powerup = true;
			gravityPowerup = true;
			counter3 = 3.0f;
			colour3 = Vector4(0.24f, 0.70f, 0.55f, 1);
			powerupNotice = "Gravity Powerup!!";
		}
		else if (otherObject->GetType() == 19) {
			powerup = true;
			invisibilityPowerup = true;
			renderObject->SetColour(Vector4(0, 0, 1, 0.5f));
			counter3 = 3.0f;
			colour3 = Vector4(1, 0, 1, 1);
			powerupNotice = "Invisibility Powerup!!";
		}
	}

	if (!key) {
		if (otherObject->GetType() == 6) {
			key = 1;
			counter3 = 3.0f;
			colour3 = Vector4(1, 0, 0, 1);
			powerupNotice = "You found a key!!";
		}
		else if (otherObject->GetType() == 7) {
			key = 2;
			counter3 = 3.0f;
			colour3 = Vector4(0, 0, 1, 1);
			powerupNotice = "You found a key!!";
		}
	}

	if (!item && !win) {
		if (otherObject->GetType() == 30) {
			item = true;
			counter2 = 2.0f;
			colour2 = Vector4(0, 1, 1, 1);
			worldNotice = "You found the target item!!";
		}
	}
}

void PlayerObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 38) {
		if (!velocityPowerup)velocity = 20.0f;
		else velocity = 100.0f;
	}

	if (otherObject->GetType() >= 100) {
		canJump = false;
	}
}

bool PlayerObject::Shoot() {
	if (TutorialGame::GetGame()->GetCameraMode() == 1) {
		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
			inRayMode = !inRayMode;
			if (inRayMode) {
				Window::GetWindow()->ShowOSPointer(true);
				Window::GetWindow()->LockMouseToWindow(true);
			}
			else {
				Window::GetWindow()->ShowOSPointer(false);
				Window::GetWindow()->LockMouseToWindow(true);
			}
		}
		if (inRayMode) {
			SetCursorPos(650, 300);
			int shootScore = 0;
			float local = 0.0f;
			Debug::Print("Rightclick to leave shooting mode!", Vector2(5, 85));
			Debug::Print("Leftclick to shoot!", Vector2(5, 90));
			if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Left)) {
				Ray ray = CollisionDetection::BuildRayFromMouse(TutorialGame::GetGame()->GetGameWorld()->GetMainCamera());
				RayCollision closestCollision;
				if (TutorialGame::GetGame()->GetGameWorld()->Raycast(ray, closestCollision, true)) {
					shootObject = (GameObject*)closestCollision.node;
					Vector3 direction = closestCollision.collidedAt - transform.GetPosition();
					float force = direction.Length();
					Debug::DrawLine(transform.GetPosition(), closestCollision.collidedAt, Vector4(1, 0, 0, 1), 0.1f);
					if (shootObject->GetShot()) {
						if (shootObject->GetType() != 39) {
							shootObject->GetPhysicsObject()->AddTorque((closestCollision.collidedAt - shootObject->GetTransform().GetPosition()) * 5000);
							shootObject->GetPhysicsObject()->AddForce(direction * 30);
						}
						if (shootObject->GetType() == 17) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
								shootObject->GetPhysicsObject()->SetGravity(true);
								shootObject->SetCollide(true);
								local = (closestCollision.collidedAt - shootObject->GetTransform().GetPosition()).Length();
								if (local != 0)shootScore = (1 / local) * 500;
								else shootScore = 50;
								score += shootScore;
								counter1 = 3.0f;
								colour1 = Vector4(1, 0, 0, 1);
								scoreNotice = "Hit it! " + std::to_string(shootScore) + "points!!";
							}
						}
						else if (shootObject->GetType() == 15) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
								shootObject->GetPhysicsObject()->SetGravity(true);
								shootObject->SetCollide(true);
								local = (closestCollision.collidedAt - shootObject->GetTransform().GetPosition()).Length();
								if (local != 0)shootScore = (1 / local) * 500;
								else shootScore = 50;
								score += shootScore;
								counter1 = 3.0f;
								colour1 = Vector4(1, 0, 0, 1);
								scoreNotice = "Hit it! " + std::to_string(shootScore) + "points!!";
								counter2 = 3.0f;
								colour2 = Vector4(0.24f, 0.70f, 0.55f, 1);
								worldNotice = "A new powerup has been added to the world";

								if (!hidePowerup2) {
									hidePowerup2 = true;
								}
							}
						}
						else if (shootObject->GetType() == 16) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
								shootObject->GetPhysicsObject()->SetGravity(true);
								shootObject->SetCollide(true);
								local = (closestCollision.collidedAt - shootObject->GetTransform().GetPosition()).Length();
								if (local != 0)shootScore = (1 / local) * 500;
								else shootScore = 50;
								score += shootScore;
								counter1 = 3.0f;
								colour1 = Vector4(1, 0, 0, 1);
								scoreNotice = "Hit it! " + std::to_string(shootScore) + "points!!";
								counter2 = 3.0f;
								colour2 = Vector4(1, 0, 1, 1);
								worldNotice = "A new powerup has been added to the world";

								if (!hidePowerup3) {
									hidePowerup3 = true;
								}
							}
						}
						else if (shootObject->GetType() == 34) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetDefaultTexture(nullptr);
								shootObject->GetRenderObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
								shootObject->GetPhysicsObject()->SetResolve(false);
								shootObject->SetCollide(true);
								if (closestCollision.collidedAt.y - shootObject->GetTransform().GetPosition().y < 5) {
									score += 70;
									counter1 = 3.0f;
									colour1 = Vector4(1, 0, 0, 1);
									scoreNotice = "Hit it! 70 points!!";
								}
								else {
									score += 100;
									counter1 = 3.0f;
									colour1 = Vector4(0, 1, 1, 1);
									scoreNotice = "Headshot! 100 points!!";
								}
							}
						}
						else if (shootObject->GetType() == 35) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetDefaultTexture(nullptr);
								shootObject->GetRenderObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
								shootObject->GetPhysicsObject()->SetResolve(false);
								shootObject->SetCollide(true);
								if (InMaze()) {
									if (closestCollision.collidedAt.y - shootObject->GetTransform().GetPosition().y < 5) {
										score += 70;
										counter1 = 3.0f;
										colour1 = Vector4(1, 0, 0, 1);
										scoreNotice = "Hit it! 70 points!!";
									}
									else {
										score += 100;
										counter1 = 3.0f;
										colour1 = Vector4(0, 1, 1, 1);
										scoreNotice = "Headshot! 100 points!!";
									}
								}
							}
						}
						else if (shootObject->GetType() == 39) {
							if (!shootObject->GetCollide()) {
								if (InMaze()) {
									shootObject->GetPhysicsObject()->AddTorque((closestCollision.collidedAt - shootObject->GetTransform().GetPosition()) * 500);
									shootObject->GetPhysicsObject()->AddForce(direction* force * 0.001);
									shootObject->SetCollide(true);
								}
							}
						}
						else if (shootObject->GetType() == 36) {
							if (!shootObject->GetCollide()) {
								shootObject->GetRenderObject()->SetDefaultTexture(nullptr);
								shootObject->GetRenderObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
								shootObject->GetPhysicsObject()->SetResolve(false);
								shootObject->SetCollide(true);
								if (closestCollision.collidedAt.y - shootObject->GetTransform().GetPosition().y < 8) {
									score += 70;
									counter1 = 3.0f;
									colour1 = Vector4(1, 0, 0, 1);
									scoreNotice = "Hit it! 70 points!!";
								}
								else {
									score += 100;
									counter1 = 3.0f;
									colour1 = Vector4(0, 1, 1, 1);
									scoreNotice = "Headshot! 100 points!!";
								}
							}
						}
						else if (shootObject->GetType() == 37) {
							if (!shootObject->GetCollide()) {
								int velocity = shootObject->GetPhysicsObject()->GetLinearVelocity().Length();
								int shootScore = 0.25 * velocity;
								score += shootScore;
								shootObject->SetCollide(true);
								shootObject->SetActive(false);
								counter1 = 3.0f;
								colour1 = Vector4(1, 0, 0, 1);
								scoreNotice = "Hit it!" + std::to_string(shootScore) + "points!!";
							}
						}
					}
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			Debug::Print("Rightclick to enter shooting mode!", Vector2(5, 85));
		}
	}
}

bool PlayerObject::Hook() {
	if (TutorialGame::GetGame()->GetCameraMode() == 1) {
		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
			inRayMode = !inRayMode;
			if (inRayMode) {
				Window::GetWindow()->ShowOSPointer(true);
				Window::GetWindow()->LockMouseToWindow(true);
			}
			else {
				Window::GetWindow()->ShowOSPointer(false);
				Window::GetWindow()->LockMouseToWindow(true);
			}
		}
		if (inRayMode) {
			SetCursorPos(650, 300);
			Debug::Print("Rightclick to leave hook mode!", Vector2(5, 85));
			Debug::Print("Leftclick to use hook!", Vector2(5, 90));
			if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Left)) {
				Ray ray = CollisionDetection::BuildRayFromMouse(TutorialGame::GetGame()->GetGameWorld()->GetMainCamera());
				RayCollision closestCollision;
				if (TutorialGame::GetGame()->GetGameWorld()->Raycast(ray, closestCollision, true)) {
					hookObject = (GameObject*)closestCollision.node;
					if (hookObject->GetHooked()) {
						Vector3 direction = closestCollision.collidedAt - transform.GetPosition();
						float force = direction.Length();
						if (force < 1000)force = 1000;
						Debug::DrawLine(transform.GetPosition(), closestCollision.collidedAt, Vector4(1, 1, 1, 1), 0.5f);
						physicsObject->AddForce(direction * force * 0.1);
						return true;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
		}
		else {
			Debug::Print("Rightclick to enter hook mode!", Vector2(5, 85));
		}
	}
}

bool PlayerObject::InMaze() {
	if (abs(transform.GetPosition().z) < 180 &&
		transform.GetPosition().y > -20 &&
		transform.GetPosition().x > 440 &&
		transform.GetPosition().x < 830)
		return true;
	else return false;
}

MazePlayer::MazePlayer(const std::string& objectName) {
	name = objectName;
	counter1 = 0.0f;
	isHit = false;
	finish = false;
}

void MazePlayer::Update(float dt) {
	counter1 -= dt;
	if (useCounter) {
		counter -= dt;
		if (counter <= 0) {
			renderObject->SetColour(initColour);
			counter = 0.0f;
			useCounter = false;
		}
	}

	if (isHit) {
		playerScore -= 50;
		renderObject->SetColour(Vector4(1, 0, 0, 1));
		useCounter = true;
		counter = 0.03f;
		isHit = false;
	}

	if (playerScore < 0) {
		playerScore = 0;
	}
}

void MazePlayer::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 20) {
		playerScore += 100;
		GameWorld::GetWorld()->RemoveGameObject(otherObject, false);
	}
	if (otherObject->GetType() == 30) {
		hasItem = true;
	}
	if (otherObject->GetType() == 39) {
		if (counter1 <= 0) {
			isHit = true;
			counter1 = 0.1f;
		}
	}
	if (name == "player1" && otherObject->GetType() == 26 && hasItem && !finish) {
		playerScore += 1000;
		finish = true;
	}
	if (name == "player2" && otherObject->GetType() == 27 && hasItem && !finish) {
		playerScore += 1000;
		finish = true;
	}
}