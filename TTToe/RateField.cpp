#include "RateField.h"

RateField::RateField(){
	movesMade = 0;
	field = nullptr;
}

RateField::RateField(size_t size){
	field = nullptr;
	initialize(size);
}

int RateField::getMovesMade(){
	return movesMade;
}

int RateField::getPercent(){
	return this->percent;
}

bool RateField::getStopThinking(){
	return stopThinking;
}

void RateField::setStopThinking(bool stop){
	this->stopThinking = stop;
}

void RateField::setPercent(int perc){
	this->percent = perc;
}

unordered_map<Combination*, bool>* RateField::getCombinations(){
	return &combinations;
}

vector<unordered_map<Combination*, Combination*>*>* RateField::getPcCombinationsByLength(){
	return &pcCombinationsByLength;
}

vector<unordered_map<Combination*, Combination*>*>* RateField::getPlayerCombinationsByLength(){
	return &playerCombinationsByLength;
}

//Deletes the old field, if it was initialized
void RateField::initialize(size_t size){
	percent = 0;
	stopThinking = false;
	if(field != nullptr){
		for(size_t i = 0; i < this->size; i++){
			delete []field[i];
		}

		delete []field;

		field = new RateCell*[size];

		for(size_t i = 0; i < size; i++){
			field[i] = new RateCell[size];
		}

		for(size_t i = 0; i < size; i++){
			for(size_t j = 0; j < size; j++){
				field[i][j].setPoint(Point(j,i));
			}
		}
	}
	else{
		field = new RateCell*[size];

		for(size_t i = 0; i < size; i++){
			field[i] = new RateCell[size];
		}

		for(size_t i = 0; i < size; i++){
			for(size_t j = 0; j < size; j++){
				field[i][j].setPoint(Point(j,i));
			}
		}
	}

	movesMade = 0;

	this->size = size;

	clearCombinations();

	//               <= !!!

	for(int i = 0; i <= GlobalSettings::aiSettings.winCombLength; i++){
		pcCombinationsByLength.push_back(new unordered_map<Combination*, Combination*>());
		playerCombinationsByLength.push_back(new unordered_map<Combination*, Combination*>());
	}
}

// Clears the map and deletes objects;
void RateField::clearCombinations(){

	for(size_t i = 0; i < pcCombinationsByLength.size(); i++){
		delete pcCombinationsByLength[i];
	}

	for(size_t i = 0; i < playerCombinationsByLength.size(); i++){
		delete playerCombinationsByLength[i];
	}

	pcCombinationsByLength.clear();
	playerCombinationsByLength.clear();

	unordered_map<Combination*, bool>::iterator it = combinations.begin();

	while(it != combinations.end()){
		delete ((*it).first);
		++it;
	}

	combinations.clear();
}

RateField::~RateField(){
	if(field != nullptr){
		for(size_t i = 0; i < size; i++){
			delete []field[i];
		}

		delete []field;
	}

	clearCombinations();
}

RateCell** RateField::getField(){
	return this->field;
}

int RateField::getSize(){
	return this->size;
}

void RateField::reset(){
	if(movesMade != 0){
		initialize(this->size);
	}
}

/*void clearField(){
	clearCombinations();
}*/

//Deletes completed combinations from all containers(Except cells), unallocates the memory allocated for that combination object.

void RateField::deleteCompletedCombinations(vector<Combination*>* combsToRestore){
	unordered_map<Combination*, Combination*>::iterator it = pcCombinationsByLength[0]->begin();
	unordered_map<Combination*, Combination*>::iterator end = pcCombinationsByLength[0]->end();

	while(it != end){
		combinations.erase(it->first);
		if(combsToRestore != nullptr){
			combsToRestore->push_back(it->first);
		}
		else{
			delete it->first;
		}
		++it;
	}

	pcCombinationsByLength[0]->clear();

	it = playerCombinationsByLength[0]->begin();
	end = playerCombinationsByLength[0]->end();

	while(it != end){
		combinations.erase(it->first);
		if(combsToRestore != nullptr){
			combsToRestore->push_back(it->first);
		}
		else{
			delete it->first;
		}
		++it;
	}

	playerCombinationsByLength[0]->clear();
}

