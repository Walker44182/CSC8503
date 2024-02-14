#pragma once
#include "MazeGame.h"
#include "NetworkBase.h"

namespace NCL {
	namespace CSC8503 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public MazeGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void SpawnPlayer();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

			static NetworkedGame* GetNetworkedGame() {
				return networkInstance;
			};

			bool IsServer() {
				return !thisServer ? false : true;
			};

			bool GetPauseMode() const {
				return inPauseMode;
			}
			void SetPauseMode(bool p) {
				inPauseMode = p;
			};

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void ClientSend(bool deltaFrame);
			void UpdateMinimumState();
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> serverPlayers;
			GameObject* localPlayer;

			static NetworkedGame* networkInstance;

			bool inPauseMode;
		};
	}
}
