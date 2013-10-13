#ifndef MENU_H
#define MENU_H

#include "ControlElement.h"
#include "Primitives.h"
#include "Graphics.h"
#include "Button.h"
#include "SwingButton.h"
#include <string>
#include <vector>
using std::string;

class Menu : public ControlElement{
private:
	Point upperLeft;
	Point oldUpperLeft;
	Point openedUpperLeft;
	Point closedUpperLeft;
	int height;
	int width;

	int status;

	Button openButton;
	SwingButton navigationButton;

	int velocity;

	MenuStyle* style;
	CFont* font;

	string label;

	vector<vector<ControlElement*>> elements;

	size_t menuIndex;

	bool displayedAfterMove;
	int xShift;
public:

	static const int MENU_OPENED = 1;
	static const int MENU_OPENING = 2;
	static const int MENU_CLOSING = 3;
	static const int MENU_CLOSED = 4;

	Menu();
	Menu(int, int, int, Point, Point, Point, int, int, int, int, int, int, string, MenuStyle*);
	virtual ~Menu();

	void setStyle(MenuStyle*);
	MenuStyle* getStyle();
	void setLabel(string);
	string getLabel();
	void setOpenButtonClickedEventID(int id);
	int getOpenButtonClickedEventId();

	void setOpenedUpperLeft(Point);
	Point getOpenedUpperLeft() const;
	void setClosedUpperLeft(Point);
	Point getClosedUpperLeft() const;

	void insertElement(int,ControlElement*);
	void insertList(vector<ControlElement*>);
	//void popElement(int);

	void setHeight(int);
	int getHeight();
	void setWidth(int);
	int getWidth();
	void setOpenButtonWidth(int);
	int getOpenButtonWidth();
	Point getBottomRight() const;

	int getStatus() const;

	bool animate();
	void setCoordinates();

	virtual void setUpperLeft(Point);
	virtual Point getUpperLeft() const;

	virtual void display();
	virtual bool isInside(Point) const;
	
	virtual Action lButtonPressed(Point);
	virtual Action rButtonPressed(Point);
	virtual Action mouseMovedOver(Point);
	virtual Action timerTicked();

	virtual bool getIsUpdated();
	virtual void displayUpdated();
	virtual void recalculate();
};

#endif