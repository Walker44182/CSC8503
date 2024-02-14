#include "GooseObject.h"
#include "TutorialGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8503;

GooseObject::GooseObject() {
	stateMachine = new StateMachine();

	counter1 = 0.0f;
	counter2 = 0.0f;
	useCounter2 = false;
	gooseNotice = " ";
	life = 100;
	mazeGrid = new MazeGrid2("MazeGrid.txt");
	direction = Vector3();
	colour = Debug::RED;
	attack = false;
	foundPlayer = false;
	back = false;
	velocity = 25.0f;
	player = nullptr;
	objClosest = nullptr;

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
	State* StateE = new State([&](float dt)->void
		{
			sequence->Execute(dt);
		}
	);

	stateMachine->AddState(StateA);
	stateMachine->AddState(StateB);
	stateMachine->AddState(StateC);
	stateMachine->AddState(StateD);
	stateMachine->AddState(StateE);

	stateMachine->AddTransition(new StateTransition(StateA, StateB,
		[&]()->bool
		{
			if (transform.GetPosition().z < -100) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().z < -100;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{
			if (transform.GetPosition().z > 100) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().z > 100;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateA, StateC,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && !player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateA, StateE,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		attack = true;
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateB, StateC,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && !player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateB, StateE,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		attack = true;
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateC, StateD,
		[&]()->bool
		{   bool transition = false;
	if (OutMaze() || (player->GetInvisibility() && !foundPlayer)) {
		testNodes.clear();
		counter1 = 3.0f;
		if (player->GetInvisibility()) gooseNotice = "Angry Goose: He disappeared!!!";
		else gooseNotice = "Angry Goose: He ran away!";
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateC, StateE,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && player->GetItem()) {
		counter1 = 3.0f;
		testNodes.clear();
		attack = true;
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateD, StateA,
		[&]()->bool
		{   bool transition = false;
	if (back) {
		physicsObject->SetAngularVelocity(Vector3());
		testNodes.clear();
		back = false;
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateD, StateC,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && !player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateD, StateE,
		[&]()->bool
		{   bool transition = false;
	if (InMaze() && (!player->GetInvisibility() || foundPlayer) && player->GetItem()) {
		counter1 = 3.0f;
		if (!player->GetInvisibility()) gooseNotice = "Angry Goose: Someone is in our maze!!!";
		else gooseNotice = "Angry Goose: Someone invisible is in our maze!!!";
		attack = true;
		transition = true;
	}
	return transition;
		}
	));
	stateMachine->AddTransition(new StateTransition(StateE, StateD,
		[&]()->bool
		{   bool transition = false;
	if (OutMaze() || (player->GetInvisibility() && !foundPlayer)) {
		testNodes.clear();
		counter1 = 3.0f;
		if (player->GetInvisibility()) gooseNotice = "Angry Goose: He disappeared!!!";
		else gooseNotice = "Angry Goose: He ran away!";
		attack = false;
		transition = true;
	}
	return transition;
		}
	));

	BehaviourAction* MoveToPowerup = new BehaviourAction("MoveToPowerup",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				counter1 = 3.0f;
				if(!player->GetInvisibility()) gooseNotice = "Angry Goose: He stole our treasure, finding my powerup!!";
				else gooseNotice = "Angry Goose: Someone invisible stole our treasure, finding my powerup!";
				state = Ongoing;
			}
			else if (state == Ongoing) {
				NavigationPath outPath;

				Vector3 startPos = transform.GetPosition();
				Vector3 endPos = Vector3(800, 10, 160);
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
					return Success;
				}		
			}
			return state;
		});
	BehaviourAction* VelocityPowerup = new BehaviourAction("VelocityPowerup",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				return Ongoing;
			}
			else if (state == Ongoing) {
				int random = rand() % 2;
				if (random == 0) {
					powerup = 1;
					counter1 = 3.0f;
					gooseNotice = "Angry Goose: I've got my velocity powerup!";
					return Success;
				}
				else if (random == 1) {
					return Failure;
				}
			}
			return state;
		});
	BehaviourAction* LockDoor = new BehaviourAction("LockDoor",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				return Ongoing;
			}
			else if (state == Ongoing) {
				int random = rand() % 2;
				if (random == 0) {
					powerup = 2;
					counter1 = 3.0f;
					gooseNotice = "Angry Goose: I've locked the gates!";
					return Success;
				}
				else if (random == 1) {
					return Failure;
				}
			}
			return state;
		});
	BehaviourAction* AttackPowerup = new BehaviourAction("AttackPowerup",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				return Ongoing;
			}
			else if (state == Ongoing) {
				powerup = 3;
				counter1 = 3.0f;
				gooseNotice = "Angry Goose: I've got my attack powerup!";
				return Success;
			}
			return state;
		});
	BehaviourAction* ChasePlayer = new BehaviourAction("ChasePlayer",
		[&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				return Ongoing;
			}
			else if (state == Ongoing) {
				Attack();
			}
			return Ongoing;
		});
	BehaviourSelector* select = new BehaviourSelector("select");
	select->AddChild(VelocityPowerup);
	select->AddChild(LockDoor);
	select->AddChild(AttackPowerup);
	sequence = new BehaviourSequence("root");
	sequence->AddChild(MoveToPowerup);
	sequence->AddChild(select);
	sequence->AddChild(ChasePlayer);
	sequence->Reset();
}

