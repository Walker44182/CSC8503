#pragma once
#include "TutorialGame.h"
#include "GameWorld.h"
#include "PhysicsObject.h"
#include "RenderObject.h"
#include "TextureLoader.h"

#include "PositionConstraint.h"
#include "HingeConstraint.h"
#include "StateGameObject.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame* TutorialGame::instance = nullptr;

TutorialGame::TutorialGame() : controller(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse()) {
	world		= new GameWorld();
#ifdef USEVULKAN
	renderer	= new GameTechVulkanRenderer(*world);
	renderer->Init();
	renderer->InitStructures();
#else 
	renderer = new GameTechRenderer(*world);
#endif

	physics		= new PhysicsSystem(*world);

	forceMagnitude	= 10.0f;
	useGravity		= true;
	inSelectionMode = false;
	flag1           = false;
	flag2           = false;
	flag3           = false;
	flag4           = false;
	flag5           = false;
	enterLeft       = false;
	enterRight      = false;
	cameraMode      = 1;

	mazeGrid = new NavigationGrid("MazeGrid.txt");

	world->GetMainCamera().SetController(controller);
	physics->UseGravity(useGravity);

	controller.MapAxis(0, "Sidestep");
	controller.MapAxis(1, "UpDown");
	controller.MapAxis(2, "Forward");

	controller.MapAxis(3, "XLook");
	controller.MapAxis(4, "YLook");

	InitialiseAssets();
	InitCamera();
	InitWorld();
	srand(time(0));

	instance = this;
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::InitialiseAssets() {
	cubeMesh	 = renderer->LoadMesh("cube.msh");
	sphereMesh	 = renderer->LoadMesh("sphere.msh");
	charMesh	 = renderer->LoadMesh("goat.msh");
	enemyMesh	 = renderer->LoadMesh("Keeper.msh");
	guideMesh    = renderer->LoadMesh("courier.msh");
	securityMesh = renderer->LoadMesh("security.msh");
	bonusMesh	 = renderer->LoadMesh("coin.msh");
	capsuleMesh  = renderer->LoadMesh("capsule.msh");
	coneMesh     = renderer->LoadMesh("Cone.msh");
	treeMesh     = renderer->LoadMesh("tree.msh");
	gooseMesh    = renderer->LoadMesh("goose.msh");

	basicTex	 = renderer->LoadTexture("checkerboard.png");
	treeTex      = renderer->LoadTexture("tree_diffuse.png");
	dogeTex      = renderer->LoadTexture("Default.png");
	waterTex     = renderer->LoadTexture("water.tga");
	plankTex     = renderer->LoadTexture("birch_planks.png");

	basicShader  = renderer->LoadShader("scene.vert", "scene.frag");
}

TutorialGame::~TutorialGame()	{
	delete  capsuleMesh;
	delete	cubeMesh;
	delete	sphereMesh;
	delete	coneMesh;
	delete	charMesh;
	delete	enemyMesh;
	delete	guideMesh;
	delete	securityMesh;
	delete	bonusMesh;
	delete	treeMesh;
	delete	gooseMesh;

	delete  basicTex;
	delete	treeTex;
	delete	dogeTex;
	delete	waterTex;
	delete	plankTex;

	delete basicShader;

	delete physics;
	delete renderer;
	delete world;

	if (instance = this) instance = nullptr;
}

void TutorialGame::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera().UpdateCamera(dt);
	}
	if (cameraMode == 1) {
		CameraUpdate1();
	}
	if (cameraMode == 3) {
		CameraUpdate2();
	}

	UpdateKeys();

	SelectObject();
	MoveSelectedObject();

	if (playerGoat->GetDie()) {
		velocityPowerup->Update(dt);
		jumpPowerup->Update(dt);
		shootPowerup->Update(dt);
		shootPowerup->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));
		hookPowerup->Update(dt);
		hookPowerup->GetRenderObject()->SetColour(Vector4(0, 0, 0, 1));
		if (flyPowerup)flyPowerup->Update(dt);
		if (invisibilityPowerup)invisibilityPowerup->Update(dt);
		if (gravityPowerup)gravityPowerup->Update(dt);
		leftKey->Update(dt);
		rightKey->Update(dt);
		item->Update(dt);
		world->GetMainCamera().SetPitch(0);
		world->GetMainCamera().SetYaw(-90);
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}

	if(!playerGoat->GetWin()){
		if (!playerGoat->GetItem()) {
			Debug::Print("Find the target item!", Vector2(5, 5), Vector4(0, 1, 1, 1));
		}
		else {
			Debug::Print("Take the item back to the starting point!", Vector2(5, 5), Vector4(0, 1, 1, 1));
		}
	}
	else {
		Debug::Print("You win! Explore the world!!", Vector2(5, 5), Vector4(0, 1, 1, 1));
	}

	Debug::Print("Life Left: " + std::to_string(playerGoat->GetLife()), Vector2(5, 10), Vector4(1, 0, 0, 1));
	Debug::Print("Press Esc to pause", Vector2(5, 25), Debug::WHITE);


	world->UpdateWorld(dt);

	if (playerGoat->GetKey() && goose->GetPowerup() != 2) {
		if (playerGoat->GetKey() == 1) {
			leftGate->GetPhysicsObject()->SetInverseMass(1);
			leftGate->GetPhysicsObject()->InitCubeInertia();
		}
		else if (playerGoat->GetKey() == 2) {
			rightGate->GetPhysicsObject()->SetInverseMass(1);
			rightGate->GetPhysicsObject()->InitCubeInertia();
		}
	}
	else {
		leftGate->GetTransform().SetPosition(Vector3(300, 30, -60)).SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
		leftGate->GetPhysicsObject()->SetInverseMass(0);
		leftGate->GetPhysicsObject()->InitCubeInertia();
		rightGate->GetTransform().SetPosition(Vector3(300, 30, 60)).SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
		rightGate->GetPhysicsObject()->SetInverseMass(0);
		rightGate->GetPhysicsObject()->InitCubeInertia();
	}

	if (playerGoat->GetHide1() && !flag1) {
		flyPowerup = AddPlayerPowerupToWorld(Vector3(-100, 10, 100), 14, "Fly", Vector4(0, 1, 1, 1));
		flag1 = true;
	}
	if (playerGoat->GetHide2() && !flag2) {
		invisibilityPowerup = AddPlayerPowerupToWorld(Vector3(-50, 10, 100), 18, "Vision", Vector4(0.24f, 0.70f, 0.55f, 1));
		flag2 = true;
	}
	if (playerGoat->GetHide3() && !flag3) {
		gravityPowerup = AddPlayerPowerupToWorld(Vector3(-50, 10, 50), 19, "Invisibility", Vector4(1, 0, 1, 0.5f));
		flag3 = true;
	}
	if (playerGoat->GetHide4() && !flag4) {
		hideCoin1 = AddHiddenBonusObjectToWorld(Vector3(-130, 11, 190));
		flag4 = true;
	}
	if (playerGoat->GetHide5() && !flag5) {
		hideCoin2 = AddHiddenBonusObjectToWorld(Vector3(-55, 11, 190));
		flag5 = true;
	}

	renderer->Update(dt);
	physics->Update(dt);
	renderer->Render();
	Debug::UpdateRenderables(dt);
}

