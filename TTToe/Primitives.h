#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "stdafx.h"
#include <cmath>
#include <vector>
#include "Object.h"
#include <boost/unordered_map.hpp>

using boost::unordered_map;
using std::vector;

class Point : public Object
{
public:
	int x;
	int y;
	int z;
	
	bool valid;

	Point(int x = 0, int y = 0, int z = 0){
		this->x = x;
		this->y = y;
		this->z = z;

		valid = true;
	}

	Point(const Point& p){
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;

		this->valid = p.valid;
	}

	Point(const CPoint& p){
		this->x = p.x;
		this->y = p.y;
		this->z = 0;

		valid = true;
	}

	const Point& operator = (const Point& p){
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;

		this->valid = p.valid;

		return *this;
	}

	const Point& operator = (const CPoint& p){
		this->x = p.x;
		this->y = p.y;
		this->z = 0;

		valid = true;

		return *this;
	}

	const double operator - (const Point& p){
		return sqrt ( (double)((x-p.x)*(x-p.x) + (y-p.x)*(y-p.y) + (z-p.z)*(z-p.z)) );
	}

	bool operator == (const Point& p) const {
		return this->x == p.x && this->y == p.y && this->z == p.z;
	}

	bool operator != (const Point& p) const {
		return this->x != p.x || this->y != p.y || this->z != p.z;
	}

	bool getIsValid(){
		return valid;
	}

	void setIsValid(bool v){
		this->valid = v;
	}

	Point shift(int sx, int sy, int sz = 0){
		this->x += sx;
		this->y += sy;
		this->z += sz;

		return *this;
	}
};

class TwoPoints : public Object{
public:
	Point point1;
	Point point2;
};

class Settings : public Object{

public:

	int fieldSize;       // Size of the field(fieldSize*fieldSize cells)  
    int combLength;      // Lenght of sequence needed to win.
	int fieldULX;        // X coordinate of upper left corner of the field
	int fieldULY;        // Y coordinate of upper left corner of the field
	int distCellHalf;    // The Half of Distance between cells
	int numOfSubCells;   // Number of subcells in each cell
	int distSub;         // Distance between subcells
	int posnum;          // Number of positions
	int* subCellSizes;   // Sizes
	bool grow;           // Is the "grow" mode activated?
	bool convex;         // Is convex?
	bool showtrack;      // Is the track of mouse shown
	bool frameExists;    // Does the field have a frame
	int frameMargin;     // Margin of frame from the field
	int frameWidth;      // The width of the frame
	int frameColPos;     // Position of corresponding color in the drawsets(PenSet)
	bool separatingLinesExist; // Indicates whether lines between cells exist
	int separatingLineWidth;
	int separatingLineIndex;
    size_t clrSchemeInd;    // Index in the DS	
	int cellMonochrome;
	size_t fieldBackgroundIndex;
	size_t backgroundIndex; // Index of the backgound penset/brushset
	size_t xIndex;          // Index of the x penset/brushset
	size_t oIndex;          // Index of the o penset/brushset
	size_t emptyIndex;      // Index of the empty penset/brushset
	size_t fieldStyleInd;

	int getFieldBackgroundIndex(){
		return frameExists ? fieldBackgroundIndex : backgroundIndex;
	}

	Settings(){
		subCellSizes = nullptr;
	}

	void setSizes(int min, int max){
		if(subCellSizes != nullptr){
			for(int i = 0; i < posnum; i++){
				subCellSizes[i] = min + (max-min)*i/(posnum-1);
			}
		}
	}

	Settings(const Settings& sett){
		subCellSizes = nullptr;
		init(sett);
	}

	~Settings(){
		delete []subCellSizes;
	}

	const Settings& operator = (const Settings& sett){
		init(sett);
		return *this;
	}

