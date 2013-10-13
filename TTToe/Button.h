#ifndef BUTTON_H
#define BUTTON_H

#include "ControlElement.h"
#include "Primitives.h"
#include "Graphics.h"
#include <string>
using std::string;

class Button : public ControlElement{
private:
	int buttonClickedEventID;
	Point upperLeft;
	int height;
	int width;
	bool mouseOver;
	bool stIncr;
	ButtonStyle* style;
	CFont* font;

	string label;

	int animStatus;

	bool updated;

public:

	Button();
	Button(int, Point, int, int, string, ButtonStyle*);
	virtual ~Button();

	void setStyle(ButtonStyle*);
	ButtonStyle* getStyle();
	void setButtonClickedEventID(int id);
	int getButtonClickedEventId();
	void setHeight(int);
	int getHeight();
	void setWidth(int);
	int getWidth();
	void setMouseOver(bool);
	bool getMouseOver();
	void setStIncr(bool);
	bool getStIncr();
	Point getBottomRight() const;
	void setLabel(string);
	string getLabel();

	Clr getCurrentColor() const;
	Clr getCurrentColorFrame() const;

	virtual void setUpperLeft(Point);
	virtual Point getUpperLeft() const;

	virtual void display();
	virtual bool isInside(Point) const;
	
	virtual Action lButtonPressed(Point);
	virtual Action rButtonPressed(Point);
	virtual Action mouseMovedOver(Point);
	virtual Action timerTicked();

	virtual void recalculate(){};
	virtual void displayUpdated();
	virtual bool getIsUpdated();
};

#endif