#ifndef CONTROLELEMENT_H
#define CONTROLELEMENT_H

#include "GUIElement.h"

class Action{
public:
	int status;
	bool valueReturned;
	int value;

	Action(){
		status = value = -1;
		valueReturned = false;
	}
};

class ControlElement : public GUIElement{
protected:
	//bool lButtonClicked;
	//bool rButtonClicked;

public:
	static const int POINT_INSIDE = 0x1;
	static const int L_BUTTON_CLICKED = 0x10;
	static const int R_BUTTON_CLICKED = 0x100;

	static const int ELEMENT_UPDATED = 0x1000;

	ControlElement(){
		//lButtonClicked = false;
		//rButtonClicked = false;
	}

	virtual Action lButtonPressed(Point) = 0;
	virtual Action rButtonPressed(Point) = 0;
	virtual Action mouseMovedOver(Point) = 0;
	virtual Action timerTicked() = 0;

	virtual void recalculate() = 0;
	virtual void displayUpdated() = 0;
	virtual bool getIsUpdated() = 0;
};

#endif