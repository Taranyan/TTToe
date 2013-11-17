#include "stdafx.h"
#include "Menu.h"
#include "GlobalSettings.h"

Menu::Menu(){
	font = new CFont();
	font->CreateFont(0, 10, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");

	oldUpperLeft.setIsValid(false);
	displayedAfterMove = true;
	xShift = -1;
	menuIndex = 0;

	this->openedUpperLeft.setIsValid(false);
	this->closedUpperLeft.setIsValid(false);
	this->upperLeft.setIsValid(false);
}

Menu::Menu(int openBCEvID, int navBLCEvID, int navBRCEvID, Point openedUpperLeft, Point closedUpperLeft, Point currentUpperLeft, int status, 
	       int height, int width, int openButtonWidth, int navButtonHeight, int navButtonWidth, string lbl, MenuStyle* style){
	
	this->openedUpperLeft = openedUpperLeft;
	this->closedUpperLeft = closedUpperLeft;
	this->upperLeft = currentUpperLeft;

	this->xShift = openedUpperLeft.x - upperLeft.x;

	menuIndex = 0;

	this->status = status;
	this->height = height;
	this->width = width;
	this->label = lbl;
	this->style = style;

	this->navigationButton.setStyle(style->navigationButtonStyle);
	this->navigationButton.setHeightSmpl(navButtonHeight);
	this->navigationButton.setWidthSmpl(navButtonWidth);
	this->navigationButton.setLeftButtonClickedEventID(navBLCEvID);
	this->navigationButton.setRightButtonClickedEventID(navBRCEvID);
	this->navigationButton.setLeftLabel("<--");
	this->navigationButton.setRightLabel("-->");

	this->openButton.setWidth(openButtonWidth);
	this->openButton.setButtonClickedEventID(openBCEvID);
	this->openButton.setStyle(style->openButtonStyle);
	this->openButton.setLabel(this->status == (this->MENU_CLOSED | this->MENU_OPENING) ? string("<") : string(">")); 

	setCoordinates();

	this->velocity = 0;

	font = new CFont();
	font->CreateFont(0, 10, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,  DEFAULT_PITCH | FF_SWISS, L"Courier");

	oldUpperLeft.setIsValid(false);
	displayedAfterMove = true;
}

Menu::~Menu(){
	delete font;
}

void Menu::setCoordinates(){
	this->openButton.setUpperLeft(this->upperLeft);
	this->openButton.setHeight(this->height);

	this->navigationButton.setUpperLeft(Point(this->openButton.getBottomRight().x + style->elementsMarginLeft, this->upperLeft.y + style->labelHeight));
	this->navigationButton.recalculate();

	for(size_t l = 0; l < elements.size(); l++){
		vector<ControlElement*>* list = &elements[l]; 
		for(size_t i = 0; i < list->size(); i++){
			ControlElement* elem = list->at(i);

			int yCord = (i == 0 ? this->navigationButton.getBottomRight().y + style->elementsMarginTop: list->at(i-1)->getBottomRight().y + style->marginBetweenElements);

			elem->setUpperLeft(Point(this->openButton.getBottomRight().x + style->elementsMarginLeft, yCord));
			elem->recalculate();
		}
	}

	displayedAfterMove = false;
}

void Menu::insertElement(int listIndex, ControlElement* elem){
	Point ul(this->openButton.getBottomRight().x + style->elementsMarginLeft);
	ul.y = this->navigationButton.getBottomRight().y + this->style->elementsMarginTop;

	vector<ControlElement*>* list = &elements[listIndex];

	if(list->size() != 0){
		ControlElement* lastElem = list->at(list->size()-1);
		ul.y = lastElem->getBottomRight().y + style->marginBetweenElements;
	}

	elem->setUpperLeft(ul);
	elem->recalculate();
	list->push_back(elem);
}

void Menu::insertList(vector<ControlElement*> list){
	elements.push_back(list);
	setCoordinates();
}

void Menu::recalculate(){
	if(this->xShift != this->openedUpperLeft.x - this->upperLeft.x){
		this->upperLeft.x = this->openedUpperLeft.x - xShift;
	}

	setCoordinates();
	displayedAfterMove = false;
}

void Menu::setStyle(MenuStyle* st){
	this->style = st;
}

MenuStyle* Menu::getStyle(){
	return this->style;
}

void Menu::setLabel(string lbl){
	this->label = lbl;
}

string Menu::getLabel(){
	return this->label;
}

void Menu::setOpenButtonClickedEventID(int id){
	this->openButton.setButtonClickedEventID(id);
}

int Menu::getOpenButtonClickedEventId(){
	return this->openButton.getButtonClickedEventId();
}

void Menu::setUpperLeft(Point ul){
	this->upperLeft = ul;
	this->upperLeft.setIsValid(true);

	if(this->openedUpperLeft.getIsValid() && this->xShift == -1){
		xShift = openedUpperLeft.x - upperLeft.x;
	}
}

void Menu::setOpenedUpperLeft(Point pt){
	this->openedUpperLeft = pt;
	this->openedUpperLeft.setIsValid(true);

	if(this->upperLeft.getIsValid() && this->xShift == -1){
		xShift = openedUpperLeft.x - upperLeft.x;
	}
}
	
Point Menu::getOpenedUpperLeft() const{
	return this->openedUpperLeft;
}

void Menu::setClosedUpperLeft(Point pt){
	this->closedUpperLeft = pt;
}

Point Menu::getClosedUpperLeft() const{
	return this->closedUpperLeft;
}

Point Menu::getUpperLeft() const{
	return this->upperLeft;
}

void Menu::setHeight(int h){
	this->height = h;
}

int Menu::getHeight(){
	return this->height;
}

void Menu::setWidth(int width){
	this->width = width;
}

int Menu::getWidth(){
	return this->width;
}

void Menu::setOpenButtonWidth(int wdt){
	this->openButton.setWidth(wdt);
}

int Menu::getOpenButtonWidth(){
	return this->openButton.getWidth();
}

Point Menu::getBottomRight() const{
	Point br = this->upperLeft;
	return br.shift(this->width, this->height);
}


void Menu::display(){
	CRect rect(openedUpperLeft.x, openedUpperLeft.y, getBottomRight().x, getBottomRight().y); 

	Graphics::createMemDc(&rect);

	openButton.display();

	CPen pen(0,1,style->getBGColorRGB());
	CBrush brush(style->getBGColorRGB());

	CPen bgPen(0,1,GlobalSettings::backgroundClr.getRGB());
	CBrush bgBrush(GlobalSettings::backgroundClr.getRGB());

	CPen lblPen(0,1,style->labelBGColor.getRGB());
	CBrush lblBrush(style->labelBGColor.getRGB());

	Graphics::drawRectangle(nullptr, openedUpperLeft.x, openedUpperLeft.y, upperLeft.x, getBottomRight().y, &bgBrush, &bgPen, true, true, true);

	Graphics::drawRectangle(nullptr,this->openButton.getBottomRight().x, this->upperLeft.y, this->getBottomRight().x, this->getBottomRight().y,
		                    &brush, &pen, true, true, true);

	if(!style->labelBGTransparent){
		Graphics::drawRectangle(nullptr,openButton.getBottomRight().x, upperLeft.y, getBottomRight().x, upperLeft.y + style->labelHeight,
		                    &lblBrush, &lblPen, true, true, true);
	}

	Graphics::drawText(nullptr, openButton.getBottomRight().x, upperLeft.y, getBottomRight().x, 
		               upperLeft.y + style->labelHeight, font, label, style->labelColor, true);

	this->navigationButton.display();

	Graphics::drawRectangle(nullptr,openButton.getBottomRight().x, navigationButton.getBottomRight().y, getBottomRight().x, navigationButton.getBottomRight().y + style->panelWidthUnderNavButton,
		                    &lblBrush, &lblPen, true, true, true);

	vector<ControlElement*>* list = &elements[menuIndex]; 

	for(size_t i = 0; i < list->size(); i++){
		ControlElement* elem = list->at(i);
		elem->display();
	}

	Graphics::deleteMemDc();
	displayedAfterMove = true;
}

bool Menu::getIsUpdated(){
	if(!displayedAfterMove || status == this->MENU_CLOSING || status == MENU_OPENING){
		return true;
	}
	else{
		if(this->openButton.getIsUpdated() || this->navigationButton.getIsUpdated()){
			return true;
		}
		else{
			vector<ControlElement*>* list = &elements[menuIndex];
			for(size_t i = 0; i < list->size(); i++){
				if(list->at(i)->getIsUpdated()){
					return true;
				}
			}
		}
	}

	return false;
}

void Menu::displayUpdated(){
	if(!displayedAfterMove){
		display();
	}
	else{
		openButton.displayUpdated();
		navigationButton.displayUpdated();
		if(status != this->MENU_CLOSED){
			vector<ControlElement*>* list = &elements[menuIndex];
			for(size_t i = 0; i < list->size(); i++){
				list->at(i)->displayUpdated();
			}	
		}
	}
}


bool Menu::isInside(Point pt) const{
	return pt.x >= upperLeft.x && pt.y >= upperLeft.y && 
		   pt.x < getBottomRight().x && pt.y < getBottomRight().y;
}

bool Menu::animate(){
	this->oldUpperLeft = this->upperLeft;
	if(this->status == this->MENU_CLOSED || this->status == this->MENU_OPENED){
		return false;
	}
	else{
		if(this->status == this->MENU_CLOSING && this->upperLeft == this->closedUpperLeft ||
		   this->status == this->MENU_OPENING && this->upperLeft == this->openedUpperLeft){

		   if(this->status == this->MENU_CLOSING){
		      this->status = this->MENU_CLOSED;
		   }
		   else if(this->status == this->MENU_OPENING){
		      this->status = this->MENU_OPENED;
		   }	   

		     return false;
		}
		else{
			if(this->status == this->MENU_CLOSING){
				if(this->upperLeft == this->openedUpperLeft && this->velocity == 0){
					this->velocity = style->closeVo;
				}
				if(upperLeft.x + velocity <= closedUpperLeft.x){
					upperLeft.x += velocity;
					velocity += style->closeA;
					xShift = openedUpperLeft.x - upperLeft.x;
					setCoordinates();				
				}
				else{
					upperLeft.x = closedUpperLeft.x;
					velocity = 0;
					status = this->MENU_CLOSED;
					xShift = openedUpperLeft.x - upperLeft.x;
					setCoordinates();
					//return true;
				}
			}
			else{
				if(this->upperLeft == this->closedUpperLeft && this->velocity == 0){
					this->velocity = style->openVo;
				}

				if(upperLeft.x + velocity >= openedUpperLeft.x){
					upperLeft.x += velocity;
					velocity += style->openA;
					xShift = openedUpperLeft.x - upperLeft.x;
					setCoordinates();
				}
				else{

				    int acc1 = (int)((openedUpperLeft.x - upperLeft.x)*(style->openA)/(velocity));
					int acc2 = style->openA - acc1;

					if( (velocity + acc1)*(style->reflectionCoef) - acc2 > - style->maxStickSpeed){
						upperLeft.x = openedUpperLeft.x;
						velocity = 0;
						status = this->MENU_OPENED;
						xShift = openedUpperLeft.x - upperLeft.x;
						setCoordinates();
					//	return true;
					}
					else{
						upperLeft.x += -(int)(style->reflectionCoef * (upperLeft.x + velocity - openedUpperLeft.x - acc1));
						velocity = (int)(( -velocity - acc1) * style->reflectionCoef + acc2);
						xShift = openedUpperLeft.x - upperLeft.x;
						setCoordinates();
						//return true;
					}
				}
			}		
		}
	}

	return true;
}



Action Menu::lButtonPressed(Point pt){
	Action act;
	
	if(isInside(pt) && (this->status == MENU_CLOSED || this->status == MENU_OPENED)){
		act.status = this->POINT_INSIDE;
		act.valueReturned = false;

		Action act1 = this->openButton.lButtonPressed(pt);
		Action act2 = this->navigationButton.lButtonPressed(pt);
		Action bAct;

		vector<ControlElement*>* list = &elements[menuIndex];

		for(size_t i = 0; i < list->size(); i++){
			ControlElement* elem = list->at(i);
			bAct = elem->lButtonPressed(pt);
			if(bAct.status & this->POINT_INSIDE){
				act = bAct;
			}
		}

		if(this->status == MENU_CLOSED || this->status == MENU_OPENED){
			if(act1.status & this->POINT_INSIDE){
				act = act1;
				this->openButton.setLabel(this->status == this->MENU_CLOSED ? string(">") : string("<"));
				this->openButton.setMouseOver(false);
				this->status = this->status == this->MENU_CLOSED ? this->MENU_OPENING : this->MENU_CLOSING;
			}

			if(act2.status & this->POINT_INSIDE){
				act = act2;
				
				if(act2.value == this->navigationButton.getLeftButtonClickedEventId()){
					if(this->menuIndex > 0){
						this->menuIndex--;
					}
				}
				else if(act2.value == this->navigationButton.getRightButtonClickedEventId()){
					if(this->menuIndex < elements.size()-1){
						this->menuIndex++;
					}
				}

				this->display();
			}
		}

		//act.value = this->buttonClickedEventID;
	}
	else{
		act.status = 0;
		act.valueReturned = false;
	}

	return act;
}

Action Menu::rButtonPressed(Point pt){
	Action act;
	act.status = 0;
	if(this->status == MENU_CLOSED || this->status == MENU_OPENED){
		this->openButton.setLabel(this->status == this->MENU_CLOSED ? string(">") : string("<"));
		this->openButton.setMouseOver(false);
		this->status = this->status == this->MENU_CLOSED ? this->MENU_OPENING : this->MENU_CLOSING;
	}
	return act;
}

Action Menu::mouseMovedOver(Point pt){
	Action act;
	
	if(isInside(pt)){
		act.status = this->POINT_INSIDE;
		Action act2 = this->openButton.mouseMovedOver(pt);
		Action act3 = this->navigationButton.mouseMovedOver(pt);

		Action bAct;

		vector<ControlElement*>* list = &elements[menuIndex];

		for(size_t i = 0; i < list->size(); i++){
			ControlElement* elem = list->at(i);
			bAct = elem->mouseMovedOver(pt);
			if(bAct.status & this->POINT_INSIDE){
				act = bAct;
			}
		}


		if(act2.status & this->POINT_INSIDE){
			act = act2;
		}
		else if(act3.status & this->POINT_INSIDE){
			act = act3;
		}

	}
	else{
		act.status = 0;
		act.valueReturned = false;

		this->openButton.mouseMovedOver(pt);
		this->navigationButton.mouseMovedOver(pt);

		vector<ControlElement*>* list = &elements[menuIndex];

		for(size_t i = 0; i < list->size(); i++){
			ControlElement* elem = list->at(i);
			elem->mouseMovedOver(pt);
		}
	}
	
	return act;
}

Action Menu::timerTicked(){
	Action act;
	
	this->animate();
	this->openButton.timerTicked();
	this->navigationButton.timerTicked();

	for(size_t l = 0; l < elements.size(); l++){
		vector<ControlElement*>* list = &elements[l];
		for(size_t i = 0; i < list->size(); i++){
			list->at(i)->timerTicked();
		}	
	}
	return act;
}

