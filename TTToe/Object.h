#ifndef OBJECT_H
#define OBJECT_H

#include "stdafx.h"

class Object : public CObject{
public:
	Object(){};
	virtual ~Object(){}

private:
	Object(const Object& obj){};
};

#endif