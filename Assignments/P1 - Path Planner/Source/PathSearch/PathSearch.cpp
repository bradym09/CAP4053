#include "PathSearch.h"

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
					if (colDist <= 1 && colDist >= -1)
						return true;
				}
				else if (rowDist == -1) {
					if (colDist == 0)
						return true;
				}
				else if (rowDist == 1) {
					if (colDist <= 1 && colDist >= -1)
						return true;
				}
			}
			//Even row
			else {
				if (rowDist == 0) {
					if (colDist <= 1 && colDist >= -1)
						return true;
				}
				else if (rowDist == -1) {
					if (colDist <= 1 && colDist >= -1)
						return true;
				}
				else if (rowDist == 1) {
					if (colDist == 0)
						return true;
				}
			}

			return false;
		}

		//Is left > right?
		bool PathSearch::greaterThan(const int& lhs, const int& rhs) {
			return false;
		}

		//PUBLIC FUNCTIONS

		PathSearch::PathSearch() : pQ(PriorityQueue(greaterThan) {
			startRow = 0;
			startCol = 0;
			goalRow = 0;
			goalCol = 0;

			//pQ = PriorityQueue(greaterThan);

			done = false;
		}

		PathSearch::~PathSearch() {

		}

		void PathSearch::load(TileMap* _tileMap) {
			_tileMap->resetTileDrawing();


		}

		void PathSearch::initialize(int startRow, int startCol, int goalRow, int goalCol) {
			this->startRow = startRow;
			this->startCol = startCol;
			this->goalRow = goalRow;
			this->goalCol = goalCol;

			map->setStartTile(startRow, startCol);
			map->setGoalTile(goalRow, goalCol);
		}

		void PathSearch::update(long timeslice) {

		}

		void PathSearch::shutdown() {

		}

		void PathSearch::unload() {

		}

		bool PathSearch::isDone() const {

			return done;
		}

		std::vector<Tile const*> const PathSearch::getSolution() const {


			return {NULL};
		}
	}
}  // close namespace ufl_cap4053::searches