GooseObject::~GooseObject(){
	delete stateMachine;
}

void GooseObject::Update(float dt) {
	if (life <= 0) {
		if (!flag2) {
			counter1 = 3.0f;
			gooseNotice = "Dead Goose: Goodbye :(";
			colour = Vector4(0.2f, 0.2f, 0.2f, 1);
			renderObject->SetDefaultTexture(nullptr);
			renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1));
			physicsObject->SetResolve(false);
			powerup = 0;
			life = 0;
			player->SetScore(player->GetScore() + 500);
			player->SetCounter1(3.0f);
			player->SetScoreNotice("Kill the goose! 500 points!!");
			player->SetColour1(Debug::RED);
			player->SetCounter2(3.0f);
			player->SetScoreNotice("You killed the angry goose!");
			player->SetColour2(Debug::RED);
			flag2 = true;
		}
	}

	if (life > 0) {
		stateMachine->Update(dt);

		counter1 -= dt;
		if (counter1 <= 0) {
			gooseNotice = " ";
		}
		if (useCounter2) {
			counter2 -= dt;
			if (counter2 <= 0) {
				renderObject->SetColour(Vector4(1, 0, 0, 1));
				useCounter2 = false;
			}
		}
		if (powerup == 1) {
			velocity = 40.0f;
		}
		if (powerup == 3) {
			flag = true;
		}

		if (hasCollided) {
			useCounter2 = true;
			counter2 = 1.0f;
			renderObject->SetColour(Vector4(1, 0, 1, 1));
			life--;
			hasCollided = false;
		}

		if ((transform.GetPosition() - player->GetTransform().GetPosition()).Length() > 80) {
			foundPlayer = false;
		}
	}
	Debug::Print(gooseNotice, Vector2(5, 60), colour);
	if (abs(player->GetTransform().GetPosition().z) < 180 &&
		player->GetTransform().GetPosition().y > -20 &&
		player->GetTransform().GetPosition().x > 440 &&
		player->GetTransform().GetPosition().x < 830 && life >= 0) {
		Debug::Print("Goose HP: " + std::to_string(life) + "/100", Vector2(5, 20), Debug::RED);
	}

}

void GooseObject::MoveLeft(float dt) {
	physicsObject->AddForce(Vector3(0, 0, -velocity));
}

void GooseObject::MoveRight(float dt) {
	physicsObject->AddForce(Vector3(0, 0, velocity));
}

