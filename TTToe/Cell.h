#ifndef CELL_H
#define CELL_H

#include "Primitives.h"
#include "stdafx.h"
#include "SubCell.h"
#include "GlobalSettings.h"
#include "BaseCell.h"

class Cell : public BaseCell
{

private:
   int mode;
   Point firstSubCell; 
   SubCell** subCells;
   int oldNum;
   bool stIncr;
   bool isStatic;
   bool needsToBeRedrawn;
   Point upperLeft;
   Settings* settings;
   DrawSets* drawSet;
   bool initialized;

public:
	Cell();
	~Cell();
	void initialize(Settings* sett, Point upperLeft, DrawSets* drawSet, int mode = 0);
	void initialize();
	void setFirstSubCell(Point point);
	Point getFirstSubCell();
	void setSettings(Settings* settings);
	Settings* getSettings();
	void setDrawSet(DrawSets* drawSet);
	DrawSets* getDrawSet();
	Point getUpperLeft();
	Point getBottomRight();
	void setStIncr(bool st);
	bool getStIncr();
	void setIsStatic(bool stat);
	bool getIsStatic();
	void setNeedsToBeRedrawn(bool n);
	bool getNeedsToBeRedrawn();
	void setMode(int mode);
	int getMode();
	bool setUpperLeft(Point pnt, bool initSubCells);
	Point getUpperLeftSubCenter();
	bool initializeCenters();
	bool initializeSubCells(bool initProps, bool initSubs);
	bool initializeProps();
	bool update(int i, int j);
	bool updateCell();
	void resetCell();
	void resetPassed();
	void resetUpdated();
	void resetStates();
	void setUpdatedToAll();
	void resetPassedAndUpdated();
	bool isInside(Point point, Point* subCell, bool strict = false);
	void display(bool redraw = false);
};

#endif