//Erases combination from all containers(Except cells), unallocates the memory allocated for that combination object.

void RateField::deleteCombinationByPointer(Combination* combPointer, vector<Combination*>* removedCombinations){
	unordered_map<Combination*, bool>::iterator it = combinations.find(combPointer); 

	if(it != combinations.end()){

		if(combPointer->mode == BaseCell::MODE_PC){ // evaluating whether the combination was pc's or players.
			pcCombinationsByLength[combPointer->combCells.size()]->erase(combPointer); // getting the length of the combination. getting corresponding map, deleting the entry. 
		}
		else{
			playerCombinationsByLength[combPointer->combCells.size()]->erase(combPointer); // getting the length of the combination. getting corresponding map, deleting the entry.
		}

		combinations.erase(combPointer); // deleting the combination from map.

		if(removedCombinations != nullptr){
			removedCombinations->push_back(combPointer);
		}
		else{
			delete combPointer; // unallocating the memory.
		}
	}
}

void RateField::restoreCombinations(vector<Combination*>* removedCombinations){
	int tsz = removedCombinations->size();
	Combination* comb;

	int combCellsSize;

	RateCell* cell;

	for(int i = 0; i < tsz; i++){
		comb = removedCombinations->at(i);

		combCellsSize = comb->combCells.size();

		if(comb->mode == BaseCell::MODE_PC){

			for(int j = 0; j < combCellsSize; j++){
				cell = (RateCell*)comb->combCells[j];
				cell->pcCombinationKeys[comb->direction]->push_back(comb);
				cell->resetRate();
			}

			pcCombinationsByLength[combCellsSize]->insert(unordered_map<Combination*,Combination*>::value_type(comb,comb));
		}
		else{
			for(int j = 0; j < combCellsSize; j++){
				cell = (RateCell*)comb->combCells[j];
				cell->playerCombinationKeys[comb->direction]->push_back(comb);
				cell->resetRate();
			}

			playerCombinationsByLength[combCellsSize]->insert(unordered_map<Combination*,Combination*>::value_type(comb,comb));
		}

		combinations.insert(unordered_map<Combination*,bool>::value_type(comb, true));
	}
}

void RateField::clearIntersectingLines(Point point, vector<Combination*>* removedCombinations, const vector<int> * dirs){

	RateCell* cell;

	list<Combination*>* combList;
	list<Combination*>::iterator combListIterator;
	list<Combination*>::iterator listEnd;

	Combination* key;

	size_t dirNum = (dirs == nullptr ? 4 : dirs->size());

	int directions[4];

	if(dirs == nullptr){
		directions[0] = RateCell::DIRECTION_HORIZONTAL;
		directions[1] = RateCell::DIRECTION_VERTICAL;
		directions[2] = RateCell::DIRECTION_MAIN_DIAG;
		directions[3] = RateCell::DIRECTION_SEC_DIAG;
	}
	else{
		for(size_t i = 0; i < dirs->size(); i++){
			directions[i] = dirs->at(i);
		}
	}

	for(size_t i = 0; i < dirNum; i++){
		lineIterator.initialize(size, directions[i], point);
		while(lineIterator.getNext()){
			cell = &field[lineIterator.currentPoint.y][lineIterator.currentPoint.x];
			cell->resetRate();

			combList = cell->playerCombinationKeys[directions[i]]; // Gettings the combination for current direction
			combListIterator = combList->begin();
			listEnd = combList->end();

			while(combListIterator != listEnd){
				key = (*combListIterator);
				deleteCombinationByPointer(key,removedCombinations);
				++combListIterator;
			}

			combList->clear();

			//for(int j = 0; j < vec->size(); j++){
			//	key = vec->at(j); // getting the key of the combination.
			//	
			//	deleteCombinationByPointer(key);
			//}
			//vec->clear();

			combList = cell->pcCombinationKeys[directions[i]]; // Gettings the combination for current direction
			combListIterator = combList->begin();
			listEnd = combList->end();

			while(combListIterator != listEnd){
				key = (*combListIterator);
				deleteCombinationByPointer(key,removedCombinations);
				++combListIterator;
			}

			combList->clear();

			//vec = &cell->pcCombinationKeys[directions[i]]; // Gettings the combination for current direction

			//for(int j = 0; j < vec->size(); j++){
			//	key = vec->at(j);
			//	
			//	deleteCombinationByPointer(key);
			//}
			//vec->clear();		
		}
	}

}

