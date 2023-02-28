#include "PathSearch.h"

using namespace std;

namespace ufl_cap4053
{
	namespace searches
	{
		// CLASS DEFINITION GOES HERE

		//PRIVATE HELPER FUNCTIONS
		
		//Checks if two tiles are adjacent
		bool PathSearch::areAdjacent(const Tile* lhs, const Tile* rhs) {
			int leftRow = lhs->getRow();
			int leftCol = lhs->getColumn();
			int rightRow = rhs->getRow();
			int rightCol = rhs->getColumn();

			int rowDist = rightRow - leftRow;
			int colDist = rightCol - leftCol;

			//Odd row
			if (leftRow % 2 == 1) {
				if (rowDist == 0) {
					if (colDist == 1 || colDist == -1)
						return true;
				}
				else if (rowDist == -1) {
					if (colDist == 0 || colDist == 1)
						return true;
				}
				else if (rowDist == 1) {
					if (colDist == 0 || colDist == 1)
						return true;
				}
			}
			//Even row
			else {
				if (rowDist == 0) {
					if (colDist == 1 || colDist == -1)
						return true;
				}
				else if (rowDist == -1) {
					if (colDist == -1 || colDist == 0)
						return true;
				}
				else if (rowDist == 1) {
					if (colDist == -1 || colDist == 0)
						return true;
				}
			}

			return false;
		}

		//Determine if tile was visited
		bool PathSearch::wasVisited(const Tile* tile, const vector<Tile*> visited) {
			for (int i = 0; i < visited.size(); i++) {
				if (visited[i] == tile)
					return true;
			}
			return false;
		}

		//Is left > right? using tile weight
		//template <typename T>
		bool PathSearch::greaterThan(Node* const& lhs, Node* const& rhs) {
			
			//return (lhs->straightCost > rhs->straightCost);
			//return (lhs->givenCost > rhs->givenCost);
			double leftEstCost = (1.0 * lhs->straightCost) + lhs->givenCost;
			double rightEstCost = (1.0 * rhs->straightCost) + rhs->givenCost;
			return (leftEstCost > rightEstCost);
		}
		
		//PUBLIC FUNCTIONS

		PathSearch::PathSearch() : pQ(&greaterThan) {
			startRow = 0;
			startCol = 0;
			goalRow = 0;
			goalCol = 0;

			done = false;

			path = {};
		}

		PathSearch::~PathSearch() {
			vector<Tile*> inQueue;
			queue<Node*> q;
			q.push(startNode);
			while (!q.empty()) {
				Node* current = q.front();
				q.pop();

				for (int i = 0; i < current->neighbors.size(); i++) {
					if (!wasVisited(current->neighbors[i]->tile, inQueue)) {
						q.push(current->neighbors[i]);
						inQueue.push_back(current->neighbors[i]->tile);
					}
				}

				delete current;
			}

			for (int i = 0; i < path.size(); i++) {
				path[i] = nullptr;
			}
			path = {};

			for (int i = 0; i < visitedTiles.size(); i++) {
				visitedTiles[i] = nullptr;
			}
			visitedTiles = {};

			for (int i = 0; i < completedTiles.size(); i++) {
				completedTiles[i] = nullptr;
			}
			completedTiles = {};

			pQ.clear();

			while (!graph.empty()) {
				graph.back()->clear();
				delete graph.back();
			}
		}
		
		void PathSearch::load(TileMap* _tileMap) {
			map = _tileMap;

			map->setStartTile(0, 0);
			map->setGoalTile(0, 0);

			vector<Tile*> visited;
			vector<Node*> inQueue;
			queue<Node*> q;
			Node* start = new Node(map->getStartTile());
			q.push(start);
			visited.push_back(start->tile);
			inQueue.push_back(start);

			while (!q.empty()) {
				Node* current = q.front();
				q.pop();

				//Node* node;

				Tile* neighbor = nullptr;

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (i == 0 && j == 0) continue;
						
						neighbor = map->getTile(current->tile->getRow() + i, current->tile->getColumn() + j);
						if (neighbor != NULL && areAdjacent(current->tile, neighbor) && neighbor->getWeight() != 0) {
							Node* newNode;// = new Node(neighbor);

							if (!wasVisited(neighbor, visited)) {
								visited.push_back(neighbor);
								newNode = new Node(neighbor);
								q.push(newNode);
								inQueue.push_back(newNode);
							}
							else {
								//Might not be in graph
								for (int i = 0; i < inQueue.size(); i++) {
									if (inQueue[i]->tile == neighbor) {
										newNode = inQueue[i];
										break;
									}
								}
							}

							current->neighbors.push_back(newNode);
						}
					}
				}

				//Save start node
				if (current->tile == map->getStartTile())
					startNode = current;
				
