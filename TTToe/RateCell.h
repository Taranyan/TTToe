#ifndef RATECELL_H
#define RATECELL_H

#include <vector>
#include <list>
#include "BaseCell.h"
#include "Primitives.h"
#include "GlobalSettings.h"
using std::vector;
using std::list;

class Combination{
public:
	Point start;
	Point end;
	int mode;
	int direction;
	int relativeStart;
	int relativeEnd;
	vector<BaseCell*> combCells; // Note: these cells are added in certain direction.

	Combination(){
		combCells.reserve(5);
	}

	~Combination(){
	}

	Combination(int mode, int direction){
		this->mode = mode;
		this->direction = direction;
		combCells.reserve(5);
	}

	//Checks whether the passed combination is within the current combination
	//Should be only called for the combinations on the same line!

	bool contains(Combination* comb){
		size_t l = 0;
		size_t i,j;
		BaseCell* elem;
		for(i = 0; i < comb->combCells.size(); i++){
			elem = comb->combCells[i];
			for(j = l; j < this->combCells.size(); j++){
				if(this->combCells[j] == elem){
					l = j + 1;
					break;
				}
			}

			if(j == this->combCells.size()){
				return false;
			}
		}

		return true;
	}
};



class RateCell : public BaseCell{
private:
	double rate;
	int* hits;
	int* coefCoef;

	list<Combination*>::iterator listIterator;
	list<Combination*>::iterator listEnd;
public:
	static const int DIRECTION_HORIZONTAL = 1;
	static const int DIRECTION_VERTICAL = 2;
	static const int DIRECTION_MAIN_DIAG = 3;
	static const int DIRECTION_SEC_DIAG = 4;

	bool isWinning;
	int directions[4];
	vector<list<Combination*>*> playerCombinationKeys;
	vector<list<Combination*>*> pcCombinationKeys;
	int mode;
	Point point;

	RateCell();
	RateCell(int mode);
	~RateCell();
	void setPoint(Point p);

	void initializeCombinations();
	void resetMode();
	void setMode(int mode);
	void setRate(double rate);
	void resetRate();
	double getRate();
};

#endif