void RateField::recalculateIntersectingLines(Point point, vector<Combination*>* combsToRestore, const vector<int> * dirs){

	clearIntersectingLines(point, combsToRestore);
	deleteCompletedCombinations(combsToRestore);

	int dirNum = (dirs == nullptr ? 4 : dirs->size());

	int directions[4];

	if(dirs == nullptr){
		directions[0] = RateCell::DIRECTION_HORIZONTAL;
		directions[1] = RateCell::DIRECTION_VERTICAL;
		directions[2] = RateCell::DIRECTION_MAIN_DIAG;
		directions[3] = RateCell::DIRECTION_SEC_DIAG;
	}
	else{
		for(size_t i = 0; i < dirs->size(); i++){
			directions[i] = dirs->at(i);
		}
	}

	int cellModes[2];

	cellModes[0] = BaseCell::MODE_PC;
	cellModes[1] = BaseCell::MODE_PLAYER;
		
	Point pt;
	int index;
	int mode;
	RateCell* cell;

	Combination* comb = nullptr;

	int winCombLenghtMinusOne = GlobalSettings::aiSettings.winCombLength - 1;
	bool added = false;

	for(int i = 0; i < dirNum; i++){
		for(int j = 0; j < 2; j++){ // For pc and for player
			lineIterator.initialize(size, directions[i], point);

			mode = cellModes[j];

			index = 0;

			while(lineIterator.getNext()){
				pt = lineIterator.currentPoint;
				cell = &field[pt.y][pt.x];

				if(cell->mode == mode){ // If the cell has the current mode
					if(comb == nullptr){
						comb = new Combination(mode, directions[i]);
						comb->relativeStart = index;
						comb->start = pt;
					}
					else{
						if(index - comb->relativeStart == winCombLenghtMinusOne){
							comb->relativeEnd = index;
							comb->end = pt;

							added = addCombination(comb);

							lineIterator.currentPoint = comb->start;
							index = comb->relativeStart;

							if(!added){
								delete comb;
							}

							comb = nullptr;
						}

					}
				}
				else if(cell->mode == BaseCell::MODE_EMPTY){ // If the cell is empty
					if(comb == nullptr){
						comb = new Combination(mode, directions[i]);
						comb->relativeStart = index;
						comb->start = pt;

						comb->combCells.push_back(cell);
					}
					else{
						comb->combCells.push_back(cell);

						if(index - comb->relativeStart == winCombLenghtMinusOne){
							comb->relativeEnd = index;
							comb->end = pt;

							added = addCombination(comb);

							lineIterator.currentPoint = comb->start;
							index = comb->relativeStart;
							
							if(!added){
								delete comb;
							}

							comb = nullptr;
						}


					}
				}
				else{ //if the cell has opposite mode
					if(comb != nullptr){
						delete comb;
						comb = nullptr;
					}			
				}

				++index;
			}

			if(comb!=nullptr){
				delete comb;
				comb = nullptr;
			}
		}

	}
}

void RateField::recalculateField(vector<Combination*>* combsToRestore){
	FieldLineIterator iter(size, FieldLineIterator::DIRECTION_MAIN_DIAG, Point(0,0));

	vector<int> dirs1;
	dirs1.push_back(FieldLineIterator::DIRECTION_HORIZONTAL);
	dirs1.push_back(FieldLineIterator::DIRECTION_VERTICAL);
	dirs1.push_back(FieldLineIterator::DIRECTION_SEC_DIAG);
		
	vector<int> dirs2;
	dirs2.push_back(FieldLineIterator::DIRECTION_MAIN_DIAG);

	while(iter.getNext()){
		recalculateIntersectingLines(iter.currentPoint, combsToRestore, &dirs1);
	}

	iter.initialize(size, FieldLineIterator::DIRECTION_SEC_DIAG, Point(0, size -1));

	while(iter.getNext()){
		recalculateIntersectingLines(iter.currentPoint, combsToRestore, &dirs2);
	}

}

