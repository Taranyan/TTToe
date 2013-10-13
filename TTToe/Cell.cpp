#include "stdafx.h"
#include "Cell.h"

Cell::Cell(){
	upperLeft.x = -1;
	upperLeft.y = -1;
	upperLeft.valid = false;
	//numOfSubCells = -1;
    isStatic = true;
	needsToBeRedrawn = false;
	stIncr = false;
	subCells = nullptr;
	drawSet = nullptr;
	settings = nullptr;
	oldNum = 0;

	initialized = false;
}

Cell::~Cell(){
	int i;
	if(subCells != nullptr){
		for(i=0;i<oldNum;i++) {
			delete []subCells[i];
		}
		delete []subCells;
	}
}

void Cell::initialize(Settings* sett, Point upperLeft, DrawSets* drawSet, int mode){
	this->settings = sett;
	this->drawSet = drawSet;
	this->upperLeft = upperLeft;
	this->mode = mode;
	initialize();
}


void Cell::initialize(){ // This function performs the calculations, when all properties are already initialized
	initializeSubCells(true, true);
	initializeCenters();
	oldNum = settings->numOfSubCells;

	initialized = true;
}

void Cell::setFirstSubCell(Point point){
	this->firstSubCell = point;
}

Point Cell::getFirstSubCell(){
	return firstSubCell;
}

void Cell::setSettings(Settings* settings){
	this->settings = settings;
}

Settings* Cell::getSettings(){
	return this->settings;
}

void Cell::setDrawSet(DrawSets* drawSet){
	this->drawSet = drawSet;
}

DrawSets* Cell::getDrawSet(){
	return drawSet;
}

Point Cell::getUpperLeft(){
	return upperLeft;
}

Point Cell::getBottomRight(){
	Point bottomRight;			
	int size = settings->subCellSizes[settings->posnum - 1];
	bottomRight.x = getUpperLeftSubCenter().x + (size*2 + settings->distSub) * (settings->numOfSubCells -1) + size;
	bottomRight.y = getUpperLeftSubCenter().y + (size*2 + settings->distSub) * (settings->numOfSubCells -1) + size;
		
	if(settings->convex){
		bottomRight.x++;
		bottomRight.y++;
	}
	return bottomRight;
}

void Cell::setStIncr(bool st){ // Set if statuses of subcells are incrementing
	this->stIncr = st;
}

bool Cell::getStIncr(){ //get if statuses of subcells are incrementing
	return this->stIncr;
}

void Cell::setIsStatic(bool stat){
	this->isStatic = stat;
}

bool Cell::getIsStatic(){
	return this->isStatic;
}

void Cell::setNeedsToBeRedrawn(bool n){
	this->needsToBeRedrawn = n;
}

bool Cell::getNeedsToBeRedrawn(){
	return this->needsToBeRedrawn;
}

void Cell::setMode(int mode){
	this->mode = mode;
}

int Cell::getMode(){
	return this->mode;
}

bool Cell::setUpperLeft(Point pnt, bool initSubCells){
	this->upperLeft = pnt;

	if(initSubCells){
		if(initializeCenters() == false){
			return false;
		}
	}

	return true;
}

Point Cell::getUpperLeftSubCenter(){
	Point t = this->upperLeft;
	t.shift(settings->subCellSizes[settings->posnum -1],settings->subCellSizes[settings->posnum -1]);
	return t;
}

bool Cell::initializeCenters(){
	if(upperLeft.valid == true && settings->numOfSubCells != -1 && subCells != nullptr && settings != nullptr){
		int i, j;
		int incr = settings->subCellSizes[settings->posnum -1]*2 + settings->distSub;

		if(settings->convex){
			incr++;
		}

		Point t = getUpperLeftSubCenter();

		int baseX = t.x;
		for(i = 0; i < settings->numOfSubCells; i++){
			for(j = 0; j < settings->numOfSubCells; j++){
				subCells[i][j].setCenter(t);
				t.x += incr;
			}
			t.y += incr;
			t.x = baseX;
		}
		return true;
	}
	return false;
}

