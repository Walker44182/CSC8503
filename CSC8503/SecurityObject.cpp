#include "SecurityObject.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

SecurityObject::SecurityObject() {
	stateMachine = new StateMachine();

	mazeGrid = new MazeGrid1("MazeGrid.txt");
	objClosest = nullptr;
	back = false;
	velocity = 20.0f;
	direction = Vector3();

	State* StateA = new State([&](float dt)->void
		{
			MoveLeft(dt);
		}
	);
	State* StateB = new State([&](float dt)->void
		{
			MoveRight(dt);
		}
	);
	State* StateC = new State([&](float dt)->void
		{
			Attack();
		}
	);
	State* StateD = new State([&](float dt)->void
		{
			MoveBack();
		}
	);

	stateMachine->AddState(StateA);
	stateMachine->AddState(StateB);
	stateMachine->AddState(StateC);
	stateMachine->AddState(StateD);

	stateMachine->AddTransition(new StateTransition(StateA, StateB,
		[&]()->bool
		{
			if (transform.GetPosition().x > 770) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 90, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().x > 770;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{
			if (transform.GetPosition().x < 670) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, -90, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().x < 670;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateA, StateC,
		[&]()->bool
		{   bool flag = false;
	if (InMaze() &&	!player->GetInvisibility()) {
		flag = true;
	}
	return flag;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateB, StateC,
		[&]()->bool
		{   bool flag = false;
	if (InMaze() && !player->GetInvisibility()) {
		flag = true;
	}
	return flag;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateC, StateD,
		[&]()->bool
		{   bool flag = false;
	if (OutMaze() || player->GetInvisibility()) {
		testNodes.clear();
		flag = true;
	}
	return flag;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateD, StateA,
		[&]()->bool
		{   bool flag = false;
	if (back) {
		physicsObject->SetAngularVelocity(Vector3());
		testNodes.clear();
		back = false;
		flag = true;
	}
	return flag;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateD, StateC,
		[&]()->bool
		{   bool flag = false;
	if (InMaze() && !player->GetInvisibility()) {
		flag = true;
	}
	return flag;
		}
	));
}

SecurityObject::~SecurityObject() {
	delete stateMachine;
}

void SecurityObject::Update(float dt) {
	if (!hasCollided)stateMachine->Update(dt);
}

void SecurityObject::MoveLeft(float dt) {
	physicsObject->AddForce(Vector3(velocity, 0, 0));
}

void SecurityObject::MoveRight(float dt) {
	physicsObject->AddForce(Vector3(-velocity, 0, 0));
}

void SecurityObject::Attack() {
	NavigationPath outPath;
	RayCollision closestCollision;

	Vector3 startPos = transform.GetPosition();
	Vector3 endPos = player->GetTransform().GetPosition();
	mazeGrid->FindPath(startPos, endPos, outPath);
	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
	
	Vector3 rayDir = endPos - startPos;
	Vector3 rayPos = startPos;
	Ray r = Ray(rayPos, rayDir);
	bool found = false;
	if (TutorialGame::GetGame()->GetGameWorld()->Raycast(r, closestCollision, true, this)) {
		objClosest = (GameObject*)closestCollision.node;
		found = objClosest->GetType() == 1 ? true : false;
	}

	if (testNodes.size() > 1 && !found) {
		direction = testNodes[1] - testNodes[0];
		physicsObject->AddForce(direction.Normalised() * velocity);
		UpdateOrientation();
		Debug::DrawLine(testNodes[1], testNodes[0], Vector4(0, 1, 0, 1));
		testNodes.clear();
	}
	else if (testNodes.size() <= 1 || found) {
		transform.SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), GetAngle()));
		if (player->GetTransform().GetPosition().y < 30) {
			direction = player->GetTransform().GetPosition() -
				transform.GetPosition();
			physicsObject->AddForce(direction.Normalised() * velocity);
		}
		else {
			if (testNodes.size() > 1) {
				direction = testNodes[1] - testNodes[0];
				physicsObject->AddForce(direction.Normalised() * velocity);
				Debug::DrawLine(testNodes[1], testNodes[0], Vector4(0, 1, 0, 1));
				testNodes.clear();
			}
			else {
				direction = player->GetTransform().GetPosition() -
					transform.GetPosition();
				physicsObject->AddForce(direction.Normalised() * velocity);
			}
		}
	}
}

void SecurityObject::MoveBack() {
	NavigationPath outPath;

	Vector3 startPos = transform.GetPosition();
	Vector3 endPos = initPosition;
	mazeGrid->FindPath(startPos, endPos, outPath);
	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	}
	if (testNodes.size() > 1) {
		direction = testNodes[1] - testNodes[0];
		physicsObject->AddForce(direction.Normalised() * velocity);
		UpdateOrientation();
		Debug::DrawLine(testNodes[1], testNodes[0], Vector4(0, 1, 0, 1));
		testNodes.clear();
	}
	else {
		back = true;
	}
}

float SecurityObject::GetAngle() {
	Vector3 npcPosition = transform.GetPosition();
	Vector3 playerPosition = player->GetTransform().GetPosition();

	float z = npcPosition.z - playerPosition.z;
	float x = npcPosition.x - playerPosition.x;

	float tanValue = x / z;

	float angleValue = atan(tanValue);

	float floatValue = RadiansToDegrees(angleValue);
	if (z < 0)floatValue = 180 + floatValue;

	return  floatValue;
}

void SecurityObject::UpdateOrientation() {
	Quaternion rotation;
	if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
	else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
	else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
	else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
	transform.SetOrientation(rotation);
}

bool SecurityObject::InMaze() {
	if (abs(player->GetTransform().GetPosition().z) < 180 &&
		player->GetTransform().GetPosition().y > -20 &&
		player->GetTransform().GetPosition().x > 440 &&
		player->GetTransform().GetPosition().x < 830)
		return true;
	else return false;
}

bool SecurityObject::OutMaze() {
	if (abs(player->GetTransform().GetPosition().z) > 200 ||
		player->GetTransform().GetPosition().y < -30 ||
		player->GetTransform().GetPosition().x < 430 ||
		player->GetTransform().GetPosition().x > 850)
		return true;
	else return false;
}

void SecurityObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1 && player->GetInvisibility()) {
		renderObject->SetDefaultTexture(nullptr);
		renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
		physicsObject->SetResolve(false);
	}

	if (otherObject->GetType() == 38) {
		velocity = 4.0f;
	}

	if (otherObject->GetType() == 50) {
		SetActive(false);
	}
}

void SecurityObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 38) {
		velocity = 15.0f;
	}
}