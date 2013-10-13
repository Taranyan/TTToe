#ifndef BASECELL_H
#define BASECELL_H

#include "Object.h"

class BaseCell : public Object{
public:
	static const int MODE_PLAYER = 1;
	static const int MODE_PC = 2;
	static const int MODE_EMPTY = 0;

	BaseCell(){
	}
};

#endif