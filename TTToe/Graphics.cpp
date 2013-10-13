#include "stdafx.h"
#include "Graphics.h"

CClientDC* Graphics::cdc = nullptr;
DrawSets* Graphics::drawSets = nullptr;
CMemmDC* Graphics::memDC = nullptr;
int Graphics::memDCCreations = 0;

