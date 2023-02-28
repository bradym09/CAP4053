#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/Tile.h"
#include "../Framework/TileSystem/TileMap.h"
#include "../PriorityQueue.h"

using namespace std;

namespace ufl_cap4053
{
	namespace searches
	{
		class PathSearch
		{
		// CLASS DECLARATION GOES HERE
			private:
				struct Node {
					Tile* tile;
					vector<Node*> neighbors;
					double straightCost;
					double givenCost;
					Node* parent;
					Node(Tile* newTile) : tile(newTile) {};
					void clear() { 
						tile = nullptr;
						for (int i = 0; i < neighbors.size(); i++)
							neighbors[i] = nullptr;
						straightCost = 0;
						givenCost = 0;
						parent = nullptr; 
					}
				};

				int startRow, startCol, goalRow, goalCol;
				ufl_cap4053::PriorityQueue<Node*> pQ;
				vector<Node*> graph;
				vector<Tile const*> path;
				TileMap* map;
				Node* startNode;
				bool done;

				vector<Tile*> visitedTiles, completedTiles;

				bool areAdjacent(const Tile* lhs, const Tile* rhs);
				bool wasVisited(const Tile* tile, const vector<Tile*> visited);

				//template <typename T>
				static bool greaterThan(Node* const &lhs, Node* const &rhs);

			public:
				DLLEXPORT PathSearch(); // EX: DLLEXPORT required for public methods - see platform.h

				DLLEXPORT ~PathSearch();

				DLLEXPORT void load(TileMap* _tileMap);

				DLLEXPORT void initialize(int startRow, int startCol, int goalRow, int goalCol);

				DLLEXPORT void update(long timeslice);

				DLLEXPORT void shutdown();

				DLLEXPORT void unload();

				DLLEXPORT bool isDone() const;

				DLLEXPORT std::vector<Tile const*> const getSolution() const;
		};
	}
}  // close namespace ufl_cap4053::searches
