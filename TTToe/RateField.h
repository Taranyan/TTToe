#ifndef RATEFIELD_H
#define RATEFIELD_H

#include "stdafx.h"
#include "RateCell.h"
#include "FieldLineIterator.h"
#include <boost/unordered_map.hpp>
#include "GlobalSettings.h"
#include "Utils.h"
#include "BaseField.h"
#include "MainFrm.h"

class PositionRate : public Object{
public:
	static const int MODE_PC_WON = 1;
	static const int MODE_PLAYER_WON = -1;
	static const int MODE_NO_VICTORY = 0;

	bool valid;
	int victoryMode; 
	int victoryDepht;
	double rate;
	bool pruned;

	PositionRate(){
		pruned = false;
		valid = true;
		victoryMode = MODE_NO_VICTORY;
		victoryDepht = -1;
	}

	PositionRate(const PositionRate& rt){
		this->operator=(rt);
	}

	const PositionRate& operator=(const PositionRate& rt){
		this->valid = rt.valid;
		this->victoryMode = rt.victoryMode;
		this->victoryDepht = rt.victoryDepht;
		this->rate = rt.rate;
		this->pruned = rt.pruned;

		return *this;
	}

	bool operator < (PositionRate r){
		if(r.victoryMode == this->victoryMode){
			if(this->victoryMode == MODE_NO_VICTORY){
				return this->rate < r.rate;
			}
			else{
				return this->victoryMode == MODE_PC_WON ? this->victoryDepht > r.victoryDepht : this->victoryDepht < r.victoryDepht;
			}
		}
		else{
			return this->victoryMode < r.victoryMode;
		}
	}

	bool operator > (PositionRate r){
		return r.operator<(*this);
	}
};

class CellRate{
public:
	Point point;
	double rate;

	CellRate(){
	
	}

	CellRate(Point point, double rate){
		this->point = point;
		this->rate = rate;
	}
};


class RateField : public Object{
private:
	RateCell** field;
	size_t size;
	int percent;
	bool stopThinking;
	FieldLineIterator lineIterator;
	unordered_map<Combination*, bool> combinations;
	vector<unordered_map<Combination*, Combination*>*> pcCombinationsByLength;
	vector<unordered_map<Combination*, Combination*>*> playerCombinationsByLength;
	list<Combination*>::iterator combListIteratorAC;
	list<Combination*>::iterator combListIterator2AC;
	list<Combination*>::iterator listEndAC;
	list<Combination*>::iterator listEnd2AC;

public:
	static const int ST_DRAW = 3;
	static const int ST_PLAYER_WON = 1;
	static const int ST_AI_WON = 2;
	static const int ST_NOTHING = -1;

	int movesMade;
	BaseField* fld; 

	RateField();
	RateField(size_t size);
	~RateField();
	int getMovesMade();
	int getPercent();
	void setPercent(int perc);
	bool getStopThinking();
	void setStopThinking(bool stop);
	unordered_map<Combination*, bool>* getCombinations();
	vector<unordered_map<Combination*, Combination*>*>* getPcCombinationsByLength();
	vector<unordered_map<Combination*, Combination*>*>* getPlayerCombinationsByLength();
	void initialize(size_t size); //Deletes the old field, if it was initialized
	void clearCombinations(); // Clears the map and deletes objects;
	RateCell** getField();	
	int getSize();
	void reset();
	void deleteCompletedCombinations(vector<Combination*>* combsToRestore = nullptr); 	//Deletes completed combinations from all containers(Except cells), unallocates the memory allocated for that combination object.
	void deleteCombinationByPointer(Combination* combPointer, vector<Combination*>* removedCombinations = nullptr); 	//Erases combination from all containers(Except cells), unallocates the memory allocated for that combination object.
	void restoreCombinations(vector<Combination*>* removedCombinations);
	void clearIntersectingLines(Point point, vector<Combination*>* removedCombinations = nullptr, const vector<int> * dirs = nullptr);
	void recalculateIntersectingLines(Point point, vector<Combination*>* combsToRestore = nullptr, const vector<int> * dirs = nullptr); // Recalculate intersecting lines. If dirs(directions) is null, than all combinations on all directions are being recalculated.
	void recalculateField(vector<Combination*>* combsToRestore = nullptr); // Recalculates the whole field

	//Checks if the current combinations is conatained is another combination, and if 
	//it is not, adds the combination and removes all containing combinations;
	//Left-to-right required.(!)
	//Returns whether the combination was added;

	bool addCombination(Combination* comb);
	Point getBestCellByRate();
	Point evaluateAIMove();
	Point getBestCellByAnalysingTheTree();
	PositionRate analyse(int depth, int mode, PositionRate minmax);
	bool checkIfCompletedCombinationExist();
	vector<Combination> getExtendedCompletedCombinations();
	bool makeMove(Point pt, int mode, bool recInterLines = true);
	bool checkIfDrawExists();
	CMainFrame* getMainFrame();

/*	void clearMode(int y, int x, bool recInterLines = true){
		setMode(y, x, BaseCell::MODE_EMPTY, true);
	}
*/
};


#endif