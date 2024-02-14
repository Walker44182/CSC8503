#include "MazeGrid.h"
#include "Assets.h"

#include <fstream>

using namespace NCL;
using namespace CSC8503;

MazeGrid1::MazeGrid1(const std::string& filename) {
	std::ifstream infile(Assets::DATADIR + filename);

	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	allNodes = new GridNode[gridWidth * gridHeight];

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];
			char type = 0;
			infile >> type;
			n.type = type;
			n.position = Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));
		}
	}

	//now to build the connectivity between the nodes
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];

			if (y > 0) { //get the above node
				n.connected[0] = &allNodes[(gridWidth * (y - 1)) + x];
			}
			if (y < gridHeight - 1) { //get the below node
				n.connected[1] = &allNodes[(gridWidth * (y + 1)) + x];
			}
			if (x > 0) { //get left node
				n.connected[2] = &allNodes[(gridWidth * (y)) + (x - 1)];
			}
			if (x < gridWidth - 1) { //get right node
				n.connected[3] = &allNodes[(gridWidth * (y)) + (x + 1)];
			}
			for (int i = 0; i < 4; ++i) {
				if (n.connected[i]) {
					if (n.connected[i]->type == '.') {
						n.costs[i] = 1;
					}
					if (n.connected[i]->type == '&') {
						n.costs[i] = 40;
					}
					if (n.connected[i]->type == '%') {
						n.costs[i] = 20;
					}
					if (n.connected[i]->type == 'x') {
						n.connected[i] = nullptr; //actually a wall, disconnect!
					}
				}
			}
		}
	}
}

MazeGrid1::~MazeGrid1() {
	delete[] allNodes;
}

bool MazeGrid1::FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) {
	//need to work out which node 'from' sits in, and 'to' sits in
	int fromX = (((int)from.x - 440) / nodeSize);
	int fromZ = (((int)from.z + 200) / nodeSize);

	int toX = (((int)to.x - 440) / nodeSize);
	int toZ = (((int)to.z + 200) / nodeSize);

	if (fromX < 0 || fromX > gridWidth - 1 ||
		fromZ < 0 || fromZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	if (toX < 0 || toX > gridWidth - 1 ||
		toZ < 0 || toZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	GridNode* startNode = &allNodes[(fromZ * gridWidth) + fromX];
	GridNode* endNode = &allNodes[(toZ * gridWidth) + toX];

	std::vector<GridNode*>  openList;
	std::vector<GridNode*>  closedList;

	openList.push_back(startNode);

	startNode->f = 0;
	startNode->g = 0;
	startNode->parent = nullptr;

	GridNode* currentBestNode = nullptr;

	while (!openList.empty()) {
		currentBestNode = RemoveBestNode(openList);

		if (currentBestNode == endNode) {			//we've found the path!
			GridNode* node = endNode;
			while (node != nullptr) {
				outPath.PushWaypoint(Vector3(node->position.x + 450, node->position.y, node->position.z - 190));
				node = node->parent;
			}
			return true;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				GridNode* neighbour = currentBestNode->connected[i];
				if (!neighbour) { //might not be connected...
					continue;
				}
				bool inClosed = NodeInList(neighbour, closedList);
				if (inClosed) {
					continue; //already discarded this neighbour...
				}

				float h = Heuristic(neighbour, endNode);
				float g = currentBestNode->g + currentBestNode->costs[i];
				float f = h + g;

				bool inOpen = NodeInList(neighbour, openList);

				if (!inOpen) { //first time we've seen this neighbour
					openList.emplace_back(neighbour);
				}
				if (!inOpen || f < neighbour->f) {//might be a better route to this neighbour
					neighbour->parent = currentBestNode;
					neighbour->f = f;
					neighbour->g = g;
				}
			}
			closedList.emplace_back(currentBestNode);
		}
	}
	return false; //open list emptied out with no path!
}

MazeGrid2::MazeGrid2(const std::string& filename) {
	std::ifstream infile(Assets::DATADIR + filename);

	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	allNodes = new GridNode[gridWidth * gridHeight];

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];
			char type = 0;
			infile >> type;
			n.type = type;
			n.position = Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));
		}
	}

	//now to build the connectivity between the nodes
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode& n = allNodes[(gridWidth * y) + x];

			if (y > 0) { //get the above node
				n.connected[0] = &allNodes[(gridWidth * (y - 1)) + x];
			}
			if (y < gridHeight - 1) { //get the below node
				n.connected[1] = &allNodes[(gridWidth * (y + 1)) + x];
			}
			if (x > 0) { //get left node
				n.connected[2] = &allNodes[(gridWidth * (y)) + (x - 1)];
			}
			if (x < gridWidth - 1) { //get right node
				n.connected[3] = &allNodes[(gridWidth * (y)) + (x + 1)];
			}
			for (int i = 0; i < 4; ++i) {
				if (n.connected[i]) {
					if (n.connected[i]->type == '.') {
						n.costs[i] = 1;
					}
					if (n.connected[i]->type == '&') {
						n.costs[i] = 0.01;
					}
					if (n.connected[i]->type == '%') {
						n.costs[i] = 20;
					}
					if (n.connected[i]->type == 'x') {
						n.connected[i] = nullptr; //actually a wall, disconnect!
					}
				}
			}
		}
	}
}

MazeGrid2::~MazeGrid2() {
	delete[] allNodes;
}

bool MazeGrid2::FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath) {
	//need to work out which node 'from' sits in, and 'to' sits in
	int fromX = (((int)from.x - 440) / nodeSize);
	int fromZ = (((int)from.z + 200) / nodeSize);

	int toX = (((int)to.x - 440) / nodeSize);
	int toZ = (((int)to.z + 200) / nodeSize);

	if (fromX < 0 || fromX > gridWidth - 1 ||
		fromZ < 0 || fromZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	if (toX < 0 || toX > gridWidth - 1 ||
		toZ < 0 || toZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	GridNode* startNode = &allNodes[(fromZ * gridWidth) + fromX];
	GridNode* endNode = &allNodes[(toZ * gridWidth) + toX];

	std::vector<GridNode*>  openList;
	std::vector<GridNode*>  closedList;

	openList.push_back(startNode);

	startNode->f = 0;
	startNode->g = 0;
	startNode->parent = nullptr;

	GridNode* currentBestNode = nullptr;

	while (!openList.empty()) {
		currentBestNode = RemoveBestNode(openList);

		if (currentBestNode == endNode) {			//we've found the path!
			GridNode* node = endNode;
			while (node != nullptr) {
				outPath.PushWaypoint(Vector3(node->position.x + 450, node->position.y, node->position.z - 190));
				node = node->parent;
			}
			return true;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				GridNode* neighbour = currentBestNode->connected[i];
				if (!neighbour) { //might not be connected...
					continue;
				}
				bool inClosed = NodeInList(neighbour, closedList);
				if (inClosed) {
					continue; //already discarded this neighbour...
				}

				float h = Heuristic(neighbour, endNode);
				float g = currentBestNode->g + currentBestNode->costs[i];
				float f = h + g;

				bool inOpen = NodeInList(neighbour, openList);

				if (!inOpen) { //first time we've seen this neighbour
					openList.emplace_back(neighbour);
				}
				if (!inOpen || f < neighbour->f) {//might be a better route to this neighbour
					neighbour->parent = currentBestNode;
					neighbour->f = f;
					neighbour->g = g;
				}
			}
			closedList.emplace_back(currentBestNode);
		}
	}
	return false; //open list emptied out with no path!
}