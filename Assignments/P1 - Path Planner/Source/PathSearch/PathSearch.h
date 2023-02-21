#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/Tile.h"
#include "../Framework/TileSystem/TileMap.h"
#include <vector>
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
				int startRow, startCol, goalRow, goalCol;
				ufl_cap4053::PriorityQueue<Tile const*> pQ;
				vector<Tile const*> path;
				TileMap* map;
				bool done;

				bool areAdjacent(const Tile* lhs, const Tile* rhs);
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