	void init(const Settings& sett){
		this->fieldSize = sett.fieldSize;
		this->combLength = sett.combLength;
		this->fieldULX = sett.fieldULX;
		this->fieldULY = sett.fieldULY;
		this->distCellHalf = sett.distCellHalf;
		this->numOfSubCells = sett.numOfSubCells;
		this->distSub = sett.distSub;

		if(subCellSizes != nullptr){
			delete []subCellSizes;
		}

		this->posnum = sett.posnum;
		this->subCellSizes = new int[posnum];

		for(int i = 0; i < posnum; i++){
			this->subCellSizes[i] = sett.subCellSizes[i];
		}

		this->grow = sett.grow;
		this->convex = sett.convex;
		this->frameExists = sett.frameExists;
		this->frameMargin = sett.frameMargin;
		this->frameWidth = sett.frameWidth;
		this->frameColPos = sett.frameColPos;	
		this->separatingLinesExist = sett.separatingLinesExist;
		this->separatingLineWidth = sett.separatingLineWidth;
		this->separatingLineIndex = sett.separatingLineIndex;
		this->clrSchemeInd = sett.clrSchemeInd;
		this->cellMonochrome = sett.cellMonochrome;
		this->fieldBackgroundIndex = sett.fieldBackgroundIndex;
		this->backgroundIndex = sett.backgroundIndex;
		this->xIndex = sett.xIndex;
		this->oIndex = sett.oIndex;
		this->emptyIndex = sett.emptyIndex;
		this->fieldStyleInd = sett.fieldStyleInd;
	}
};

class Clr : public Object
{
public:
	int r,g,b;

	Clr(Clr& c)
	{
		this->operator=(c);
	}

    Clr(int R=0, int G=0, int B=0)
	{
	   set(R, G, B);
	}

	Clr(COLORREF cf)
	{
		set(GetRValue(cf), GetGValue(cf), GetBValue(cf));
	}

	COLORREF getRGB(){
		return RGB(r, g, b);
	}

	void set(int R, int G, int B)
	{
	   r=R;
	   g=G;
	   b=B;
	}

	void shift(int SR, int SG, int SB, int minR = 0, int maxR = 255, int minG = 0, int maxG = 255, int minB = 0, int maxB = 255)
	{
		this->r = shiftComponent(this->r, SR, minR, maxR);
		this->g = shiftComponent(this->g, SG, minG, maxG);
		this->b = shiftComponent(this->b, SB, minB, maxB);
	}

	void shift(int shift)
	{
		this->r = shiftComponent(this->r, shift);
		this->g = shiftComponent(this->g, shift);
		this->b = shiftComponent(this->b, shift);
	}

	const Clr& operator = (const Clr& clr)
	{ 
		if(&clr != this){
			this->r = clr.r;
			this->g = clr.g;
			this->b = clr.b;
		}
		return *this;
	}

	const bool operator == (const Clr& c)
	{
		return r == c.r && g == c.g && b == c.b; 
	}

    static int shiftComponent(int comp, int shift, int min = 0, int max = 255)
	{
		if(comp + shift > min)
		{
			if(comp + shift < max)
			{
				return comp + shift;
			}
			else
			{
				return max;
			}
		}
		else
		{
			return min;
		}
	}
};

class FieldStyle : public Object{
public:
	int bgColorInd;
	int frameClrInd;
	int playerClrInd;
	int aiClrInd;
	int emptyClrInd;

	FieldStyle(){}

	FieldStyle(const FieldStyle& st){
		this->operator=(st);
	}

	const FieldStyle& operator = (const FieldStyle& st){
		this->bgColorInd = st.bgColorInd;
		this->frameClrInd = st.frameClrInd;
		this->playerClrInd = st.playerClrInd;
		this->aiClrInd = st.aiClrInd;
		this->emptyClrInd = st.emptyClrInd;

		return *this;
	}

	FieldStyle(int bgColorInd, int frameClrInd, int playerClrInd, int aiClrInd, int emptyClrInd){
		this->bgColorInd = bgColorInd;
		this->frameClrInd = frameClrInd;
		this->playerClrInd = playerClrInd;
		this->aiClrInd = aiClrInd;
		this->emptyClrInd = emptyClrInd;
	}
};

class GameLevel : public Object{
public:
	size_t depth;
	size_t baseSelNum;
	size_t selNum;

	GameLevel(){}

	GameLevel(const GameLevel& gl){
		this->operator=(gl);
	}

	GameLevel(int depth, int baseSelNum, int selNum){
		this->depth = depth;
		this->baseSelNum = baseSelNum;
		this->selNum = selNum;
	}

	const GameLevel& operator=(const GameLevel& gl){
		this->depth = gl.depth;
		this->baseSelNum = gl.baseSelNum;
		this->selNum = gl.selNum;

		return *this;
	}
};

class AISettings : public Object{
public:
	int winCombLength;
	GameLevel level;
	vector<double> coeficients;
	double randDif;
	//double difDirCoef;

	AISettings(){

	}

	AISettings(const AISettings& ais){
		this->operator=(ais);
	}

