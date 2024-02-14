#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include "NetworkObject.h"
#include "GameServer.h"
#include "GameClient.h"

#define COLLISION_MSG 30

NetworkedGame* NetworkedGame::networkInstance = nullptr;

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};

NetworkedGame::NetworkedGame()	{
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket  = 0.0f;
	packetsToSnapshot = 0;
	inPauseMode = false;

	networkInstance = this;
}

NetworkedGame::~NetworkedGame()	{
	delete thisServer;
	delete thisClient;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 2);

	thisServer->RegisterPacketHandler(Delta_State, this);
	thisServer->RegisterPacketHandler(Full_State, this);
	thisServer->RegisterPacketHandler(Received_State, this);

	StartLevel();
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);

	SpawnPlayer();
}

void NetworkedGame::UpdateGame(float dt) {
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			UpdateAsServer(dt);	
		}
		else if (thisClient) {
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}
	MazeGame::UpdateGame(dt);
}

void NetworkedGame::UpdateAsServer(float dt) {
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}	
	if (player1->HasFinished() || player2->HasFinished())finished = true;
	thisServer->UpdateServer();
	for (int i = 0; i < 20;  ++i) {
		if (coins[i].IsActive()) {
			coins[i].Update(dt);
		}
	}
	player1->Update(dt);
	player2->Update(dt);
	item->Update(dt);
	goose->Update(dt);
	if (!inPauseMode) GoatMovement(player1);
}

void NetworkedGame::UpdateAsClient(float dt) {
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		ClientSend(false);
		packetsToSnapshot = 5;
	}
	else {
		ClientSend(true);
	}	
	if (player1->HasFinished() || player2->HasFinished())finished = true;
	thisClient->UpdateClient();
	player2->Update(dt);
	if (!inPauseMode) GoatMovement(player2);
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o || o->GetNetworkID() == 2) {
			continue;
		}
		//TODO - you'll need some way of determining
		//when a player has sent the server an acknowledgement
		//and store the lastID somewhere. A map between player
		//and an int could work, or it could be part of a 
		//NetworkPlayer struct. 
		int playerState = 0;
		GamePacket* newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
}

void NetworkedGame::ClientSend(bool deltaFrame) {
	int playerState = 0;
	GamePacket* newPacket = nullptr;
	if (player2->GetNetworkObject()->WritePacket(&newPacket, deltaFrame, playerState)) {
		thisClient->SendPacket(*newPacket);
		delete newPacket;
	}
}

void NetworkedGame::UpdateMinimumState() {
	//Periodically remove old data from the server
	int minID = INT_MAX;
	int maxID = 0; //we could use this to see if a player is lagging behind?

	for (auto i : stateIDs) {
		minID = std::min(minID, i.second);
		maxID = std::max(maxID, i.second);
	}
	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	}
}

void NetworkedGame::SpawnPlayer() {
	player2 = AddPlayerToWorld(Vector3(450, 30, 100), Vector4(1, 1, 0, 1), "player2", 2, true);
}

void NetworkedGame::StartLevel() {
	player1 = AddPlayerToWorld(Vector3(450, 30, -100), Vector4(0, 0, 1, 1), "player1", 1, true);
	AddBonusObjectToWorld(true);
	item = AddItemToWorld(true);
	goose = AddGooseToWorld(true);
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (thisClient) {
		std::vector<GameObject*>::const_iterator first;
		std::vector<GameObject*>::const_iterator last;
		world->GetObjectIterators(first, last);
		for (auto i = first; i != last; ++i) {
			NetworkObject* o = (*i)->GetNetworkObject();
			if (!o || o->GetNetworkID() == 2) {
				continue;
			}
			o->ReadPacket(*payload);
		}
	}
	else if (thisServer) {
		player2->GetNetworkObject()->ReadPacket(*payload);
	}
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b) {
	if (thisServer) { //detected a collision between players!
		MessagePacket newPacket;
		newPacket.messageID = COLLISION_MSG;
		newPacket.playerID  = a->GetPlayerNum();

		thisClient->SendPacket(newPacket);

		newPacket.playerID = b->GetPlayerNum();
		thisClient->SendPacket(newPacket);
	}
}