#ifndef SUBCELL_H
#define SUBCELL_H

#include "Primitives.h"
#include "Graphics.h"
#include "stdafx.h"

class SubCell : public Object
{

private:

	int status;
	bool isUpdated;
	bool isPassed;
	Point center;

    Settings* settings;
    DrawSets* drawSet;

public:

	SubCell(){
		isPassed = false;
		isUpdated = false;
		status = 0;
		settings = nullptr;
		drawSet = nullptr;
	}

	~SubCell(){
	}

	void setStatus(int status){
		this->status = status;
	}

	int getStatus(){
		return status;
	}

	void setIsPassed(bool isPassed){
		this->isPassed = isPassed;
	}

	bool getIsPassed(){
		return isPassed;
	}

	void setIsUpdated(bool isUpdated){
		this->isUpdated = isUpdated;
	}

	bool getIsUpdated(){
		return isUpdated;
	}

	void resetState(){
		status = 0;
		isUpdated = 0;
		isPassed = 0;
	}

	void setCenter(Point center){
		this->center = center;
	}

	Point getCenter(){
		return center;
	}

	void setSettings(Settings* settings){
		this->settings = settings;
	}

	const Settings* getSettings(){
		return settings;
	}
	/*
	void setCDC( CWnd* wnd )
	{
		if(clientDC!=nullptr && deleteDC == true){
			delete clientDC;
		}
		clientDC = new CClientDC(wnd);
		deleteDC = true;
	}

	void setCDC(CClientDC* p){
		if(this->clientDC != nullptr && deleteDC == true){
			delete this->clientDC;
		}
		this->clientDC = p;
		deleteDC = false;
	}

	void createOwnCDC(){
		CWnd* wnd = AfxGetApp()->m_pMainWnd;
		if(this->clientDC != nullptr && deleteDC == true){
			delete this->clientDC;
		}
		this->clientDC = new CClientDC(wnd);
		deleteDC = true;
	}
	
	CClientDC* getClientDC(){
		return clientDC;
	}
	
	*/
	void setDrawSet(DrawSets* drawSet){
		this->drawSet = drawSet;
	}

	DrawSets* getDrawSet(){
		return drawSet;
	}

	bool isInside(Point point, bool strict = false){
		int wdt;
		if(strict){
			wdt = settings->subCellSizes[status]; 
		}
		else{
			wdt = settings->subCellSizes[settings->posnum-1];
		}
		if(point.x >= center.x - wdt && point.x < center.x + wdt && point.y >= center.y - wdt && point.y < center.y + wdt ){
			return true;
		}
		return false;
	}

	void display(int mode, bool redraw, bool clear)
	{	
		if(this->isUpdated || redraw)
		{ 
			if( clear )
			{		 
				int width = settings->subCellSizes[settings->posnum-1];
				//CRect rect(center.x-width, center.y-width, center.x+width, center.y+width);
 				Graphics::drawRectangle(nullptr,center.x-width, center.y-width, center.x+width, center.y+width,drawSet,settings->getFieldBackgroundIndex(),settings->getFieldBackgroundIndex(),0,0);
			}

//			int R,G,B;

			
			int curSize; 
			if( mode==1 || mode==2 )// 1 - player, 2 - pc
			{
				curSize = settings->subCellSizes[settings->posnum-1];
				int dsInd = (mode == 1 ? settings->xIndex : settings->oIndex);
				Graphics::drawRectangle(nullptr,center.x-curSize,center.y-curSize,center.x+curSize,center.y+curSize,drawSet, dsInd, dsInd, status, status);
			}
			else
			{
				if(settings->grow){
				   curSize = settings->subCellSizes[status];
				}
				else{
				   curSize= settings->subCellSizes[settings->posnum-1];
				}
				Graphics::drawRectangle(nullptr,center.x - curSize, center.y - curSize, center.x + curSize, center.y + curSize,
					drawSet->getBrushSet(settings->emptyIndex)->getBrush(status), settings->grow ? drawSet->getPenSet(settings->emptyIndex)->getPen(0) : drawSet->getPenSet(settings->emptyIndex)->getPen(status));
			}
		}
	}
};

#endif