#ifndef UTILS_H
#define UTILS_H

#include "Primitives.h"

template<class A, class B>
class myPair{
public:
	A key;
	B value;
	
	myPair(){};
	myPair(A a, B b){
		key = a;
		value = b;
	}
};

template<class C, class D>
class UnorderedMapConverter{
public:
 static vector<myPair<C,D>> getPairVectorFromBoostUnorderedMap(unordered_map<C,D>* map){
	vector<myPair<C,D>> vec;

	unordered_map<C,D>::iterator it = map->begin();

	while(it != map->end()){
		vec.push_back(myPair<C,D>( (*it).first, (*it).second));
		it++;
	}

	return vec;
 }
};

class Utils{
public:
	static CRect getContainingRect(CRect r1, CRect r2){
		int left = r1.left < r2.left ? r1.left : r2.left;
		int top = r1.top < r2.top ? r1.top : r2.top;
		int right = r1.right > r2.right ? r1.right : r2.right;
		int bottom = r1.bottom > r2.bottom ? r1.bottom : r2.bottom;

		return CRect(left, top, right, bottom);
	}

	static CRect getContainingRect(Point ul1, Point br1, Point ul2, Point br2){
		return getContainingRect(CRect(ul1.x, ul1.y, br1.x, br1.y), CRect(ul2.x, ul2.y, br2.x, br2.y));
	}
};

#endif