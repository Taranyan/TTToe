#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "Primitives.h"
#include "Object.h"

class GUIElement : public Object{
public:

	virtual void display() = 0;
	virtual bool isInside(Point) const = 0;
	virtual Point getUpperLeft() const = 0;
	virtual void setUpperLeft(Point) = 0;
	virtual Point getBottomRight() const = 0;
	virtual ~GUIElement(){}
};

#endif