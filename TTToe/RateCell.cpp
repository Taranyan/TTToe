#include "stdafx.h"
#include "RateCell.h"

RateCell::RateCell(){
	rate = - 1.0;
	isWinning = false;
	mode = this->MODE_EMPTY;
	point.valid = false;
	initializeCombinations();
	hits = new int[GlobalSettings::aiSettings.winCombLength+1];
	coefCoef = new int[GlobalSettings::aiSettings.winCombLength+1];

	directions[0] = DIRECTION_HORIZONTAL;
	directions[1] = DIRECTION_VERTICAL;
	directions[2] = DIRECTION_MAIN_DIAG;
	directions[3] = DIRECTION_SEC_DIAG;
}

RateCell::RateCell(int mode){
	rate = -1.0;
	isWinning = false;
	this->mode = mode;
	initializeCombinations();
	hits = new int[GlobalSettings::aiSettings.winCombLength+1];
	coefCoef = new int[GlobalSettings::aiSettings.winCombLength+1];
}

RateCell::~RateCell(){
	delete hits;
	delete coefCoef;

	for(int i = 0; i < 5; i++){
		delete playerCombinationKeys[i];
		delete pcCombinationKeys[i];
	}
}

void RateCell::setPoint(Point p){
	this->point = p;
}

void RateCell::initializeCombinations(){
	for(int i = 0; i < 5; i++){
		playerCombinationKeys.push_back(new list<Combination*>());
		pcCombinationKeys.push_back(new list<Combination*>());
	}
}	

void RateCell::resetMode(){
	mode = this->MODE_EMPTY;
}

void RateCell::setMode(int mode){
	this->mode = mode;
	rate = -1.0;
}

void RateCell::setRate(double rate){
	this->rate = rate;
}

void RateCell::resetRate(){
	this->rate = -1.0;
	isWinning = false;
}

double RateCell::getRate(){
	list<Combination*>* combs;
	Combination* comb;

	int length;
	int i;

	if(this->rate < 0.0){	
		rate = 0.0;

		for(i = 1; i <= GlobalSettings::aiSettings.winCombLength; i++){
			hits[i] = 0;
			coefCoef[i] = 0;
		}

		for(i = 0; i < 4; i++){
			combs = pcCombinationKeys[directions[i]];
			listIterator = combs->begin();
			listEnd = combs->end();

			while(listIterator != listEnd){
				comb = (*listIterator);
				length = comb->combCells.size();
				if(coefCoef[length] == 0){
					coefCoef[length] = i + 1;
				}
				else if(coefCoef[length] > 0 && coefCoef[length] != i + 1){
					coefCoef[length] = -1;
				}

				if(!isWinning && length == 1){
					isWinning = true;
				}

				++hits[length];
				++listIterator;
			}

		}	

		for(i = 1; i <= GlobalSettings::aiSettings.winCombLength; i++){
			rate += coefCoef[i] < 0 ? hits[i] * (i + 1) * GlobalSettings::aiSettings.coeficients[i] : hits[i] * GlobalSettings::aiSettings.coeficients[i];
			hits[i] = 0;
			coefCoef[i] = 0;
		}

		for(i = 0; i < 4; i++){
			combs = playerCombinationKeys[directions[i]];
			listIterator = combs->begin();
			listEnd = combs->end();

			while(listIterator != listEnd){
				comb = (*listIterator);
				length = comb->combCells.size();
				if(coefCoef[length] == 0){
					coefCoef[length] = i + 1;
				}
				else if(coefCoef[length] > 0 && coefCoef[length] != i + 1){
					coefCoef[length] = -1;
				}

					
				if(!isWinning && length == 1){
					isWinning = true;
				}

				++hits[length];
				++listIterator;
			}

		}	

		for(i = 1; i <= GlobalSettings::aiSettings.winCombLength; i++){
			rate += coefCoef[i] < 0 ? hits[i] * (i + 1) * GlobalSettings::aiSettings.coeficients[i] : hits[i] * GlobalSettings::aiSettings.coeficients[i];
		}
	}

	return rate;
}