	void operator=(const AISettings& ais){
		this->winCombLength = ais.winCombLength;
		this->level = ais.level;
		this->coeficients = ais.coeficients;
		this->randDif = ais.randDif;
	}

	~AISettings(){
	}
};


class ButtonStyle : public Object{
public:
	Clr startColor;
	Clr finalColor;
	Clr startColorFrame;
	Clr finalColorFrame;
	int finalStatus;
	int highlightStatus;
	Clr textColor;

	ButtonStyle(){};

	ButtonStyle(const ButtonStyle& bs){
		this->operator=(bs);
	}

	const ButtonStyle& operator=(const ButtonStyle& bs){
		this->startColor = bs.startColor;
		this->finalColor = bs.finalColor;
		this->startColorFrame = bs.startColorFrame;
		this->finalColorFrame = bs.finalColorFrame;
		this->finalStatus = bs.finalStatus;
		this->highlightStatus = bs.highlightStatus;
		this->textColor = bs.textColor;

		return *this;
	}
};

class SwingButtonStyle : public Object{
public:
	Clr labelBGColor;
	bool labelBGTransparent;
	bool labelExists;
	Clr labelColor;
	ButtonStyle* buttonStyle;

	SwingButtonStyle(){
		labelExists = true;
	}

	SwingButtonStyle(const SwingButtonStyle& sbs){
		this->operator=(sbs);
	}

	COLORREF getLabelBGColorRGB(){
		return labelBGColor.getRGB();
	}

	COLORREF getLabelColorRGB(){
		return labelColor.getRGB();
	}

	const SwingButtonStyle& operator=(const SwingButtonStyle& sbs){
		this->labelBGColor = sbs.labelBGColor;
		this->labelBGTransparent = sbs.labelBGTransparent;
		this->labelExists = sbs.labelExists;
		this->labelColor = sbs.labelColor;
		this->buttonStyle = sbs.buttonStyle;

		return *this;
	}
};

class MenuStyle : public Object{
public:
	Clr bGColor;
	Clr labelColor;
	bool labelBGTransparent;
	Clr labelBGColor;
	ButtonStyle* openButtonStyle;
	SwingButtonStyle* navigationButtonStyle;

	int openVo;
	int closeVo;
	int openA;
	int closeA;

	double reflectionCoef;
	int maxStickSpeed;

	int labelHeight;
	int elementsMarginTop;
	int panelWidthUnderNavButton;
	int elementsMarginLeft;
	int marginBetweenElements;

	COLORREF getBGColorRGB(){
		return bGColor.getRGB();
	}
};


class PenObject : public Object
{
public:
	CPen* pen;

	PenObject(){
		pen = new CPen();
	}

	PenObject(int penStyle, int width, COLORREF crColor){
		pen = new CPen(penStyle, width, crColor);
	}

	PenObject(CPen& p){
		pen = new CPen();
		setPen(pen,&p,true);
	}

	PenObject(const PenObject& p){
		pen = new CPen();
		setPen(pen, p.getPen(), true);
	}

	~PenObject(){
		delete pen;
	}

	CPen* getPen() const
	{
		return this->pen;
	}

	void setPen(CPen& p){
		setPen(pen, &p, true);
	}

	const PenObject& operator = (const PenObject& p){
		setPen(pen, p.getPen(), true);
		return *this;
	}
	
	const PenObject& operator = (CPen& p){
		setPen(pen, &p, true);
		return *this;
	}

	void setPen(CPen* first, CPen* second, boolean del){
		LOGPEN lp;
		second->GetLogPen(&lp);

		if(del){
			first->DeleteObject();
		}
		first->CreatePen(lp.lopnStyle, lp.lopnWidth.x, lp.lopnColor);
	}

	COLORREF getRGB(){
		LOGPEN lp;
		pen->GetLogPen(&lp);
		return lp.lopnColor;
	}
};

class BrushObject : public Object
{
public:
	CBrush* brush;

	BrushObject(){
		brush = new CBrush();
	}

	BrushObject(COLORREF crColor){
		brush = new CBrush(crColor);
	}

	BrushObject(CBrush& b){
		brush = new CBrush();
		setBrush(brush,&b,true);
	}

	BrushObject(const BrushObject& b){
		brush = new CBrush();
		setBrush(brush, b.getBrush(), true);
	}

	~BrushObject(){
		delete brush;
	}

	CBrush* getBrush() const
	{
		return this->brush;
	}

