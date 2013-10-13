#ifndef BASEFIELD_H
#define BASEFIELD_H

#include "Object.h"
#include "Cell.h"

class BaseField : public Object{
protected:
	Cell** cells;
public:
	Cell** getCells(){
		return cells;
	}

	void display(bool renderUpdatedCellsOnly = true, bool renderUpdatedSubCellsOnly = true, bool renderFrame = false, bool renderLines = false){
	}
	void animateCell(Point pt){}
};


#endif