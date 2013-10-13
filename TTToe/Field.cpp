#include "stdafx.h"
#include "Cell.h"
#include "Primitives.h"
#include "Graphics.h"
#include <list>
using std::list;

#include "Field.h"
	Field::Field(){
		upperLeft.x = -1;
		upperLeft.y = -1;

		drawSet = nullptr;
		settings = nullptr;

		cells = nullptr;
		rateField = nullptr;

		oldNum = 0;
		isDisplayedAfterUpdate = true;

		lastSelectedCell.x = -1;
		lastSelectedCell.y = -1;
		lastSelectedCell.z = 0;

		gameStarted = false;
	}

	Field::~Field(){
		if(cells != nullptr){
			for(int i = 0; i < settings->fieldSize; i++){
				delete []cells[i];
			}

			delete []cells;
		}

		if(rateField != nullptr){
			delete rateField;
		}
	}

	void Field::initialize(Settings* sett, Point p, DrawSets* drawSet, int mode){
		this->settings = sett;
		this->upperLeft = p;
		this->drawSet = drawSet;
		this->mode = mode;
		oldNum = sett->fieldSize;
		rateField = new RateField(sett->fieldSize);
		createCells();
		initializeCells();
	}

	void Field::setUpperLeft(const Point& ul, bool reCalculate){
		this->upperLeft = ul;
		
		if(reCalculate){
			initializeCenters(true);
		}
	}

	void Field::shift(int x, int y, bool reCalculate){
		this->upperLeft.x += x;
		this->upperLeft.y += y;

		if(reCalculate){
			initializeCenters(true);
		}
	}

	Cell** Field::getCells(){
		return cells;
	}

	void Field::setMode(int mode){
		this->mode = mode;
	}

	int Field::getMode(){
		return this->mode;
	}

	Point Field::getUpperLeft(){
		return upperLeft;
	}

	Point Field::getInnerUpperLeft(){
		Point innerUL;
		innerUL.x = upperLeft.x + (settings->frameExists ? settings->frameMargin + settings->frameWidth/2 + 1 : 0);
		innerUL.y = upperLeft.y + (settings->frameExists ? settings->frameMargin + settings->frameWidth/2 + 1 : 0);

		return innerUL;
	}

	Point Field::getBottomRight(){
		Point br;
		br.x = getInnerBottomRight().x + (settings->frameExists ? settings->frameMargin + settings->frameWidth/2 + 1 : 0);
		br.y = getInnerBottomRight().y + (settings->frameExists ? settings->frameMargin + settings->frameWidth/2 + 1 : 0);
		return br;
	}

	Point Field::getInnerBottomRight(){
		return cells[settings->fieldSize-1][settings->fieldSize-1].getBottomRight();
	}

	bool Field::getGameStarted(){
		return gameStarted;
	}

	CRect Field::getCRect(){
		return CRect(this->getUpperLeft().x, this->getUpperLeft().y, this->getBottomRight().x, this->getBottomRight().y);
	}

	int Field::getSize(){
		return settings->fieldSize;
	}

	int Field::getSizeForGivenSettings(Settings* sett, CRect& rect){
		Point tUpperLeft(sett->fieldULX, sett->fieldULY);

		rect.left = tUpperLeft.x;
		rect.top = tUpperLeft.y;

		Point innerUL;
		innerUL.x = tUpperLeft.x + (sett->frameExists ? sett->frameMargin + sett->frameWidth/2 + 1 : 0);
		innerUL.y = tUpperLeft.y + (sett->frameExists ? sett->frameMargin + sett->frameWidth/2 + 1 : 0);
				
		int dist = sett->distCellHalf * 2 + (sett->separatingLinesExist ? sett->separatingLineWidth : 0);

		Cell cell;
		cell.initialize(sett, innerUL, drawSet, Cell::MODE_EMPTY);
		int width = cell.getBottomRight().x - cell.getUpperLeft().x;

		innerUL.x += (dist + width)*(sett->fieldSize-1);
		innerUL.y += (dist + width)*(sett->fieldSize-1);

		cell.setUpperLeft(innerUL, true);

		Point innerBottomRight = cell.getBottomRight();
		Point tBottomRight = innerBottomRight;

		tBottomRight.x += (sett->frameExists ? sett->frameMargin + sett->frameWidth/2 + 1 : 0);
		tBottomRight.y += (sett->frameExists ? sett->frameMargin + sett->frameWidth/2 + 1 : 0);

		rect.right = tBottomRight.x;
		rect.bottom = tBottomRight.y;

		return tBottomRight.x - tUpperLeft.x;
	}

	void Field::createCells(){
		if(cells == nullptr || oldNum != settings->fieldSize){
			if(cells != nullptr){
				for(int i = 0; i < oldNum; i++){
					delete []cells[i];
				}

				delete []cells;
			}

			oldNum = settings->fieldSize;

			cells = new Cell*[settings->fieldSize];

			for(int i = 0; i < settings->fieldSize; i++){
				cells[i] = new Cell[settings->fieldSize];
			}

			if(rateField != nullptr){
				delete rateField;
			}
		
			rateField = new RateField(settings->fieldSize);
		}
	}

	void Field::initializeCells(bool keepModes){
		for(int i = 0; i < settings->fieldSize; i++){
			for(int j = 0; j < settings->fieldSize; j++){
				cells[i][j].setSettings(settings);
				cells[i][j].setDrawSet(drawSet);
				if(!keepModes){
					cells[i][j].setMode(Cell::MODE_EMPTY);
				}
			}
		}

		if(!keepModes){
			rateField->reset();
		}

		initializeCenters(true);
	}

	void Field::resetCellModes(){
		for(int i = 0; i < settings->fieldSize; i++){
			for(int j = 0; j < settings->fieldSize; j++){
				cells[i][j].setMode(Cell::MODE_EMPTY);
			}
		}

		rateField->reset();

		gameStarted = false;
	}

	// NOTE : if initialize == false then the Settings property of cells must be initialized before calling this method.
	//        otherwise all the settings must be initialized
	void Field::initializeCenters(bool initialize){
		Point tempC = getInnerUpperLeft();
		int baseX = tempC.x;

		int dist = settings->distCellHalf * 2 + (settings->separatingLinesExist ? settings->separatingLineWidth : 0);
		int width = -1;

		for(int i = 0; i < settings->fieldSize; i++){
			for(int j = 0; j < settings->fieldSize; j++){
				if(initialize){
					cells[i][j].setUpperLeft(tempC, false);
					cells[i][j].initialize();
				}
				else{
				    cells[i][j].setUpperLeft(tempC, true);
				}

				if(width == -1){
					width = cells[i][j].getBottomRight().x - cells[i][j].getUpperLeft().x;
				}

				tempC.x += dist + width;
			}

			tempC.y += dist + width;
			tempC.x = baseX;		
		}
	}

	bool Field::isInsideBorders(Point point){
		int oldMode = mode;
		mode = MODE_MOVE;
		bool inside = isInside(point, nullptr);
		mode = oldMode;
		return inside;
	}

	bool Field::isInsideFrame(Point point){
		if(settings->frameExists){
			return isInsideBorders(point);
		}
		else{
			int extra = settings->frameMargin + settings->frameWidth;
			if(point.x >= getInnerUpperLeft().x - extra && point.x < getInnerBottomRight().x + extra && point.y >= getInnerUpperLeft().y - extra && point.y < getInnerBottomRight().y + extra){
				return true;
			}
		}

		return false;
	}

    bool Field::isInside(Point point, Point* cell, Point* subCell, bool strict){

		if(cell != nullptr){
			cell->setIsValid(false);
		}

		if(point.x >= getInnerUpperLeft().x && point.x < getInnerBottomRight().x && point.y >= getInnerUpperLeft().y && point.y < getInnerBottomRight().y){
			if(mode == MODE_MOVE || settings->fieldSize == 1){
				if(cell != nullptr){
					cell->x = 0;
					cell->y = 0;
					cell->setIsValid(true);
				}
				return true;
			}

			int div = cells[0][0].getBottomRight().x - cells[0][0].getUpperLeft().x + settings->distCellHalf*2 + (settings->separatingLinesExist ? settings->separatingLineWidth : 0);
			int x = (point.x - getInnerUpperLeft().x) / div;
			int y = (point.y - getInnerUpperLeft().y) / div;


			if(cells[y][x].isInside(point, subCell, strict) ){
				if(cell != nullptr){
					cell->x = x;
					cell->y = y;
					cell->setIsValid(true);
				}
		    }

			return true;
		}
		if(cell != nullptr){
			cell->x = -1;
			cell->y = -1;
			cell->setIsValid(false);
		}
		return false;
	}

	bool Field::findInListOfAnimatedCells(Point point){
		list<Point>::iterator it = animatedCells.begin();
			
		bool found = false;

		if(animatedCells.size() > 0){
			while(it != animatedCells.end()){
				if( (*it) == point){
					return true;
				}
				++it;
			}
		}

		return false;
	}

	bool Field::mouseOver(Point point){ // returns true, if any action is performed(If the point is within the field)
		Point cell;
		Point subCell;
		if(isInside(point, &cell, &subCell)){
			if(cell.getIsValid()){
				if(!(cell == lastSelectedCell)){
					list<Point>::iterator it = animatedCells.begin();			
					bool found = false;

					if(animatedCells.size() > 0){
						while(it != animatedCells.end()){
							if( (*it) == cell){
								//if( !cells[cell.y][cell.x].getStIncr() ){
									cells[cell.y][cell.x].setStIncr(true);
								    cells[cell.y][cell.x].setIsStatic(false);

							        lastSelectedCell = cell;
								//}
								found = true;
							}
							++it;
						}
					}

					if(!found && subCell.getIsValid()){
						cells[cell.y][cell.x].setFirstSubCell(subCell);
						cells[cell.y][cell.x].setStIncr(true);
						cells[cell.y][cell.x].setIsStatic(false);
						cells[cell.y][cell.x].resetPassedAndUpdated();
						animatedCells.push_back(cell); 

						lastSelectedCell = cell;
					}

					//PlaySound(NULL, NULL, NULL);
				    //PlaySound(_T("mouse_over.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);

					return true;
				
				}

				return true;
			}
			else{
				lastSelectedCell.x = -1;
				lastSelectedCell.y = -1;
				return false;
			}
		}
		else{
			lastSelectedCell.x = -1;
			lastSelectedCell.y = -1;
			return false;
		}
	}

	Point Field::lButtonPressed(Point point, int mode){
		Point cell;
		isInside(point, &cell);

		if(cell.getIsValid() && cells[cell.y][cell.x].getMode() == Cell::MODE_EMPTY){
			cells[cell.y][cell.x].setMode(mode);

			cells[cell.y][cell.x].setStIncr(true);
			cells[cell.y][cell.x].setNeedsToBeRedrawn(true);
			cells[cell.y][cell.x].setIsStatic(false);

			gameStarted = true;

			if(!findInListOfAnimatedCells(cell)){
				animatedCells.push_back(cell);
			}

			Point pt(cell.x, cell.y);

			return pt;
		}

		Point pt;
		pt.setIsValid(false);

		return pt;
	}

	//bool Field::mouseLeftReleased(Point point){
	//	return false;
	//}

	bool Field::rButtonPressed(Point point){
		return false;
	}

	//bool Field::mouseRightReleased(Point point){
	//	return false;
	//}

	void Field::resetAnimatedCells(){
		animatedCells.clear();
		isDisplayedAfterUpdate = false;
	}

	bool Field::getAnimationActive(){
		if(!isDisplayedAfterUpdate){
			return true;
		}
		else{
			if(animatedCells.size() == 0){
				return false;
			}
			else{
				if(animatedCells.size() == 1){
					list<Point>::iterator it = animatedCells.begin();
					Point pt = *it;

					if(pt == lastSelectedCell && cells[it->y][it->x].getIsStatic()){
						return false;
					}
				}
			}
		}

		return true;
	}

	void Field::updateCells(bool displayAfterUpdate){
		
		if(animatedCells.size() >0){
			list<Point>::iterator it = animatedCells.begin();

			while(it!=animatedCells.end()){
				Cell* cell = &cells[it->y][it->x];
				if(cell->getIsStatic()){
					if(!( *it == lastSelectedCell)){
						if(cell->getStIncr()){
							cell->setStIncr(false);
							cell->setIsStatic(false);
							++it;

						    isDisplayedAfterUpdate = false;
						}
						else if(isDisplayedAfterUpdate){
							it = animatedCells.erase(it);
						}
						else{
							++it;
						}
					}
					else{
						++it;
					}

				}
				else{
					if(isDisplayedAfterUpdate){
						cell->resetPassedAndUpdated();
					}
					else{
						cell->resetPassed();
					}
					if(!( *it == lastSelectedCell) && !settings->showtrack){
						cell->setStIncr(false);	
					}
					cell->updateCell();
				  	++it;

					isDisplayedAfterUpdate = false;
				}
			}

			if(displayAfterUpdate){
				display();
			}
		}
	}

	void Field::clear(int left, int top, int right, int bottom){
			Point ul, br;

			ul = getUpperLeft();
			br = getBottomRight();

			if(left != -1 && left > ul.x){
				ul.x = left;
			}
			if(top != -1 && top > ul.y){
				ul.y = top;
			}
			if(right != -1 && right < br.x){
				br.x = right;
			}
			if(bottom != -1 && bottom < br.y){
				br.y = bottom;
			}

			if(settings->convex){
				br.x--;
				br.y--;
			}

			Graphics::drawRectangle(nullptr, ul.x, ul.y, br.x+1, br.y+1, nullptr, settings->backgroundIndex, settings->backgroundIndex, 0, 0);
	}

	void Field::display(bool renderUpdatedCellsOnly, bool renderUpdatedSubCellsOnly, bool renderFrame, bool renderLines){

		isDisplayedAfterUpdate = true;

		if(renderFrame && settings->frameExists){
			Point ul, br;
					
			ul = getUpperLeft();
			br = getBottomRight();

			if(settings->convex){
				br.x--;
				br.y--;
			}

			Graphics::drawRectangle(nullptr, ul.x, ul.y, br.x, br.y, nullptr, settings->frameColPos, settings->fieldBackgroundIndex, 0, 0);
			
		}

		if(renderLines && settings->separatingLinesExist){
				int x;
				int y;

				Point upperLeftBorder;
				upperLeftBorder.x = (settings->frameExists ? this->getInnerUpperLeft().x - settings->frameMargin : this->getInnerUpperLeft().x);
				upperLeftBorder.y = (settings->frameExists ? this->getInnerUpperLeft().y - settings->frameMargin : this->getInnerUpperLeft().y);

		        Point bottomRightBorder;
				bottomRightBorder.x = (settings->frameExists ? this->getInnerBottomRight().x + settings->frameMargin : this->getInnerBottomRight().x);
				bottomRightBorder.y = (settings->frameExists ? this->getInnerBottomRight().y + settings->frameMargin : this->getInnerBottomRight().y);

				if(settings->convex){
					bottomRightBorder.x--;
					bottomRightBorder.y--;
				}

				int shift = settings->distCellHalf + 1 + (settings->separatingLineWidth-1)/2;

				for(int i = 1; i < settings->fieldSize; i++){
					x = cells[i][i].getUpperLeft().x - shift;
					y = cells[i][i].getUpperLeft().y - shift;

					Graphics::drawLine(nullptr, upperLeftBorder.x, y, bottomRightBorder.x, y, nullptr, settings->separatingLineIndex, 0);
					Graphics::drawLine(nullptr, x, upperLeftBorder.y, x, bottomRightBorder.y, nullptr, settings->separatingLineIndex, 0);
				}
		}

		if(renderUpdatedCellsOnly){
			list<Point>::iterator it = animatedCells.begin();

			while(it != animatedCells.end()){
				cells[it->y][it->x].display(!renderUpdatedSubCellsOnly);
				++it;
			}		
		}
		else{
			for(int i = 0; i < settings->fieldSize; i++){
				for(int j = 0; j < settings->fieldSize; j++){
					cells[i][j].display(!renderUpdatedSubCellsOnly);
				}
			}
		}
	}

	__int64** Field::createFieldForAnalyse(){
		__int64** anFld = new __int64*[settings->fieldSize];

		for(int i = 0; i < settings->fieldSize; i++){
			anFld[i] = new __int64[settings->fieldSize];
		}
	
		for(int i = 0; i < settings->fieldSize; i++){
			for(int j = 0; j < settings->fieldSize; j++){
				anFld[i][j] = cells[i][j].getMode();
			}
		}

		return anFld;
	}

	void Field::aIPlayed(Point pt){
		Cell& cell = this->cells[pt.y][pt.x];

		cell.setMode(Cell::MODE_PC);
		cell.setNeedsToBeRedrawn(true);
		cell.setStIncr(true);
		cell.setIsStatic(false);
		cell.setFirstSubCell(Point(this->settings->numOfSubCells/2,this->settings->numOfSubCells/2));

		if(!findInListOfAnimatedCells(pt)){
			animatedCells.push_back(pt);
		}
	}

	void Field::animateCell(Point pt){
		Cell& cell = this->cells[pt.y][pt.x];

		cell.setNeedsToBeRedrawn(true);
		cell.setStIncr(true);
		cell.setIsStatic(false);
		cell.resetStates();
		cell.setFirstSubCell(Point(this->settings->numOfSubCells/2,this->settings->numOfSubCells/2));

		if(!findInListOfAnimatedCells(pt)){
			animatedCells.push_back(pt);
		}
	}

	bool Field::makeMove(Point pt, int mode, bool recInterLines ){
		return rateField->makeMove(pt, mode, recInterLines);
	}

	bool Field::checkIfCompletedCombinationExist(){
		return rateField->checkIfCompletedCombinationExist();
	}

	bool Field::checkIfDrawExists(){
		return rateField->checkIfDrawExists();
	}

	vector<Combination> Field::getExtendedCompletedCombinations(){
		return rateField->getExtendedCompletedCombinations();
	}

	Point Field::evaluateAIMove(){
		return rateField->evaluateAIMove();
	}

	int Field::getMovesMade(){
		return rateField->getMovesMade();
	}

	int Field::getAIPercent(){
		return rateField->getPercent();
	}

	void Field::forceStopThinking(){
		rateField->setStopThinking(true);
	}