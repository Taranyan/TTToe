#ifndef GRAPHICS_H
#define GRAPHICS_H

#include  <hash_map>
#include "stdafx.h"
#include "primitives.h"
#include "MemDC.h"
#include <boost/unordered_map.hpp>
#include <string>
#include "GlobalSettings.h"

using std::string;
using boost::unordered_map;


struct CachedPen{
	PenObject* pen;
	int timeUnused;
};

class Graphics
{
private:
	static int memDCCreations;

public:
	static CClientDC* cdc;
	static DrawSets* drawSets;

	static CMemmDC* memDC;

	static void createMemDc(CRect* r){
		if(GlobalSettings::doubleBuffering){
			if(memDC == nullptr){
				memDC = new CMemmDC(cdc,r);
				memDCCreations = 1;
			}
			else{
				++memDCCreations;
			}
		}
	}


	static void deleteMemDc(){
		if(memDCCreations == 1){
			delete memDC;
			memDC = nullptr;
			memDCCreations = 0;
		}
		else if(memDCCreations > 1){
			memDCCreations--;
		}
	}

	static void setCdc(CClientDC* dc){
		cdc = dc;
	}

	static CClientDC* getCdc(){
		return cdc;
	}


    static void drawRectangle(CClientDC* p, int x1, int y1, int x2, int y2){
		if(p != nullptr){
			p->Rectangle(x1,y1,x2,y2);
		}
		else{
			CDC* cdc1 = memDC != nullptr ? memDC : (CDC*)cdc;
			cdc1->Rectangle(x1,y1,x2,y2);
		}
	}

	static bool drawRectangle(CClientDC* p, int x1, int y1, int x2, int y2, DrawSets* DS, int penInd, int brushInd, int penStatus, int brushStatus, bool deselect = false){
		DrawSets* curDS = (DS == nullptr ? drawSets : DS);
		if(penInd<0 /*|| penInd >= curDS->getPenSetsSize()*/ || brushInd<0 /*|| brushInd >= curDS->getBrushSetsSize()*/){
			return false;
		}
		else{
			CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);

		    CPen* oldPen = clientdc->SelectObject(curDS->getPenSet(penInd)->getPenSafe(penStatus));
			CBrush* oldBrush = clientdc->SelectObject(curDS->getBrushSet(brushInd)->getBrushSafe(brushStatus));
            clientdc->Rectangle(x1,y1,x2,y2);

			if(deselect){
				clientdc->SelectObject(oldPen);
				clientdc->SelectObject(oldBrush);
			}

			return true;
		}
	}

	static bool drawRectangleW(CWnd* wnd,int x1, int y1, int x2, int y2, DrawSets* DS, int penInd, int brushInd, int penStatus, int brushStatus){
		DrawSets* curDS = (DS == nullptr ? drawSets : DS);
        if(penInd<0 /*|| penInd >= curDS->getPenSetsSize()*/ || brushInd<0 /*|| brushInd >= curDS->getBrushSetsSize()*/){
			return false;
		}
		else{
			CClientDC p(wnd);
			p.SelectObject(curDS->getPenSet(penInd)->getPenSafe(penStatus));
			p.SelectObject(curDS->getBrushSet(brushInd)->getBrushSafe(brushStatus));
			p.Rectangle(x1,y1,x2,y2);
			return true;
		}
	}


	static void drawRectangle(CClientDC* p, int x1, int y1, int x2, int y2, CBrush* br, CPen* pn, bool sb = true, bool sp = true, bool deselect = false){
		CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);

		CPen* oldPen;
		CBrush* oldBrush;

		if(sb){
			oldBrush = clientdc->SelectObject(br);
		}
		if(sp){
			oldPen = clientdc->SelectObject(pn);
		}

		clientdc->Rectangle(x1,y1,x2,y2);
		
		if(deselect){
			if(sb){
				clientdc->SelectObject(oldBrush);
			}
			if(sp){
				clientdc->SelectObject(oldPen);
			}
		}
	}

	static void drawRectangleW(CWnd* wnd,int x1, int y1, int x2, int y2, CBrush* br, CPen* pn){
		CClientDC p(wnd);
		p.SelectObject(br);
		p.SelectObject(pn);
		p.Rectangle(x1,y1,x2,y2);
	}

	static void drawLine(CClientDC* p, int x1, int y1, int x2, int y2){
		CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);
		clientdc->MoveTo(x1,y1);
		clientdc->LineTo(x2,y2);
	}

	static bool drawLine(CClientDC* p, int x1, int y1, int x2, int y2, DrawSets* DS, int ind, int status, bool deselect = false){
		DrawSets* curDS = (DS == nullptr ? drawSets : DS);
		if(ind<0 /*|| ind >= curDS->getPenSetsSize()*/){
			return false;
		}
		else{
			CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);

			CPen* oldPen = clientdc->SelectObject(curDS->getPenSet(ind)->getPenSafe(status));
			clientdc->MoveTo(x1,y1);
		    clientdc->LineTo(x2,y2);

			if(deselect){
				clientdc->SelectObject(oldPen);
			}

			return true;
		}
	}

	static bool drawLineW(CWnd* wnd,int x1, int y1, int x2, int y2, DrawSets* DS, int ind, int status){
		DrawSets* curDS = (DS == nullptr ? drawSets : DS);		
		if(ind<0 /*|| ind >= curDS->getPenSetsSize()*/){
			return false;
		}
		else{
			CClientDC p(wnd);
			p.SelectObject(curDS->getPenSet(ind)->getPenSafe(status));
			p.MoveTo(x1,y1);
		    p.LineTo(x2,y2);
			return true;
		}
	}

	static void drawLine(CClientDC* p, int x1, int y1, int x2, int y2, CPen* pn, bool deselect = false){
		    CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);
			CPen* oldPen = clientdc->SelectObject(pn);
		    clientdc->MoveTo(x1,y1);
		    clientdc->LineTo(x2,y2);

			if(deselect){
				clientdc->SelectObject(oldPen);
			}
	}

	static void drawLineW(CWnd* wnd,int x1, int y1, int x2, int y2, CPen* pn){
			CClientDC p(wnd);
			p.SelectObject(pn);
			p.MoveTo(x1,y1);
		    p.LineTo(x2,y2);
	}

	static void drawText(CClientDC* p, int x1, int y1, int x2, int y2, CFont* font, string str, Clr textColor, bool deselect = false){
		 CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);

		 clientdc->SetBkMode(TRANSPARENT);
		 clientdc->SetTextColor(RGB(textColor.r, textColor.g, textColor.b));

		 CFont* oldFont = clientdc->SelectObject(font);

		 CString str1(str.c_str());
		 clientdc->DrawText(str1, CRect(x1, y1, x2, y2), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		 if(deselect){
			clientdc->SelectObject(oldFont);
		 }
	}

	static void drawPixel(CClientDC* p, int x, int y, COLORREF clr){
		CDC* clientdc = memDC != nullptr ? memDC : (CDC*)(p != nullptr ? p : cdc);

		clientdc->SetPixelV(x, y, clr);
	}

};

#endif