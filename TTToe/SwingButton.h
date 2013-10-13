#ifndef SWINGBUTTON_H
#define SWINGBUTTON_H

#include "ControlElement.h"
#include "Primitives.h"
#include "Graphics.h"
#include "Button.h"
#include <string>
using std::string;

class SwingButton : public ControlElement{
private:
	Button leftButton;
	Button rightButton;
	string label;
	Point upperLeft;
	int width;
	int height;
	int labelHeight;

	SwingButtonStyle* style;
	CFont* font;

public:

	SwingButton();
	SwingButton(int, int, Point, int, int, int, string, string, string, SwingButtonStyle*);
	virtual ~SwingButton();

	void setStyle(SwingButtonStyle*);
	SwingButtonStyle* getStyle();
	void setLeftButtonClickedEventID(int id);
	int getLeftButtonClickedEventId();
	void setRightButtonClickedEventID(int id);
	int getRightButtonClickedEventId();
	void setHeight(int);
	void setHeightSmpl(int);
	int getHeight();
	void setWidth(int);
	void setWidthSmpl(int);
	int getWidth();
	void setLabelHeight(int);
	int getLabelHeight();
	Point getBottomRight() const;
	void setLabel(string);
	string getLabel();
	void setLeftLabel(string);
	string getLeftLabel();
	void setRightLabel(string);
	string getRightLabel();

	void initializeButtonPositions();

	virtual void setUpperLeft(Point);
	virtual void setUpperLeftSmpl(Point);
	virtual Point getUpperLeft() const;

	virtual void display();
	virtual bool isInside(Point) const;
	
	virtual Action lButtonPressed(Point);
	virtual Action rButtonPressed(Point);
	virtual Action mouseMovedOver(Point);
	virtual Action timerTicked();

	virtual void recalculate();
	virtual void displayUpdated();
	virtual bool getIsUpdated();
};

#endif