//Checks if the current combinations is conatained is another combination, and if 
//it is not, adds the combaintion and removes all containing combinations;
//Left-to-right required.(!)
//Returns whether the combination was added;

bool RateField::addCombination(Combination* comb){

	bool isCombModePC = comb->mode == BaseCell::MODE_PC;

	vector<BaseCell*>* combCells = &comb->combCells;
	list<Combination*>* firstCellCombs;

	//Checking whether this combination contains another combination

	if(combCells->size() > 0){
		if(isCombModePC){
			firstCellCombs = ((RateCell*)combCells->at(0))->pcCombinationKeys[comb->direction];
		}
		else{
			firstCellCombs = ((RateCell*)combCells->at(0))->playerCombinationKeys[comb->direction];
		}

		combListIteratorAC = firstCellCombs->begin();
		listEndAC = firstCellCombs->end();

		while(combListIteratorAC != listEndAC){
			if(comb->contains(*combListIteratorAC)){
				return false;	
			}
			++combListIteratorAC;
		}

		//for(i = 0; i < firstCellCombs->size(); i++ ){
		//	if(comb->contains(firstCellCombs->at(i))){
		//		return false;
		//	}
		//}

		Combination* tempComb;
		RateCell* tempCell;
		list<Combination*>* tempCombinations;

		size_t j;

		//Removing containing combinations

		combListIteratorAC = firstCellCombs->begin();
		listEndAC = firstCellCombs->end();

		while(combListIteratorAC != listEndAC){
			tempComb = *(combListIteratorAC);

			if(tempComb->contains(comb)){
				combListIteratorAC = firstCellCombs->erase(combListIteratorAC);
					
				for(j = 0; j < tempComb->combCells.size(); j++){
					tempCell = (RateCell*)tempComb->combCells[j];

					if(isCombModePC){
						tempCombinations = tempCell->pcCombinationKeys[comb->direction];
					}
					else{
						tempCombinations = tempCell->playerCombinationKeys[comb->direction];
					}

					combListIterator2AC = tempCombinations->begin();
					listEnd2AC = tempCombinations->end();

					while(combListIterator2AC != listEnd2AC){
						if((*combListIterator2AC) == tempComb){
							tempCombinations->erase(combListIterator2AC);
							break;
						}
						else{
							++combListIterator2AC;
						}
					}

				}
				deleteCombinationByPointer(tempComb);
			}
			else{
				++combListIteratorAC;
			}
			
		}

		//for(i = firstCellCombs->size() - 1; i >= 0; i-- ){
		//	tempComb = firstCellCombs->at(i);
		//	if(tempComb->contains(comb)){
		//		for(j = 0; j < tempComb->combCells.size(); j++){
		//			tempCell = (RateCell*)tempComb->combCells[j];

		//			if(isCombModePC){
		//				tempCombinations = &tempCell->pcCombinationKeys[comb->direction];
		//			}
		//			else{
		//				tempCombinations = &tempCell->playerCombinationKeys[comb->direction];
		//			}

		//			for(k = tempCombinations->size() - 1; k >= 0; k++){
		//				if( (tempCombinations->at(k)) == tempComb ){
		//					tempCombinations->erase(tempCombinations->begin()+k);
		//					break;
		//				}
		//			}

		//		}

		//		deleteCombinationByPointer(tempComb);
		//	}
		//}
		
		for(size_t i = 0; i < combCells->size(); i++){
			if(isCombModePC){
				tempCombinations = ( (RateCell*) combCells->at(i))->pcCombinationKeys[comb->direction];
			}
			else{
				tempCombinations = ( (RateCell*) combCells->at(i))->playerCombinationKeys[comb->direction];
			}

			tempCombinations->push_back(comb);
		}

	}

	combinations.insert(unordered_map<Combination*,bool>::value_type(comb,true));
	unordered_map<Combination*,Combination*>* map = isCombModePC ? pcCombinationsByLength[comb->combCells.size()] : playerCombinationsByLength[comb->combCells.size()];
	map->insert(unordered_map<Combination*,Combination*>::value_type(comb,comb));

	return true;
}

