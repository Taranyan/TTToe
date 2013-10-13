#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include "Primitives.h"

class GlobalSettings{
public:
	static bool doubleBuffering;
	static Clr backgroundClr;
	static int menuWidth;
	static size_t gameLevel;
	static AISettings aiSettings;
	static size_t settingsInd;
};

#endif