bool Cell::initializeSubCells(bool initProps, bool initSubs){
	int i,j;
	//int size = settings->numOfSubCells;
	if(initSubs && oldNum != settings->numOfSubCells){
		if(subCells != nullptr){
			for(i = 0; i < oldNum; i++){
				delete []subCells[i];
			}
			delete []subCells;
		}

		//size = newNum;
		subCells = new SubCell*[settings->numOfSubCells];
		for(i = 0; i < settings->numOfSubCells; i++){
			subCells[i] = new SubCell[settings->numOfSubCells];
		}
	}

	if(initProps){
		if(settings != nullptr){
			for(i = 0; i < settings->numOfSubCells; i++){
				for(j = 0; j < settings->numOfSubCells; j++){
//						subCells[i][j].setCDC(clientDC);
					subCells[i][j].setDrawSet(drawSet);
					subCells[i][j].setSettings(settings);
				}
			}
		}
		return false;
	}

	return true;
}


bool Cell::initializeProps(){
	return initializeSubCells(true, false) && initializeCenters();// && calculateBounds();
}


bool Cell::update(int i, int j)
{
    bool a = false;
	subCells[i][j].setIsPassed(true);
	SubCell* sub = &subCells[i][j];
	int subSt = sub->getStatus();
	if( subSt != settings->posnum-1) {
		subCells[i][j].setIsUpdated(true);
		a=true;
		sub->setStatus(subSt + 1);
		++subSt;
	}
	if(subSt!=1) { // it isn't the first hit of this subcell - update adjacent subcells.
	    if(i!=0) {
		if(j != 0 && subCells[i-1][j-1].getIsPassed()==false){
	        if(update(i-1,j-1))
			    a=true;
		}
	    if(j != settings->numOfSubCells-1 && subCells[i-1][j+1].getIsPassed()==false){
	        if(update(i-1,j+1))
			    a=true;
		}
		if(subCells[i-1][j].getIsPassed()==false){
	        if(update(i-1,j))
	            a=true;
		}
		}
		if(i != settings->numOfSubCells - 1) {
	    if(j != 0 && subCells[i+1][j-1].getIsPassed()==false) {
		    if(update(i+1,j-1))
			    a=true;		 
		}
	    if(j != settings->numOfSubCells-1 && subCells[i+1][j+1].getIsPassed()==false)	{
	        if(update(i+1,j+1))
			    a=true;
		}
		if(subCells[i+1][j].getIsPassed()==false) {
	        if(update(i+1,j))
	            a=true;
		}
		}
	    if(j != 0 && subCells[i][j-1].getIsPassed()==false) {
	    if(update(i,j-1))
			a=true;
		}
	    if(j != settings->numOfSubCells-1 && subCells[i][j+1].getIsPassed()==false) {
	    if(update(i,j+1))
			a=true;
		}
	}
	return a;
}

bool Cell::updateCell() {
    bool up=false;
	int i,j;
	if( stIncr ==false) { // statuses are decreasing
		for(i=0; i<settings->numOfSubCells; i++){
			for(j=0; j<settings->numOfSubCells; j++){

				int subSt = subCells[i][j].getStatus();

				if(subSt!=0) {
					up=true;
					subCells[i][j].setIsUpdated(true);
					/* if(subSt==4)   
						subCells[i][j].setStatus(2);
					else
						subCells[i][j].setStatus(subSt-1);*/
					if(settings->showtrack){
						subCells[i][j].setStatus(subSt - 1);
					}
					else{
						subCells[i][j].setStatus(0);
					}
				}		       		
			}
		}
	}
	else{
		for(i=0; i<settings->numOfSubCells; i++)
			for(j=0; j<settings->numOfSubCells; j++)
			{
				subCells[i][j].setIsUpdated(false);
				subCells[i][j].setIsPassed(false);
			}

			up=update(firstSubCell.y, firstSubCell.x);
	}
	isStatic = !up;
    return up;
}

