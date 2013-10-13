#include "stdafx.h"
#include "FieldLineIterator.h"

FieldLineIterator::FieldLineIterator(){
	direction = DIRECTION_UNDEFINED;
	currentPoint.setIsValid(false);
	pointOnLine.setIsValid(false);
	initializeCurrentPoint = false;

	dx = INT_MAX;
	dy = INT_MAX;
}

FieldLineIterator::FieldLineIterator(int size, int direc, Point pointOnLine, bool startFromThePoint){
	this->size = size;
	this->direction = direc;
	if(startFromThePoint){
		currentPoint = pointOnLine;
		currentPoint.valid = true;
		initializeCurrentPoint = false;
	}
	else{
		currentPoint.valid = false;
		currentPoint.x = 0;
		currentPoint.y = 0;
		initializeCurrentPoint = true;
	}
	this->pointOnLine = pointOnLine;

	setVectorFromDirection(direc);
}

void FieldLineIterator::initialize(int size, int direc, Point pointOnLine, bool startFromThePoint){
	this->size = size;
	this->direction = direc;
	if(startFromThePoint){
		currentPoint = pointOnLine;
		currentPoint.valid = true;
		initializeCurrentPoint = false;
	}
	else{
		currentPoint.valid = false;
		currentPoint.x = 0;
		currentPoint.y = 0;
		initializeCurrentPoint = true;
	}

	this->pointOnLine = pointOnLine;

	setVectorFromDirection(direc);
}

void FieldLineIterator::setVectorFromDirection(int dir){
	switch(dir){

				case DIRECTION_HORIZONTAL:
					dx = 1;
					dy = 0;

					break;
				case DIRECTION_HORIZONTAL_BACWARDS:
					dx = -1;
					dy = 0;

					break;
				case DIRECTION_VERTICAL:
					dx = 0;
					dy = 1;

					break;
				case DIRECTION_VERTICAL_BACKWARDS:
					dx = 0;
					dy = -1;

					break;
				case DIRECTION_MAIN_DIAG:
					dx = 1;
					dy = 1;

					break;
				case DIRECTION_MAIN_DIAG_BACKWARDS:
					dx = -1;
					dy = -1;
					break;
				case DIRECTION_SEC_DIAG:
					dx = 1;
					dy = -1;

					break;
				case DIRECTION_SEC_DIAG_BACKWARDS:
					dx = -1;
					dy = 1;

					break;
			}
}

void FieldLineIterator::initializeStartPoint(int dir){
	currentPoint.valid = true;

	Point point;
	point.x = pointOnLine.x;
	point.y = pointOnLine.y;

	int dif,sum;

	switch(direction){

	case DIRECTION_HORIZONTAL:
		currentPoint.y = point.y;
		currentPoint.x = 0;

		break;
	case DIRECTION_HORIZONTAL_BACWARDS:
		currentPoint.y = point.y;
		currentPoint.x = size - 1;

		break;
	case DIRECTION_VERTICAL:
		currentPoint.y = 0;
		currentPoint.x = point.x;

		break;
	case DIRECTION_VERTICAL_BACKWARDS:
		currentPoint.y = size - 1;
		currentPoint.x = point.x;

		break;
	case DIRECTION_MAIN_DIAG:
		dif = point.y - point.x;

		if(dif > 0){
			currentPoint.y = dif;
			currentPoint.x = 0;
		}
		else{
			currentPoint.y = 0;
			currentPoint.x = -dif;
		}

		break;
	case DIRECTION_MAIN_DIAG_BACKWARDS:
		dif = point.y - point.x;

		if(dif > 0){
			currentPoint.y = size - 1;
			currentPoint.x = size - 1 -dif;
		}
		else{
			currentPoint.y = size - 1 + dif;
			currentPoint.x = size - 1;
		}

		break;
	case DIRECTION_SEC_DIAG:
		sum = point.y + point.x;

		if(sum < size){
			currentPoint.y = sum;
			currentPoint.x = 0;
		}
		else{
			currentPoint.y = size - 1;
			currentPoint.x = sum - currentPoint.y;
		}

		break;
	case DIRECTION_SEC_DIAG_BACKWARDS:
		sum = point.y + point.x;

		if(sum < size){
			currentPoint.y = 0;
			currentPoint.x = sum;
		}
		else{// vice-versa
			currentPoint.x = size - 1;
			currentPoint.y = sum - currentPoint.x;
		}

		break;
	}
}

Point FieldLineIterator::getCurrentPoint(){
	if(currentPoint.getIsValid()){
		return currentPoint;
	}

	Point point(-1,-1);
	point.setIsValid(false);

	return point;
}
	
bool FieldLineIterator::getNext(){
	return move(1);
}

bool FieldLineIterator::getPrevious(){
	return move(-1);
}

bool FieldLineIterator::move(int count){
	if(direction == DIRECTION_UNDEFINED || !(initializeCurrentPoint || currentPoint.valid)){
		throw new std::exception("Iterator is not initialized");
	}
	else{
		bool borderReached = false;

		int locDir = this->direction;
		int locDx = this->dx;
		int locDy = this->dy;


		if(count < 0){ 
			locDir = - direction; // See the values of direction constants.
			locDx = -locDx;
			locDy = -locDy;
			count = -count;
		}


		if(initializeCurrentPoint){
			initializeStartPoint(locDir);
			initializeCurrentPoint = false;

			count--;
		}
		if(count > 0){

			tempX = count*locDx;
			tempY = count*locDy;

			currentPoint.x += tempX;
			currentPoint.y += tempY;

			if(currentPoint.x < 0 || currentPoint.x >= size ||
				currentPoint.y < 0 || currentPoint.y >= size){
					
				initializeStartPoint(-locDir);
				currentPoint.valid = false; 

				borderReached = true;				   
	
			}
				
		}

		return !borderReached;
	}
}
