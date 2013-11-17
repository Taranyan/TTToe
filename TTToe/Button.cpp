#include "stdafx.h"
#include "Button.h"

Button::Button(){
	style = nullptr;
	animStatus = 0;
	mouseOver = false;
	stIncr = false;
	updated = false;

	font = new CFont();
	font->CreateFont(0, 10, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");
}

Button::Button(int bCEvID, Point upperLeft, int height, int width, string lbl, ButtonStyle* st){
	this->buttonClickedEventID = bCEvID;
	this->upperLeft = upperLeft;
	this->height = height;
	this->width = width;
	this->label = lbl;
	this->style = st;
	mouseOver = false;
	stIncr = false;
	updated = false;

	font = new CFont();
	font->CreateFont(0, 10, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");

	animStatus = 0;
}

Button::~Button(){
	delete font;
}

void Button::setStyle(ButtonStyle* st){
	this->style = st;
}

ButtonStyle* Button::getStyle(){
	return this->style;
}

void Button::setButtonClickedEventID(int id){
	this->buttonClickedEventID = id;
}

int Button::getButtonClickedEventId(){
	return this->buttonClickedEventID;
}

void Button::setUpperLeft(Point ul){
	this->upperLeft = ul;
}

Point Button::getUpperLeft() const{
	return this->upperLeft;
}

void Button::setHeight(int h){
	this->height = h;
}

int Button::getHeight(){
	return this->height;
}

void Button::setWidth(int width){
	this->width = width;
}

int Button::getWidth(){
	return this->width;
}

void Button::setMouseOver(bool over){
	this->mouseOver = over;
}

bool Button::getMouseOver(){
	return this->mouseOver;
}

void Button::setStIncr(bool incr){
	this->stIncr = incr;
}

bool Button::getStIncr(){
	return this->stIncr;
}

Point Button::getBottomRight() const{
	Point br = this->upperLeft;
	return br.shift(this->width, this->height);
}

void Button::setLabel(string lbl){
	this->label = lbl;
}

string Button::getLabel(){
	return this->label;
}

Clr Button::getCurrentColor() const{
	Clr clr;
	clr.r = style->startColor.r + (style->finalColor.r - style->startColor.r)*(this->animStatus)/style->finalStatus;
	clr.g = style->startColor.g + (style->finalColor.g - style->startColor.g)*(this->animStatus)/style->finalStatus;
	clr.b = style->startColor.b + (style->finalColor.b - style->startColor.b)*(this->animStatus)/style->finalStatus;
	return clr;
}

Clr Button::getCurrentColorFrame() const{
	Clr clr;
	clr.r = style->startColorFrame.r + (style->finalColorFrame.r - style->startColorFrame.r)*(this->animStatus)/style->finalStatus;
	clr.g = style->startColorFrame.g + (style->finalColorFrame.g - style->startColorFrame.g)*(this->animStatus)/style->finalStatus;
	clr.b = style->startColorFrame.b + (style->finalColorFrame.b - style->startColorFrame.b)*(this->animStatus)/style->finalStatus;
	return clr;
}

void Button::display(){
	Clr clr = getCurrentColor();
	Clr clrFrame = getCurrentColorFrame();

	CPen pen(0,1,RGB(clrFrame.r, clrFrame.g, clrFrame.b));
	CBrush brush(RGB(clr.r, clr.g, clr.b));

	Graphics::drawRectangle(nullptr, this->upperLeft.x, this->upperLeft.y, this->getBottomRight().x, this->getBottomRight().y,
		                    &brush, &pen, true, true, true);

	Graphics::drawText(nullptr, this->upperLeft.x, this->upperLeft.y, this->getBottomRight().x, this->getBottomRight().y, font, label, Clr(style->textColor.r,style->textColor.g,style->textColor.b), true);		               
	
	this->updated = false;
}

bool Button::isInside(Point pt) const{
	return pt.x >= upperLeft.x && pt.y >= upperLeft.y && 
		   pt.x < getBottomRight().x && pt.y < getBottomRight().y;
}

Action Button::lButtonPressed(Point pt){
	Action act;
	
	if(isInside(pt)){
		setStIncr(true);
		act.status = this->POINT_INSIDE;
		act.valueReturned = true;
		act.value = this->buttonClickedEventID;
	}
	else{
		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action Button::rButtonPressed(Point pt){
	Action act;
	
	if(isInside(pt)){
		act.status = this->POINT_INSIDE;
		act.valueReturned = false;
	}
	else{
		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action Button::mouseMovedOver(Point pt){
	Action act;
	
	if(isInside(pt)){
		this->setMouseOver(true);
		act.status = this->POINT_INSIDE;
		act.valueReturned = false;
	}
	else{
		this->setMouseOver(false);
		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action Button::timerTicked(){
	Action act;
	
	act.status = 0;
	act.valueReturned = false;

	if(getStIncr()){
		if(animStatus < style->finalStatus){
			animStatus++;
			act.status |= this->ELEMENT_UPDATED;
			this->updated = true;
		}
		else{
			setStIncr(false);
			animStatus--;
			act.status |= this->ELEMENT_UPDATED;
			this->updated = true;
		}
	}
	else{
		if(!mouseOver){
			if(animStatus > 0){
				animStatus--;
				act.status |= this->ELEMENT_UPDATED;
				this->updated = true;
			}
		}
		else{
			if(animStatus < style->highlightStatus){
				animStatus++;
				act.status |= this->ELEMENT_UPDATED;
				this->updated = true;
			}
			else if(animStatus > style->highlightStatus){
				animStatus--;
				act.status |= this->ELEMENT_UPDATED;
				this->updated = true;
			}
		}
	}
		
	return act;
}

bool Button::getIsUpdated(){
	return this->updated;
}

void Button::displayUpdated(){
	if(this->updated){
		this->display();
	}
}



