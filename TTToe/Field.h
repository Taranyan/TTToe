#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include "Primitives.h"
#include "Graphics.h"
#include <list>
#include "Object.h"
#include "ControlElement.h"
#include "RateField.h"
#include "BaseField.h"
using std::list;

class Field : public BaseField
{
public:
	static const int MODE_MOVE = 1;
	static const int MODE_PLAY = 0;
private:

	Point upperLeft;
	Settings* settings;
	DrawSets* drawSet;

	int oldNum;
	int mode;

	bool isDisplayedAfterUpdate;
	list<Point> animatedCells;
	Point lastSelectedCell;
	bool gameStarted;

public:
	RateField* rateField;

	Field();
	~Field();
    void initialize(Settings* sett, Point p, DrawSets* drawSet, int mode = MODE_PLAY);	
	void setUpperLeft(const Point& ul, bool reCalculate = false);
	void shift(int x, int y, bool reCalculate = false);
	bool getGameStarted();
	Point getUpperLeft();
	Cell** getCells();
	Point getInnerUpperLeft();
	Point getBottomRight();
	Point getInnerBottomRight();
	CRect getCRect();
	int getSize();
	void setMode(int mode);
	int getMode();
	int getSizeForGivenSettings(Settings* sett, CRect& rect);
	void createCells();
	void resetCellModes();
	void initializeCells(bool keepModes = false);
	void initializeCenters(bool initialize); // NOTE : if initialize == false then the Settings property of cells must be initialized before calling this method.
										     //        otherwise all the settings must be initialized. This also calles initialize method of Cells.
	bool isInsideBorders(Point point);
	bool isInsideFrame(Point point);
    bool isInside(Point point, Point* cell, Point* subCell = nullptr, bool strict = false);
	bool mouseOver(Point point);
	Point lButtonPressed(Point point, int mode = BaseCell::MODE_PLAYER);
	bool rButtonPressed(Point point);
	void resetAnimatedCells();
	void updateCells(bool displayAfterUpdate = true);
	bool findInListOfAnimatedCells(Point);
	bool getAnimationActive();
	void clear(int left = -1, int top = -1, int right = -1, int bottom = -1);
	void display(bool renderUpdatedCellsOnly = true, bool renderUpdatedSubCellsOnly = true, bool renderFrame = false, bool renderLines = false);
	__int64** createFieldForAnalyse();
	void aIPlayed(Point);
	void animateCell(Point);
	bool makeMove(Point pt, int mode, bool recInterLines = true);
	Point evaluateAIMove();
	bool checkIfCompletedCombinationExist();
	bool checkIfDrawExists();	
	vector<Combination> getExtendedCompletedCombinations();
	int getMovesMade();
	int getAIPercent();
	void forceStopThinking();
};

#endif

