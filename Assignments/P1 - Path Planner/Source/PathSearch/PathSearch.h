#include "../platform.h" // This file will make exporting DLL symbols simpler for students.
#include "../Framework/TileSystem/Tile.h"
#include "../Framework/TileSystem/TileMap.h"
#include <vector>

namespace ufl_cap4053
{
	namespace searches
	{
		class PathSearch
		{
		// CLASS DECLARATION GOES HERE
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