				graph.push_back(current);
			}
		}

		void PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol) {
			if (startRow < 0)
				this->startRow = 0;
			else if (startRow > map->getRowCount())
				this->startRow = map->getRowCount() - 1;
			else
				this->startRow = startRow;

			if (startCol < 0)
				this->startCol = 0;
			else if (startCol > map->getColumnCount())
				this->startCol = map->getColumnCount() - 1;
			else
				this->startCol = startCol;

			if (goalRow < 0)
				this->goalRow = 0;
			else if (goalRow > map->getRowCount())
				this->goalRow = map->getRowCount() - 1;
			else
				this->goalRow = goalRow;

			if (goalCol < 0)
				this->goalCol = 0;
			else if (goalCol > map->getColumnCount())
				this->goalCol = map->getColumnCount() - 1;
			else
				this->goalCol = goalCol;

			map->setStartTile(startRow, startCol);
			map->setGoalTile(goalRow, goalCol);

			Tile* goal = map->getGoalTile();
			for (int i = 0; i < graph.size(); i++) {
				if (graph[i]->tile == map->getStartTile()) {
					startNode = graph[i];
					startNode->parent = nullptr;
					startNode->givenCost = 0;
					startNode->straightCost = abs(sqrt(pow(goal->getXCoordinate() - startNode->tile->getXCoordinate(), 2) +
						pow(goal->getYCoordinate() - startNode->tile->getYCoordinate(), 2)));
				}
			}

			visitedTiles = {};
			done = false;
		}

		void PathSearch::update(long timeslice) {

			Tile* goal = map->getGoalTile();

			if (pQ.empty() && visitedTiles.empty()) {
				pQ.push(startNode);
				visitedTiles.push_back(startNode->tile);
			}

			//Reset draw
			map->resetTileDrawing();

			//Update colors
			for (Tile* i : completedTiles)
				i->setFill(0xFF0000FF);
			
			PriorityQueue<Node*> copy = pQ;
			for (int i = 0; i < copy.size(); i++) {
				Node* curr = copy.front();
				curr->tile->setMarker((0xFF00FF00) - (i * (0xFF000000 / copy.size())));
				copy.pop();
			}

			//Draw tested path
			Node* draw = pQ.front();
			while (draw != nullptr) {
				if (draw->parent != nullptr)
					draw->tile->addLineTo(draw->parent->tile, 0xFFFF0000);
				draw = draw->parent;
			}

			do {
				auto startTime = std::chrono::high_resolution_clock::now();

				Node* current = pQ.front();
				pQ.pop();

				//Goal check
				if (current->tile == map->getGoalTile()) {
					done = true;
					path = {};
						
					//Save path
					Node* temp = current;
					while (temp != nullptr) {
						path.push_back(temp->tile);
						temp = temp->parent;
					}

					break;
				}

				for (int i = 0; i < current->neighbors.size(); i++) {
					double newCost = current->givenCost + current->tile->getWeight();

					if (!wasVisited(current->neighbors[i]->tile, visitedTiles)) {
						current->neighbors[i]->parent = current;

						//Given cost
						current->neighbors[i]->givenCost = newCost;

						//Straight line cost
						current->neighbors[i]->straightCost = abs(sqrt(pow(goal->getXCoordinate() - current->neighbors[i]->tile->getXCoordinate(), 2) +
							pow(goal->getYCoordinate() - current->neighbors[i]->tile->getYCoordinate(), 2)));

						pQ.push(current->neighbors[i]);
						visitedTiles.push_back(current->neighbors[i]->tile);
					}

					//Uniform cost
					else {
						if (newCost < current->neighbors[i]->givenCost) {
							current->neighbors[i]->givenCost = newCost;
							current->neighbors[i]->parent = current;

							//Add neighbor to the priority queue
							if (!wasVisited(current->neighbors[i]->tile, visitedTiles))
								pQ.push(current->neighbors[i]);

							//Refresh priority queue (push and pop a temp node)
							else {
								Node* temp = new Node(nullptr);
								temp->givenCost = -999;
								pQ.push(temp);
								if (pQ.front()->givenCost != -999)
									cout << "ERROR: Temp node not at front of pQ\n";
								pQ.pop();
								delete temp;
							}
						}
					}
				}

				completedTiles.push_back(current->tile);
				
				//Apply elapsed time
				auto stopTime = std::chrono::high_resolution_clock::now();
				auto duration = duration_cast<std::chrono::milliseconds>(stopTime - startTime);
				timeslice -= (long)duration.count();

			} while (timeslice > 0 && !pQ.empty());
		}

		void PathSearch::shutdown() {
			map->resetTileDrawing();

			for (int i = 0; i < path.size(); i++) {
				path[i] = nullptr;
			}
			path = {};

			for (int i = 0; i < visitedTiles.size(); i++) {
				visitedTiles[i] = nullptr;
			}
			visitedTiles = {};

			for (int i = 0; i < completedTiles.size(); i++) {
				completedTiles[i] = nullptr;
			}
			completedTiles = {};

			pQ.clear();
		}

		void PathSearch::unload() {
			graph = {};

			/*
			vector<Tile*> inQueue;
			queue<Node*> q;
			if (startNode != nullptr)
				q.push(startNode);
			while (!q.empty()) {
				Node* current = q.front();
				q.pop();

				for (int i = 0; i < current->neighbors.size(); i++) {
					if (current->neighbors[i] != nullptr && !wasVisited(current->neighbors[i]->tile, inQueue)) {
						q.push(current->neighbors[i]);
						inQueue.push_back(current->neighbors[i]->tile);
					}
				}

				delete current;
			}*/
		}

		bool PathSearch::isDone() const {
			return done;
		}

		std::vector<Tile const*> const PathSearch::getSolution() const {
			return path;
		}
	}
}  // close namespace ufl_cap4053::searches
