#ifndef FIELDLINEITERATOR_H
#define FIELDLINEITERATOR_H

#include "Primitives.h"
#include "RateCell.h"

class FieldLineIterator : public Object{
public:
	static const int DIRECTION_UNDEFINED = 0;

	static const int DIRECTION_HORIZONTAL = RateCell::DIRECTION_HORIZONTAL;
	static const int DIRECTION_HORIZONTAL_BACWARDS = -RateCell::DIRECTION_HORIZONTAL;
	static const int DIRECTION_VERTICAL = RateCell::DIRECTION_VERTICAL;
	static const int DIRECTION_VERTICAL_BACKWARDS = -RateCell::DIRECTION_VERTICAL;
	static const int DIRECTION_MAIN_DIAG = RateCell::DIRECTION_MAIN_DIAG; // From upper left corner to bottom right corner
	static const int DIRECTION_MAIN_DIAG_BACKWARDS = -RateCell::DIRECTION_MAIN_DIAG;
	static const int DIRECTION_SEC_DIAG = RateCell::DIRECTION_SEC_DIAG; // From bottom left corner to upper right corner
	static const int DIRECTION_SEC_DIAG_BACKWARDS = -RateCell::DIRECTION_SEC_DIAG;

private:
	int direction;
	int size;
	Point pointOnLine;
	bool initializeCurrentPoint;
	int dx;
	int dy;

	//
	int tempX;
	int tempY;
	//
public:

	Point currentPoint;

	FieldLineIterator();
	FieldLineIterator(int size, int direc, Point pointOnLine, bool startFromThePoint = false);
	void initialize(int size, int direc, Point pointOnLine, bool startFromThePoint = false);
	void setVectorFromDirection(int dir);
	void initializeStartPoint(int dir);
	Point getCurrentPoint();	
	bool getNext();
	bool getPrevious();
	bool move(int count);
};
#endif