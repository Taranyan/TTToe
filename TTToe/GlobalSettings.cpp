#include "stdafx.h"
#include "GlobalSettings.h"

bool GlobalSettings::doubleBuffering = true;
Clr GlobalSettings::backgroundClr = Clr(255,255,255);
int GlobalSettings::menuWidth = 0;
size_t GlobalSettings::gameLevel = 0;
size_t GlobalSettings::settingsInd = 0;
AISettings GlobalSettings::aiSettings;