	void setBrush(CBrush& b){
		setBrush(brush, &b, true);
	}

	const BrushObject& operator = (const BrushObject& b){
		setBrush(brush, b.getBrush(), true);
		return *this;
	}
	
	const BrushObject& operator = (CBrush& b){
		setBrush(brush, &b, true);
		return *this;
	}

	void setBrush(CBrush* first, CBrush* second, bool del){
		LOGBRUSH lb;
	    second->GetLogBrush(&lb);
		if(del){
			first->DeleteObject();
		}
	    first->CreateSolidBrush(lb.lbColor);
	}

	COLORREF getRGB(){
		LOGBRUSH lb;
		brush->GetLogBrush(&lb);
		return lb.lbColor;
	}
};


class BrushSet : public Object
{
private:
	vector<BrushObject> brushes;
public:

	BrushSet()
	{
	}
	
	BrushSet(const BrushSet& bs)
	{
		//this->getsize = bs.size;
		//brushes = new CBrush[this->size];
		//for(int i = 0; i < this->size; i++)
		//{
		//	LOGBRUSH lb;
		//	bs.getBrush(i)->GetLogBrush( &lb );
		//	brushes[i].DeleteObject();
		//	brushes[i].CreateSolidBrush(lb.lbColor);
		//}
		this->brushes = bs.getBrushes();
	}
	
	BrushSet(int size, Clr* colors)
	{
		for(int i = 0; i < this->getSize(); i++)
		{
			BrushObject br = BrushObject( RGB( colors[i].r, colors[i].g, colors[i].b ));
			brushes.push_back(br);
		}
	}

	~BrushSet(){
	}
	
	int getSize(){
		return brushes.size();
    }

	vector<BrushObject> getBrushes() const{
		return brushes;
	}

	void setBrushes(vector<BrushObject> brushes){
		this->brushes = brushes;
	}

    CBrush* getBrush(int ind)
	{
		if(ind >= 0 && ind < getSize()){
			return brushes[ind].getBrush();
		}
		return nullptr;
	}

	BrushObject* getBrushObject(int ind)
	{
		if(ind>= 0 && ind<getSize()){
			return &brushes[ind];
		}
		return nullptr;
	}

	CBrush* getBrushSafe(int ind){
		if(ind > getSize()){
			return brushes[getSize()-1].getBrush();
		}
		else if(ind >= 0){
			return brushes[ind].getBrush();
		}
		return brushes[0].getBrush();
	}

	BrushObject* getBrushObjectSafe(int ind){
		if(ind > getSize()){
		   return &brushes[getSize()-1];
		}
		else if(ind >= 0){
		   return &brushes[ind];
		}
		return &brushes[0];
	}

	void setBrush(CBrush* first, CBrush* second){
		LOGBRUSH lb;
	    second->GetLogBrush(&lb);
	    first->DeleteObject();
	    first->CreateSolidBrush(lb.lbColor);
	}
	
	void setBrush(int ind, BrushObject& br)
	{
		if(ind >= 0 && ind < getSize()){
			//brushes[ind] = br;
			//setBrush(&brushes[ind], &br);
			brushes[ind] = br;
		}
	}

	void addBrush(BrushObject& brush){
		brushes.push_back(brush);
	}

	void popBack(){
		brushes.pop_back();
	}

	void clear(){
		brushes.clear();
	}
	
	const BrushSet& operator = (BrushSet& bs){
		if(this != &bs){
			this->brushes = bs.getBrushes();
		}
		return *this;
	}

	void shift(int shift)
	{
		for(int i = 0; i<getSize(); i++){
			Clr clr = getRGB(i);
			clr.shift(shift);
			set(i,clr);
		}
	}

	Clr getRGB(int i)
	{
		if(i >= 0 && i < getSize())
		{
		   LOGBRUSH lb;
		   brushes[i].getBrush()->GetLogBrush(&lb);
		   return Clr(GetRValue(lb.lbColor), GetGValue(lb.lbColor), GetBValue(lb.lbColor));
		}
		return Clr();
	}
	
	void set(int size, Clr* clrs)
	{
		brushes.clear();
		
		for(int i = 0; i< size; i++)
		{
			set(i, clrs[i]);
		}
	}

	void set(int ind, BrushObject &br)
	{
		if(ind >= 0 && ind < getSize())
		{
			brushes[ind] = br;
		}
	}