Point RateField::getBestCellByRate(){
	double max = 0.0;
	Point point;
	bool first = true;

	for(size_t i = 0; i < size; i++){
		for(size_t j = 0; j < size; j++){
			if(field[i][j].mode == BaseCell::MODE_EMPTY){
				double rate = field[i][j].getRate();

				if(first || rate > max || (max-rate) < GlobalSettings::aiSettings.randDif && rand() % 3 == 0){
					max = rate;
					point.x = j;
					point.y = i;
					first = false;
				}
			}
		}
	}
	
	return point;
}

Point RateField::evaluateAIMove(){
	if(GlobalSettings::aiSettings.level.depth == 1 || movesMade<= 1){
		return getBestCellByRate();
	}
	else{
		return getBestCellByAnalysingTheTree();
	}
}



Point RateField::getBestCellByAnalysingTheTree(){
	vector<CellRate> topCells;
	size_t baseSelNum = GlobalSettings::aiSettings.level.baseSelNum;
	topCells.reserve(baseSelNum);

	double min;
	int minIndex = -1;

	for(size_t i = 0; i < size; i++){
		for(size_t j = 0; j < size; j++){
			if(field[i][j].mode == BaseCell::MODE_EMPTY){
				double rate = field[i][j].getRate();

				if(topCells.size() < baseSelNum){
					topCells.push_back(CellRate(Point(j,i), rate));

					if(topCells.size() == baseSelNum){
						for(size_t k = 0; k < baseSelNum; k++){
							if(minIndex == -1 || topCells[k].rate < min){
								min = topCells[k].rate;
								minIndex = k;
							}
						}
					}
				}
				else{
					if(rate > min){
						topCells[minIndex].point.x = j;
						topCells[minIndex].point.y = i;
						topCells[minIndex].rate = rate;

						min = -1.0;
						minIndex = -1;

						for(size_t k = 0; k < baseSelNum; k++){
							if(minIndex == -1 || topCells[k].rate < min){
								min = topCells[k].rate;
								minIndex = k;
							}
						}
					}
					else if((min-rate) < GlobalSettings::aiSettings.randDif && rand() % 3 == 0){
						topCells[minIndex].point.x = j;
						topCells[minIndex].point.y = i;
						topCells[minIndex].rate = rate;

						min = rate;
					}
				}
			}
		}
	}

	Point res;
	PositionRate posRate;
	posRate.valid = false;

	bool first = true;
	PositionRate temp;
	Point tempPoint;

	vector<Combination*> combsToRestore;

	bool winningCellsExist = pcCombinationsByLength[1]->size() != 0 || playerCombinationsByLength[1]->size() != 0;

	RateCell* tempCell;

	int topCellsSize = topCells.size();

	for(int i = 0; i < topCellsSize; i++){
		if(stopThinking){
			res.valid = false;
			break;
		}
		tempPoint = topCells[i].point;
		tempCell = &field[tempPoint.y][tempPoint.x];
		tempCell->getRate(); // to init isWinning property;


		this->percent = 100*(i+1)/topCellsSize;

		if(!winningCellsExist || tempCell->isWinning){

 			tempCell->mode = BaseCell::MODE_PC;
			recalculateIntersectingLines(tempPoint, &combsToRestore);

			temp = analyse(1,BaseCell::MODE_PLAYER, posRate);

			tempCell->mode = BaseCell::MODE_EMPTY;
			
			clearIntersectingLines(tempPoint);
			deleteCompletedCombinations();
			restoreCombinations(&combsToRestore);
			combsToRestore.clear();

 			if(!temp.pruned && (first || posRate < temp)){
				posRate = temp;
				posRate.valid = true;
				res = tempPoint;
				
				first = false;
			}
		}
	}

	return res;
}