void Cell::resetCell(){
	isStatic=true;
    for(int i = 0; i<settings->numOfSubCells; i++)
        for(int j=0; j<settings->numOfSubCells; j++){
		subCells[i][j].resetState();
		}
}

void Cell::resetPassed(){
	for(int i = 0; i < settings->numOfSubCells; i++){
	    for(int j = 0; j < settings->numOfSubCells; j++){
			subCells[i][j].setIsPassed(false);    
		}
	}
}

void Cell::resetUpdated(){
    for(int i = 0; i < settings->numOfSubCells; i++){
	    for(int j = 0; j < settings->numOfSubCells; j++){
			subCells[i][j].setIsUpdated(false);   
		}
	}
}

void Cell::resetStates(){
    for(int i = 0; i < settings->numOfSubCells; i++){
	    for(int j = 0; j < settings->numOfSubCells; j++){
			subCells[i][j].setStatus(0);  
		}
	}
}

void Cell::setUpdatedToAll(){
	for(int i = 0; i < settings->numOfSubCells; i++){
	    for(int j = 0; j < settings->numOfSubCells; j++){
			subCells[i][j].setIsUpdated(true);   
		}
	}
}

void Cell::resetPassedAndUpdated(){
	for(int i = 0; i < settings->numOfSubCells; i++){
	    for(int j = 0; j < settings->numOfSubCells; j++){
			subCells[i][j].setIsPassed(false); 
			subCells[i][j].setIsUpdated(false);
		}
	}
}

bool Cell::isInside(Point point, Point* subCell, bool strict){
	if(point.x >= upperLeft.x && point.y >= upperLeft.y && point.x < getBottomRight().x && point.y < getBottomRight().y){
		if(subCell != nullptr){
			subCell->setIsValid(false);
		}

		if(settings->numOfSubCells == 1){
			if(subCell != nullptr){
			    subCell->x = 0;
			    subCell->y = 0;
				subCell->setIsValid(true);
			}
			return true;
		}
		//int div = settings->subCellSizes[settings->posnum -1] + settings->distSub + 1; //!!!!!!!!!!!!!! *2 + if....
		int div = settings->subCellSizes[settings->posnum -1]*2 + settings->distSub;// + settings->convex ? 1 : 0;
		int x = (point.x - upperLeft.x) / div;
		int y = (point.y - upperLeft.y) / div;
		if(subCells[y][x].isInside(point,strict) ){
			if(subCell != nullptr){
			    subCell->x = x;
			    subCell->y = y;
		    	subCell->setIsValid(true);
			}
		}

		return true;
	}
	if(subCell != nullptr){
	    subCell->x = -1;
	    subCell->y = -1;
        subCell->setIsValid(false);
	}
	return false;
}

void Cell::display(bool redraw) {
	bool clear = settings->grow && mode!=1 && mode!=2 && stIncr==false && (!isStatic || redraw); // subCells are getting smaller - so there is need to clear their previous state

	if(GlobalSettings::doubleBuffering){
		Graphics::createMemDc(&CRect(this->upperLeft.x,this->upperLeft.y,this->getBottomRight().x, this->getBottomRight().y)); 
	} 
	  
	if(!stIncr && settings->cellMonochrome && mode == MODE_EMPTY){
		Graphics::drawRectangle(nullptr,this->upperLeft.x,this->upperLeft.y, this->getBottomRight().x, this->getBottomRight().y, nullptr, settings->emptyIndex, settings->emptyIndex, subCells[0][0].getStatus(), subCells[0][0].getStatus());
	}
	else{
		for(int i=0; i<settings->numOfSubCells; i++){
			for(int j=0; j<settings->numOfSubCells; j++){
				subCells[i][j].display(mode,redraw || needsToBeRedrawn,clear);
			}
		}
	}

	if(GlobalSettings::doubleBuffering){
		Graphics::deleteMemDc();
	}

	needsToBeRedrawn = false;
}