void GooseObject::Attack() {
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
	TutorialGame::GetGame()->GetGameWorld()->Raycast(r, closestCollision, true, this);
	objClosest = (GameObject*)closestCollision.node;
	bool found = objClosest->GetType() == 1 ? true : false;

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

void GooseObject::MoveBack() {
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

float GooseObject::GetAngle() {
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

void GooseObject::UpdateOrientation() {
	Quaternion rotation;
	if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
	else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
	else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
	else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
	transform.SetOrientation(rotation);
}

bool GooseObject::InMaze() {
	if (abs(player->GetTransform().GetPosition().z) < 180 &&
		player->GetTransform().GetPosition().y > -20 &&
		player->GetTransform().GetPosition().x > 440 &&
		player->GetTransform().GetPosition().x < 830)
		return true;
	else return false;
}

bool GooseObject::OutMaze() {
	if (abs(player->GetTransform().GetPosition().z) > 200 ||
		player->GetTransform().GetPosition().y < -30 ||
		player->GetTransform().GetPosition().x < 430 ||
		player->GetTransform().GetPosition().x > 850)
		return true;
	else return false;
}

void GooseObject::OnCollisionBegin(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 1) {
		if (player->GetInvisibility()) {
			foundPlayer = true;
		}
	}

	if (otherObject->GetType() == 38) {
		velocity = 40.0f;
	}

	if (otherObject->GetType() == 50) {
		powerup = 0;
		isActive = false;
	}
}

void GooseObject::OnCollisionEnd(GameObject* otherObject, float penetration) {
	if (otherObject->GetType() == 38) {
		velocity = powerup == 1 ? 40.0f : 25.0f;
	}
}

MazeGoose::MazeGoose() {
	stateMachine = new StateMachine();

	mazeGrid = new MazeGrid2("MazeGrid2.txt");
	direction = Vector3();
	player = nullptr;
	objClosest = nullptr;

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

	stateMachine->AddState(StateA);
	stateMachine->AddState(StateB);
	stateMachine->AddState(StateC);

	stateMachine->AddTransition(new StateTransition(StateA, StateB,
		[&]()->bool
		{
			if (transform.GetPosition().z < -100) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 180, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().z < -100;
		}
	));

	stateMachine->AddTransition(new StateTransition(StateB, StateA,
		[&]()->bool
		{
			if (transform.GetPosition().z > 100) {
				transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(0, 0, 0));
				physicsObject->SetAngularVelocity(Vector3());
			}
			return transform.GetPosition().z > 100;
		}
	));

   stateMachine->AddTransition(new StateTransition(StateA, StateC,
   	    [&]()->bool
   	    {
		   return player == nullptr ? false : true;
   	    }
   ));

   stateMachine->AddTransition(new StateTransition(StateB, StateC,
	   [&]()->bool
	   {
		   return player == nullptr ? false : true;
	   }
   ));
}

MazeGoose::~MazeGoose() {
	delete stateMachine;
}

void MazeGoose::Update(float dt) {
	if (item->GetMazePlayer() != nullptr) {
		player = item->GetMazePlayer();
	}
	stateMachine->Update(dt);
}

void MazeGoose::MoveLeft(float dt) {
	physicsObject->AddForce(Vector3(0, 0, -80));
}

void MazeGoose::MoveRight(float dt) {
	physicsObject->AddForce(Vector3(0, 0, 80));
}

void MazeGoose::Attack() {
	if (player) {
		float velocity = 80.0f;
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
		if (NetworkedGame::GetNetworkedGame()->GetGameWorld()->Raycast(r, closestCollision, true, this)) {
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
			direction = player->GetTransform().GetPosition() -
				transform.GetPosition();
			physicsObject->AddForce(direction.Normalised() * velocity);
		}
	}
}

float MazeGoose::GetAngle() {
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

void MazeGoose::UpdateOrientation() {
	Quaternion rotation;
	if (direction.x > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, -90, 0);
	else if (direction.x < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 90, 0);
	else if (direction.z > 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 180, 0);
	else if (direction.z < 0) rotation = Quaternion::EulerAnglesToQuaternion(0, 0, 0);
	transform.SetOrientation(rotation);
}