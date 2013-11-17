#include "stdafx.h"
#include "SwingButton.h"
#include "Utils.h"

SwingButton::SwingButton(){
	font = new CFont();
	font->CreateFont(0,8,0,0,FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");
}

SwingButton::SwingButton(int lBCEvID, int rBCEvID, Point upperLeft, int height, int width, int labelHeight, string label, string leftLabel, string rightLabel, SwingButtonStyle* st){
	this->upperLeft = upperLeft;
	this->height = height;
	this->width = width;
	this->labelHeight = labelHeight;
	this->label = label;

	this->leftButton.setButtonClickedEventID(lBCEvID);
	this->rightButton.setButtonClickedEventID(rBCEvID);

	this->leftButton.setLabel(leftLabel);
	this->rightButton.setLabel(rightLabel);

    this->style = st;

	initializeButtonPositions();

	this->leftButton.setStyle(st->buttonStyle);
	this->rightButton.setStyle(st->buttonStyle);



	font = new CFont();
	font->CreateFont(0,8,0,0,FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");
}

SwingButton::~SwingButton(){
	delete font;
}

void SwingButton::initializeButtonPositions(){
	Point ul = this->upperLeft;
	if(this->style->labelExists){
		ul.y += this->labelHeight;
	}
	
	leftButton.setUpperLeft(ul);

	int bHeight = this->height - (this->style->labelExists ? this->labelHeight : 0);
	int lwidth = this->width/2;
	int rwidth = this->width - lwidth;

	leftButton.setHeight(bHeight);
	leftButton.setWidth(lwidth);
	
	Point rightUL = leftButton.getBottomRight();
	rightUL.y = leftButton.getUpperLeft().y;

	rightButton.setUpperLeft(rightUL);
	rightButton.setHeight(bHeight);
	rightButton.setWidth(rwidth);
}

void SwingButton::recalculate(){
	initializeButtonPositions();
}

void SwingButton::setStyle(SwingButtonStyle* st){
	this->style = st;
	this->leftButton.setStyle(st->buttonStyle);
	this->rightButton.setStyle(st->buttonStyle);
}

SwingButtonStyle* SwingButton::getStyle(){
	return this->style;
}

void SwingButton::setLeftButtonClickedEventID(int id){
	this->leftButton.setButtonClickedEventID(id);
}

int SwingButton::getLeftButtonClickedEventId(){
	return this->leftButton.getButtonClickedEventId();
}

void SwingButton::setRightButtonClickedEventID(int id){
	this->rightButton.setButtonClickedEventID(id);
}

int SwingButton::getRightButtonClickedEventId(){
	return this->rightButton.getButtonClickedEventId();
}

void SwingButton::setUpperLeft(Point ul){
	this->upperLeft = ul;
	initializeButtonPositions();
}

void SwingButton::setUpperLeftSmpl(Point ul){
	this->upperLeft = ul;
}

Point SwingButton::getUpperLeft() const{
	return this->upperLeft;
}

void SwingButton::setHeight(int h){
	this->height = h;
	initializeButtonPositions();
}

void SwingButton::setHeightSmpl(int h){
	this->height = h;
}

int SwingButton::getHeight(){
	return this->height;
}

void SwingButton::setWidth(int width){
	this->width = width;
	initializeButtonPositions();
}

void SwingButton::setWidthSmpl(int width){
	this->width = width;
}

int SwingButton::getWidth(){
	return this->width;
}

void SwingButton::setLabelHeight(int height){
	this->labelHeight = height;
	initializeButtonPositions();
}

int SwingButton::getLabelHeight(){
	return this->labelHeight;
}

Point SwingButton::getBottomRight() const{
	Point br = this->upperLeft;
	return br.shift(this->width, this->height);
}

void SwingButton::setLabel(string lbl){
	this->label = lbl;
}

string SwingButton::getLabel(){
	return this->label;
}

void SwingButton::setLeftLabel(string lbl){
	this->leftButton.setLabel(lbl);
}

string SwingButton::getLeftLabel(){
	return this->leftButton.getLabel();
}

void SwingButton::setRightLabel(string lbl){
	this->rightButton.setLabel(lbl);
}

string SwingButton::getRightLabel(){
	return this->rightButton.getLabel();
} 

void SwingButton::display(){

	Point tempUL = getUpperLeft();
	Point tempBR = getBottomRight();

	CRect tempRect = CRect(tempUL.x, tempUL.y, tempBR.x, tempBR.y);

	Graphics::createMemDc(&tempRect);

	if(this->style->labelExists){
		if(!this->style->labelBGTransparent){
			CPen pn(0,1,this->style->getLabelBGColorRGB());
			CBrush br(this->style->getLabelBGColorRGB());

			Graphics::drawRectangle(nullptr, this->upperLeft.x, this->upperLeft.y, this->upperLeft.x + width, this-> upperLeft.y + this->labelHeight,
									&br, &pn, true, true, true);
		}

		Graphics::drawText(nullptr, this->upperLeft.x, this->upperLeft.y, this->upperLeft.x + width, this-> upperLeft.y + this->labelHeight,
								this->font, this->label, this->style->getLabelColorRGB(), true);
	}

	this->leftButton.display();
	CPen pen(0,1,leftButton.getCurrentColor().getRGB());

	Graphics::drawLine(nullptr, leftButton.getBottomRight().x - 1, leftButton.getUpperLeft().y + 1, leftButton.getBottomRight().x-1, leftButton.getBottomRight().y - 1,
		              &pen, true);

	this->rightButton.display();
	CPen pen2(0,1,rightButton.getCurrentColor().getRGB());

	Graphics::drawLine(nullptr, rightButton.getUpperLeft().x, rightButton.getUpperLeft().y + 1, rightButton.getUpperLeft().x, rightButton.getBottomRight().y - 1,
		              &pen2, true);

	Graphics::deleteMemDc();
}

bool SwingButton::isInside(Point pt) const{
	return pt.x >= upperLeft.x && pt.y >= upperLeft.y && 
		   pt.x < getBottomRight().x && pt.y < getBottomRight().y;
}

Action SwingButton::lButtonPressed(Point pt){
	Action act;
	
	if(isInside(pt)){
		act.status = this->POINT_INSIDE;

		Action lbAction = this->leftButton.lButtonPressed(pt);
		Action rbAction = this->rightButton.lButtonPressed(pt);

		if(lbAction.status & this->POINT_INSIDE){
			act.valueReturned = lbAction.valueReturned;
			act.value = lbAction.value;
		}
		else if(rbAction.status & this->POINT_INSIDE){
			act.valueReturned = rbAction.valueReturned;
			act.value = rbAction.value;
		}
		else{
			act.valueReturned = false;
		}
	}
	else{
		this->leftButton.lButtonPressed(pt);
		this->rightButton.lButtonPressed(pt);

		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action SwingButton::rButtonPressed(Point pt){
	Action act;
	
	if(isInside(pt)){
		act.status = this->POINT_INSIDE;

		Action lbAction = this->leftButton.rButtonPressed(pt);
		Action rbAction = this->rightButton.rButtonPressed(pt);

		if(lbAction.status & this->POINT_INSIDE){
			act.valueReturned = lbAction.valueReturned;
			act.value = lbAction.value;
		}
		else if(rbAction.status & this->POINT_INSIDE){
			act.valueReturned = rbAction.valueReturned;
			act.value = rbAction.value;
		}
		else{
			act.valueReturned = false;
		}
	}
	else{
		this->leftButton.rButtonPressed(pt);
		this->rightButton.rButtonPressed(pt);

		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action SwingButton::mouseMovedOver(Point pt){
	Action act;
	
	if(isInside(pt)){
		act.status = this->POINT_INSIDE;
		act.valueReturned = false;

		this->leftButton.mouseMovedOver(pt);
		this->rightButton.mouseMovedOver(pt);
	}
	else{
		this->leftButton.mouseMovedOver(pt);
		this->rightButton.mouseMovedOver(pt);

		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action SwingButton::timerTicked(){
	Action act;
	
	act.status = 0;
	act.valueReturned = false;

	Action act1 = this->leftButton.timerTicked();
	Action act2 = this->rightButton.timerTicked();

	act.status |= (act1.status | act2.status);
	
	return act;
}

bool SwingButton::getIsUpdated(){
	return this->leftButton.getIsUpdated() || this->rightButton.getIsUpdated();
}

void SwingButton::displayUpdated(){
	if(this->leftButton.getIsUpdated() || this->rightButton.getIsUpdated()){
		CRect contRect = Utils::getContainingRect(this->leftButton.getUpperLeft(), this->leftButton.getBottomRight(), this->rightButton.getUpperLeft(), this->rightButton.getBottomRight());

		Graphics::createMemDc(&contRect);

		if(this->leftButton.getIsUpdated()){
			this->leftButton.displayUpdated();
			CPen pen(0, 1, leftButton.getCurrentColor().getRGB());
			Graphics::drawLine(nullptr, leftButton.getBottomRight().x - 1, leftButton.getUpperLeft().y + 1, leftButton.getBottomRight().x - 1, leftButton.getBottomRight().y - 1,
						  &pen, true);
		}
		if(this->rightButton.getIsUpdated()){
			this->rightButton.displayUpdated();
			CPen pen(0,1,rightButton.getCurrentColor().getRGB());
			Graphics::drawLine(nullptr, rightButton.getUpperLeft().x, rightButton.getUpperLeft().y + 1, rightButton.getUpperLeft().x, rightButton.getBottomRight().y - 1,
						  &pen, true);
		}

		Graphics::deleteMemDc();
	}

}