void TutorialGame::CameraUpdate1() {
	Matrix4 view = world->GetMainCamera().BuildViewMatrix();
	Matrix4 camWorld = view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 objPos = lockedObject->GetTransform().GetPosition();
	Vector3 camPos = Vector3(objPos.x - (fwdAxis.x * 20), objPos.y + 14, objPos.z - (fwdAxis.z * 20));

	Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos + Vector3(0, 13, 0), Vector3(0, 1, 0));

	world->GetMainCamera().SetPosition(camPos);
	if (!inSelectionMode)world->GetMainCamera().SetAngleFromMouse();
}

void TutorialGame::CameraUpdate2() {

	Vector3 objPos = lockedObject->GetTransform().GetPosition();
	Vector3 camPos = Vector3(objPos.x, objPos.y + 200, objPos.z);

	Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

	world->GetMainCamera().SetPosition(camPos);
	world->GetMainCamera().SetPitch(-90);
	world->GetMainCamera().SetYaw(-90);
}

void TutorialGame::UpdateKeys() {
	//if (Window::GetKeyboard()->KeyPressed(KeyCodes::F1)) {
	//	InitWorld(); //We can reset the simulation at any time with F1
	//	selectionObject = nullptr;
	//}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::F8)) {
		world->ShuffleObjects(false);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM1)) {
		cameraMode = 1;
		world->GetMainCamera().SetPitch(0);
		world->GetMainCamera().SetYaw(-90);
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM2)) {
		cameraMode = 2;
		playerGoat->SetRayMode(false);
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
		InitCamera();
	}
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::NUM3)) {
		cameraMode = 3;
		playerGoat->SetRayMode(false);
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}

	if (cameraMode == 1) {
		GoatMovement1();
	}
	else {
		GoatMovement2();
	}
}