	void set(int ind, Clr cl)
	{
		if(ind >= 0 && ind < getSize())
		{
			brushes[ind] = BrushObject(RGB(cl.r,cl.g,cl.b));
		}
	}
	
	void fill(Clr stClr, Clr endClr, int size){
		brushes.clear();

		int r,g,b;

		bool rFB = (stClr.r > endClr.r);
		bool gFB = (stClr.g > endClr.g);
		bool bFB = (stClr.b > endClr.b);

		if(size != 1){
			for(int i = 0; i < size; i++){
				if(rFB){
					r = stClr.r + (endClr.r - stClr.r)*i/(size-1);
				}
				else{
					r = stClr.r - (stClr.r - endClr.r)*i/(size-1);
				}
				if(rFB){
					g = stClr.g + (endClr.g - stClr.g)*i/(size-1);
				}
				else{
					g = stClr.g - (stClr.g - endClr.g)*i/(size-1);
				}
				if(rFB){
					b = stClr.b + (endClr.b - stClr.b)*i/(size-1);
				}
				else{
					b = stClr.b - (stClr.b - endClr.b)*i/(size-1);
				}

				this->addBrush(BrushObject(RGB(r, g, b)));
			}
		}
		else{
			this->addBrush(BrushObject(RGB(stClr.r, stClr.g, stClr.b)));
		}
	}

};

class PenSet : public Object
{
private:
	vector<PenObject> pens;
public:

	PenSet()
	{
	}

	PenSet(const PenSet& ps)
	{
		this->pens = ps.getPens();
	}

	PenSet(int size, Clr* colors, int width)
	{
		int* widths = new int[1];
		widths[0] = width;
		this->PenSet::PenSet(size, colors, widths, true);
	}
	
	PenSet(int size, Clr* colors, int* widths, bool sameWidth = false)
	{
		for(int i = 0; i < size; i++)
		{
			int width;
			if(sameWidth){
				width = widths[0];
			}
			else{
				width = widths[i];
			}
			pens.push_back(PenObject(0, width, RGB( colors[i].r, colors[i].g, colors[i].b)));
		}
	}

	~PenSet()
	{
	}

	int getSize()
	{
		return pens.size();
	}

	CPen* getPen(int ind){
		if(ind < getSize() && ind >= 0){
			return pens[ind].getPen();
		}
		return nullptr;
	}

    PenObject* getPenObject(int ind)
	{
		if(ind < getSize() && ind >= 0){
			return &pens[ind];
		}
		return nullptr;
	}

	CPen* getPenSafe(int ind){
		if(ind > getSize()){
			return pens[getSize()-1].getPen();
		}
		else if(ind >= 0){
			return pens[ind].getPen();
		}
		return pens[0].getPen();
	}

	PenObject* getPenObjectSafe(int ind){
		if(ind > getSize()){
		   return &pens[getSize()-1];
		}
		else if(ind >= 0){
		   return &pens[ind];
		}
		return &pens[0];
	}

	void addPen(PenObject& pen){
		pens.push_back(pen);
	}

	void popBack(){
		pens.pop_back();
	}

	void clear(){
		pens.clear();
	}
	
	void setPen(CPen* first, CPen* second){
		LOGPEN lp;
		second->GetLogPen(&lp);
		first->DeleteObject();
		first->CreatePen(lp.lopnStyle, lp.lopnWidth.x, lp.lopnColor);
	}

	void setPen(int ind, CPen& pen)
	{
		if(ind >= 0 && ind < getSize())
		{
			pens[ind] = pen;
		}
	}

	vector<PenObject> getPens() const{
		return pens;
	}


	const PenSet& operator = (PenSet& ps)
	{
		if(this != &ps){
			this->pens = ps.getPens();
		}
		return *this;
	}
	

	Clr getRGB(int i)
	{
		if( i >= 0 &&  i < getSize())
		{
		   LOGPEN lp;
		   pens[i].getPen()->GetLogPen(&lp);
		   return Clr(GetRValue(lp.lopnColor), GetGValue(lp.lopnColor), GetBValue(lp.lopnColor));
		}
		return Clr();
	}

	int getWidth(int i){
		if( i >= 0 && i < getSize())
		{
		   LOGPEN lp;
		   pens[i].getPen()->GetLogPen(&lp);
		   return lp.lopnWidth.x;
		}
		return -1;
	}
	
	void set(int size, Clr* colors, int width)
	{
		int* widths = new int[1];
		widths[0] = width;
		set(size,colors,widths,true);
	}