PositionRate RateField::analyse(int depth, int mode, PositionRate minmax){
	bool modePC = mode == BaseCell::MODE_PC;

	size_t selNum = GlobalSettings::aiSettings.level.selNum;

	PositionRate rate;
	
	if(stopThinking){
		rate.valid = false;
		rate.pruned = true;
		return rate;
	}

	if(modePC){
		if(playerCombinationsByLength[0]->size() != 0){
			rate.valid = true;
			rate.victoryMode = PositionRate::MODE_PLAYER_WON;
			rate.victoryDepht = depth; 

			return rate;
		}

		if(pcCombinationsByLength[1]->size() != 0){
			rate.valid = true;
			rate.victoryMode = PositionRate::MODE_PC_WON;
			rate.victoryDepht = depth+1; 

			return rate;
		}
	}
	else{
		if(pcCombinationsByLength[0]->size() != 0){
			rate.valid = true;
			rate.victoryMode = PositionRate::MODE_PC_WON;
			rate.victoryDepht = depth;

			return rate;
		}

		if(playerCombinationsByLength[1]->size() != 0){
			rate.valid = true;
			rate.victoryMode = PositionRate::MODE_PLAYER_WON;
			rate.victoryDepht = depth+1;

			return rate;
		}
	}

	if(depth == GlobalSettings::aiSettings.level.depth){

		rate.rate = 0.0;

		for(int i = 1; i < GlobalSettings::aiSettings.winCombLength; i++){
			int size = pcCombinationsByLength[i]->size() - playerCombinationsByLength[i]->size();
			rate.rate += size*GlobalSettings::aiSettings.coeficients[i];
		}

		return rate;
	}

	vector<CellRate> topCells;
	topCells.reserve(selNum);

	double min;
	int minIndex = -1;
	CellRate* minCellRef;

	for(size_t i = 0; i < size; i++){
		for(size_t j = 0; j < size; j++){
			if(field[i][j].mode == BaseCell::MODE_EMPTY){
				double rate = field[i][j].getRate();

				if(topCells.size() < selNum){
					topCells.push_back(CellRate(Point(j,i), rate));

					if(topCells.size() == selNum){
						for(size_t k = 0; k < selNum; k++){
							if(minIndex == -1 || topCells[k].rate < min){
								min = topCells[k].rate;
								minIndex = k;
							}
						}
					}
				}
				else{
					minCellRef = &topCells[minIndex];
					if(rate > min){
						minCellRef->point.x = j;
						minCellRef->point.y = i;
						minCellRef->rate = rate;

						min = -1.0;
						minIndex = -1;

						for(size_t k = 0; k < selNum; k++){
							if(minIndex == -1 || topCells[k].rate < min){
								min = topCells[k].rate;
								minIndex = k;
							}
						}
					}
					else if((min-rate) < GlobalSettings::aiSettings.randDif && rand() % 3 == 0){
						minCellRef->point.x = j;
						minCellRef->point.y = i;
						minCellRef->rate = rate;

						min = rate;
					}
				}
			}
		}
	}

	rate.valid = false;

	bool first = true;
	PositionRate temp;
	Point tempPoint;

	vector<Combination*> combsToRestore;
	bool winningCellsExist = pcCombinationsByLength[1]->size() != 0 || playerCombinationsByLength[1]->size() != 0;

	RateCell* tempCell;

	if(modePC){

		for(size_t i = 0; i < topCells.size(); i++){
			if(stopThinking){
				rate.valid = false;
				rate.pruned = true;
				break;
			}

			tempPoint = topCells[i].point;
			tempCell = &field[tempPoint.y][tempPoint.x];
			tempCell->getRate(); // to init isWinning property;
			if(!winningCellsExist || tempCell->isWinning){

				tempCell->mode = BaseCell::MODE_PC;
				recalculateIntersectingLines(tempPoint, &combsToRestore);

				temp = analyse(depth + 1,BaseCell::MODE_PLAYER, rate);

				tempCell->mode = BaseCell::MODE_EMPTY;
		     	clearIntersectingLines(tempPoint);	
				deleteCompletedCombinations();
				restoreCombinations(&combsToRestore);
				combsToRestore.clear();

//				recalculateIntersectingLines(tempPoint);

				if(!temp.pruned){
					if(minmax.valid){
						if(minmax < temp){
							rate.pruned = true;
							return rate;
						}
					}

					if(first || rate < temp){
						rate = temp;
						rate.valid = true;
						
						first = false;
					}
				}
			}
		}
	}
	else{
		for(size_t i = 0; i < topCells.size(); i++){
			if(stopThinking){
				rate.valid = false;
				rate.pruned = true;
				break;
			}
			tempPoint = topCells[i].point;
			tempCell = &field[tempPoint.y][tempPoint.x];
			tempCell->getRate(); // to init isWinning property;

			if(!winningCellsExist || tempCell->isWinning){

				tempCell->mode = BaseCell::MODE_PLAYER;
				recalculateIntersectingLines(tempPoint, &combsToRestore);

				temp = analyse(depth + 1,BaseCell::MODE_PC, rate);

				tempCell->mode = BaseCell::MODE_EMPTY;
        		clearIntersectingLines(tempPoint);
				deleteCompletedCombinations();	
				restoreCombinations(&combsToRestore);
				combsToRestore.clear();
//				recalculateIntersectingLines(tempPoint);

				if(!temp.pruned){
					if(minmax.valid){
						if(temp < minmax){
							rate.pruned = true;
							return rate;
						}
					}

					if(first || temp < rate){
						rate = temp;
						rate.valid = true;

						first = false;
					}
				}
			}
		}
	}


	return rate;
}