void TutorialGame::GoatMovement1() {
	Matrix4 view		= world->GetMainCamera().BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	float force = playerGoat->GetVelocity();
	float jumpHeight = playerGoat->GetJumpHeight();

	if (Window::GetKeyboard()->KeyDown(KeyCodes::A)) {
		playerGoat->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::D)) {
		Vector3 worldPos = playerGoat->GetTransform().GetPosition();
		playerGoat->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::W)) {
		playerGoat->GetPhysicsObject()->AddForce(fwdAxis * force);
		playerGoat->GetTransform().SetOrientation(
			Quaternion::EulerAnglesToQuaternion(0, world->GetMainCamera().GetYaw(), 0));
		playerGoat->GetPhysicsObject()->SetAngularVelocity(Vector3());
	}

	if (Window::GetKeyboard()->KeyDown(KeyCodes::S)) {
		playerGoat->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}
	if (!playerGoat->GetFly()) {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::SPACE)) {
			if (playerGoat->GetJump())
				playerGoat->GetPhysicsObject()->AddForce(Vector3(0, 1, 0) * jumpHeight);
		}
	}
	else {
		if (Window::GetKeyboard()->KeyDown(KeyCodes::SPACE)) {
			playerGoat->GetPhysicsObject()->AddForce(Vector3(0, force, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyCodes::SHIFT)) {
			playerGoat->GetPhysicsObject()->AddForce(Vector3(0, -force, 0));
		}
	}
}

void TutorialGame::GoatMovement2() {
	float force = playerGoat->GetVelocity();
	float jumpHeight = playerGoat->GetJumpHeight();

	if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
		playerGoat->GetPhysicsObject()->AddForce(Vector3(0, 0, -1) * force);
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
		playerGoat->GetPhysicsObject()->AddForce(Vector3(0, 0, 1) * force);
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
		playerGoat->GetPhysicsObject()->AddForce(Vector3(-1, 0, 0) * force);
	}
	if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
		playerGoat->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * force);
	}
	if (!playerGoat->GetFly()) {
		if (Window::GetKeyboard()->KeyPressed(KeyCodes::O)) {
			if (playerGoat->GetJump())
				playerGoat->GetPhysicsObject()->AddForce(Vector3(0, 1, 0) * jumpHeight);
		}
	}
	else {
		if (Window::GetKeyboard()->KeyDown(KeyCodes::O)) {
			playerGoat->GetPhysicsObject()->AddForce(Vector3(0, force, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyCodes::P)) {
			playerGoat->GetPhysicsObject()->AddForce(Vector3(0, -force, 0));
		}
	}
}

void TutorialGame::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float invCubeMass = 5;
	int numLinks = 10;
	float maxDistance = 30;
	float cubeDistance = 20;

	Vector3 startPos = Vector3(50, 200, 50);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0)
		           , cubeSize, 0);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 4)
		           * cubeDistance, 0, 0), cubeSize, 0);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) *
			     cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous,
			     block, maxDistance);
		world->AddConstraint(constraint);
		previous = block;
	}
	PositionConstraint* constraint = new PositionConstraint(previous, 
		        end, maxDistance);
	world->AddConstraint(constraint);
}

void TutorialGame::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyCodes::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyCodes::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}
}



/*
Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool TutorialGame::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyCodes::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		//Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::Left)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				selectionObject = (GameObject*)closestCollision.node;

				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				return true;
			}
			else {
				return false;
			}
		}
		if (Window::GetKeyboard()->KeyPressed(NCL::KeyCodes::L)) {
			if (selectionObject) {
				if (lockedObject == selectionObject) {
					lockedObject = nullptr;
				}
				else {
					lockedObject = selectionObject;
				}
			}
		}
	}
	else {
		//Debug::Print("Press Q to change to select mode!", Vector2(5, 85));
	}
	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/

void TutorialGame::MoveSelectedObject() {
	//Debug::Print("Click Force:" + std::to_string(forceMagnitude), Vector2(5, 90));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;//we haven't selected anything!
	}
	//Push the selected object!
	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::Right)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}
}

void TutorialGame::TestPathfinding() {
	NavigationPath outPath;

	Vector3 startPos = securityNPCs[0].GetTransform().GetPosition();
	Vector3 endPos = playerGoat->GetTransform().GetPosition();

	bool found = mazeGrid->FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
}

void TutorialGame::DisplayPathfinding() {
	for (int i = 1; i < testNodes.size(); ++i) {
		Vector3 a = testNodes[i - 1];
		Vector3 b = testNodes[i];

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
	testNodes.clear();
}