	void set(int size, Clr* clrs, int* widths, bool same = false)
	{
		for(int i = 0; i< size; i++)
		{
			int w;
			if(same){
				w = widths[0];
			}
			else{
				w = widths[i];
			}
			pens.push_back( PenObject( 0, w, RGB(clrs[i].r, clrs[i].g, clrs[i].b) ));
		}
	}

	void set(int ind, Clr c, int width = 1){
		if(ind >= 0 && ind < getSize()){
			pens[ind] = PenObject(0, width, RGB(c.r, c.g, c.b));
		}
	}

    void set(int size, CPen* pn)
	{
		for(int i = 0; i < size; i++)
		{
			pens.clear();
			pens.push_back(pn[i]);

		}

	}

	void setFromBrushSet(BrushSet& BS,int width)
	{
		pens.clear();

		for(int i = 0; i < BS.getSize(); i++)
		{
			LOGBRUSH lb;
			BS.getBrush(i)->GetLogBrush(&lb);
			pens.push_back(PenObject(0, width, lb.lbColor));	 
		}
	}

	void shift(int shift)
	{
		for(int i = 0; i < this->getSize(); i++)
		{
			LOGPEN lp;
			pens[i].getPen()->GetLogPen(&lp);
			Clr clr(lp.lopnColor);
			clr.shift(shift);
			set(i, clr, lp.lopnWidth.x);
		}
	}

	void fill(Clr stClr, Clr endClr, int size, bool clear = true){
		if(clear){
			pens.clear();
		}
		int r,g,b;

		bool rFB = (stClr.r > endClr.r);
		bool gFB = (stClr.g > endClr.g);
		bool bFB = (stClr.b > endClr.b);

		if(size != 1){
			for(int i = 0; i < size; i++){
				if(rFB){
					r = stClr.r + (endClr.r - stClr.r)*i/(size-1);
				}
				else{
					r = stClr.r - (stClr.r - endClr.r)*i/(size-1);
				}
				if(rFB){
					g = stClr.g + (endClr.g - stClr.g)*i/(size-1);
				}
				else{
					g = stClr.g - (stClr.g - endClr.g)*i/(size-1);
				}
				if(rFB){
					b = stClr.b + (endClr.b - stClr.b)*i/(size-1);
				}
				else{
					b = stClr.b - (stClr.b - endClr.b)*i/(size-1);
				}
				this->addPen(PenObject(0, 1, RGB(r, g, b)));
			}
		}
		else{
			this->addPen(PenObject(0, 1, RGB(stClr.r, stClr.g, stClr.b)));
		}
	}

	void fill2(Clr stClr,Clr middleClr1, Clr middleClr2, Clr endClr, int middleSize, int size){
		pens.clear();
		fill(stClr, middleClr1, middleSize, false);
		fill(middleClr2, endClr, size-middleSize, false);
	}
};


class DrawSets : public Object
{
private:
	unordered_map<int, BrushSet> brushSets;
	unordered_map<int, PenSet> penSets;

public:
	~DrawSets()
	{
	}

	DrawSets()
	{
	}

	unordered_map<int, BrushSet> getBrushSets(){
		return this->brushSets;
	}

	unordered_map<int, PenSet> getPenSets(){
		return this->penSets;
	}

	boolean setBrushSet(int ind, BrushSet& bs)
	{
		if(ind >= 0 /*&& (unsigned int)ind < brushSets.size()*/){
			brushSets[ind] = bs;
		    //brushSets.insert(it + ind, bs);
			return true;
		}
		return false;
	}

	BrushSet* getBrushSet(int ind)
	{
		if(ind >=0 /*&& (unsigned int)ind < brushSets.size()*/){
			return &brushSets[ind];
		}
		return nullptr;
    }


	bool setPenSet(int ind, PenSet& ps)
	{
		if(ind >= 0 /*&& (unsigned int)ind < penSets.size()*/){
			penSets[ind] = ps;
			//penSets.insert(it + ind, ps);
			return true;
		}
		return false;
	}

	PenSet* getPenSet(int ind)
	{
		if(ind >= 0 /*&& (unsigned int)ind < penSets.size()*/){
			return &penSets[ind];
		}
		return nullptr;
    }

	const DrawSets& operator = (DrawSets& ds){
		if(this != &ds){
			this->brushSets = ds.getBrushSets();
			this->penSets = ds.getPenSets();
		}
		return *this;
	}
};

#endif