bool RateField::checkIfCompletedCombinationExist(){
	return pcCombinationsByLength[0]->size() > 0 || playerCombinationsByLength[0]->size() > 0; 
}
	
vector<Combination> RateField::getExtendedCompletedCombinations(){
	bool directions[4];

	directions[0] = directions[1] = directions[2] = directions[3] = false;

	int dirs[4] = {RateCell::DIRECTION_HORIZONTAL, RateCell::DIRECTION_VERTICAL, RateCell::DIRECTION_MAIN_DIAG, RateCell::DIRECTION_SEC_DIAG};

	vector<Combination> combs;

		

	unordered_map<Combination*, Combination*>::iterator iterator = pcCombinationsByLength[0]->size() > 0 ? pcCombinationsByLength[0]->begin() : playerCombinationsByLength[0]->begin();
	unordered_map<Combination*, Combination*>::iterator itEnd = pcCombinationsByLength[0]->size() > 0 ? pcCombinationsByLength[0]->end() : playerCombinationsByLength[0]->end();

	while(iterator != itEnd){
		Combination* comb = iterator->second;

		int dirInd = -1;

		for(int i = 0; i < 4; i++){
			if(comb->direction == dirs[i]){
				dirInd = i;
			}
		}

		if(directions[dirInd] == false){
			Combination combination(comb->mode, comb->direction);
			combination.start = comb->start;
			combination.end = comb->end;

			FieldLineIterator it(this->size, comb->direction, comb->start, true);

			while(it.move(-1)){
				if(field[it.currentPoint.y][it.currentPoint.x].mode == comb->mode){
					combination.start = it.currentPoint;
				}
				else{
					break;
				}
			}

			it.currentPoint = comb->end;

			while(it.move(1)){
				if(field[it.currentPoint.y][it.currentPoint.x].mode == comb->mode){
					combination.end = it.currentPoint;
				}
				else{
					break;
				}
			}

			combs.push_back(combination);

			directions[dirInd] = true;
		}

		++iterator;
	}


	return combs;
}

bool RateField::makeMove(Point pt, int mode, bool recInterLines){
	if(field[pt.y][pt.x].mode == BaseCell::MODE_EMPTY){

		this->field[pt.y][pt.x].setMode(mode);
		if(recInterLines){
			recalculateIntersectingLines(pt);
		}

		movesMade++;

		return true;
	}

	return false;
}

bool RateField::checkIfDrawExists(){
	return movesMade == size*size;
}

CMainFrame* RateField::getMainFrame(){
	return (CMainFrame*)AfxGetApp()->GetMainWnd();
}