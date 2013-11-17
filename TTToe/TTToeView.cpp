#include "stdafx.h"
#include "TTToe.h"

#include "TTToeDoc.h"
#include "TTToeView.h"
#include <cmath>
#include "Primitives.h"
#include "Graphics.h"

#include "Cell.h"
#include "Field.h"
#include "MemDC.h"
#include <time.h>
#include "GlobalSettings.h"
#include "Button.h"
#include "SwingButton.h"
#include "Menu.h"
#include "FieldLineIterator.h"
#include "Utils.h"

#pragma comment(linker, "/STACK:16777216")
#pragma warning( disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTTToeView, CView)

BEGIN_MESSAGE_MAP(CTTToeView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

const int DSI_BACKGROUND_SETS		  = 0;
const int DSI_CELL_EMPTY_SETS		  = 1;
const int DSI_CELL_AI_SETS			  = 2;
const int DSI_CELL_USER_SETS		  = 3;
const int DSI_FIELD_BACKGROUND_SETS   = 4;
const int DSI_FIELD_FRAME_SETS		  = 5;

const int DSI_CELL_EMPTY_SETS_2       = 6;
const int DSI_CELL_AI_SETS_2          = 7;
const int DSI_CELL_USER_SETS_2        = 8;

const int DSI_FIELD_BACKGROUND_SETS_2 = 9;
const int DSI_FIELD_FRAME_SETS_2      = 10;

const int TMR_MENU_ID = 0;
const int TMR_FIELD_ID = 1;
const int TMR_MAIN_ID = 2;
const int TMR_MENU_ACT_SEQ_ID = 3;
const int TMR_WINNING_COMB_ID = 4;
const int TMR_STATUS_BAR_ID = 5;

const int TMR_TEST_ID = 6;

int TMR_MENU_FREQUENCY = 10;
int TMR_FIELD_FREQUENCY = 20;
int TMR_MAIN_FREQUENCY = 10;
int TMR_MENU_ACT_SEQ_FREQUENCY = 500;
int TMR_WINNING_COMB_FREQUENCY = 250;
int TMR_STATUS_BAR_FREQUENCY = 100;

int TMR_TEST_FREQUENCY = 20000;

const int MAX_FIELD_WIDTH = 800;
const int MAX_FIELD_HEIGHT = 720;

const int MAX_FIELD_SIZE = INT_MAX;
const int MIN_FIELD_SIZE = 5;

const int MIN_SUBCELL_SIZE = 1;

const int MIN_NUM_OF_SUBCELLS = 1;

const int MIN_NUM_OF_ROOT_CELLS = 1;
const int MAX_NUM_OF_ROOT_CELLS = 25;

const int MIN_NUM_OF_CELLS = 1;
const int MAX_NUM_OF_CELLS = 25;

const int MIN_DEPTH = 1;
const int MAX_DEPTH = 15;

bool menuSequenceOn = false;
int menuSequenceId = -1;

//NOTE: Convention - for the swing buttons the opposite actions have opposite ID's, so that I can easily check it by adding them and comparing with '0';

const int MENU_OPEN_ID = 0;

const int FSIZE_DECREASE = -1;
const int FSIZE_INCREASE = 1;
const int SUBCELLSIZE_DECREASE = -2;
const int SUBCELLSIZE_INCREASE = 2;
const int NUM_OF_SUBCELLS_DECREASE = -3;
const int NUM_OF_SUBCELLS_INCREASE = 3;
const int CHANGE_COLOR = 4;
const int CHANGE_GROWING = 5;
const int DECREASE_GAME_LEVEL = -6;
const int INCREASE_GAME_LEVEL = 6;
const int CHANGE_STYLE = 7;
const int DIST_CELL_DECREASE = -8;
const int DIST_CELL_INCREASE = 8;
const int CHANGE_DOUBLE_BUFFERING = 9;
const int SHOW_HIDE_FIELD_FRAME = 10;
const int SHOW_HIDE_SEP_LINES = 11;
const int DECREASE_BASE_SEL_NUM = -12;
const int INCREASE_BASE_SEL_NUM = 12;
const int DECREASE_DEPTH = -13;
const int INCREASE_DEPTH = 13;
const int DECREASE_SEL_NUM = -14;
const int INCREASE_SEL_NUM = 14;
const int SHOW_TRACK = 15;
const int MENU_NAV_LEFT_ID = -16;
const int MENU_NAV_RIGHT_ID = 16;

int sequenceValue = 0;

Point lastMove;
bool moveQueued = false;

int gameStatus = -1;

TwoPoints vicCombStartEnd;
bool vicAnimFirstTime = false;

Point aiMove;
bool moveMade = false;

vector<Combination> winningCombinations;

bool aiThinking = false;

DrawSets DS;
Settings settings;
Field* field;
FieldLineIterator iter;

bool isViewInitialized = false;

ButtonStyle* buttonStyle;
ButtonStyle* buttonStyle2;
ButtonStyle* buttonStyle3;
ButtonStyle* openButtonStyle;
SwingButtonStyle* menuNavButtonStyle;
SwingButtonStyle* swingButtonStyle;
MenuStyle* menuStyle;

SwingButton* swb_FieldSize;
SwingButton* swb_SubCellSize;
SwingButton* swb_SubCellNumber;
SwingButton* swb_GameLevel;
SwingButton* swb_CellDistance;
SwingButton* swb_RootCellNum;
SwingButton* swb_SearchDepth;
SwingButton* swb_CellNum;

Button* bt_Color;
Button* bt_Growing;
Button* bt_StylePreset;
Button* bt_DoubleBuffering;
Button* bt_FieldFrame;
Button* bt_FieldLines;
Button* bt_ShowTrack;

Button* openButton;
Menu* menu;
CRect prevRect;

vector<Object*> objectsToDelete;
vector<FieldStyle> fieldStyles;
vector<GameLevel> gameLevels;
vector<Settings> settingsVector;

bool testProperty = false;

int playerVicNumber = 0;
int drawNumber = 0;
int aiVictoryNumber = 0;

CWinThread* aiThread;
bool handleInitialized;

CTTToeView::CTTToeView()
{ 	
	aiThread = nullptr;
	handleInitialized = false;

	fieldStyles.push_back(FieldStyle(DSI_FIELD_BACKGROUND_SETS, DSI_FIELD_FRAME_SETS, DSI_CELL_USER_SETS, DSI_CELL_AI_SETS, DSI_CELL_EMPTY_SETS));
	fieldStyles.push_back(FieldStyle(DSI_FIELD_BACKGROUND_SETS_2, DSI_FIELD_FRAME_SETS_2, DSI_CELL_USER_SETS_2, DSI_CELL_AI_SETS_2, DSI_CELL_EMPTY_SETS_2));

	settings.emptyIndex = DSI_CELL_EMPTY_SETS;
	settings.xIndex = DSI_CELL_USER_SETS;
	settings.oIndex = DSI_CELL_AI_SETS;

	settings.showtrack = true;
	settings.convex = false;
	settings.distCellHalf = 1;
	settings.distSub = 0;
	settings.backgroundIndex = DSI_BACKGROUND_SETS;
	settings.fieldBackgroundIndex = DSI_FIELD_BACKGROUND_SETS;
	settings.fieldSize = 20;
	settings.fieldULX = 20;
	settings.fieldULY = 20;
	settings.frameExists = true;
	settings.frameColPos = DSI_FIELD_FRAME_SETS;
	settings.frameMargin = 2;
	settings.frameWidth = 1;
	settings.grow = false;
	settings.numOfSubCells = 1;
	settings.posnum = 12;
	settings.separatingLinesExist = true;
	settings.separatingLineWidth = 1;
	settings.separatingLineIndex = DSI_FIELD_FRAME_SETS; 
	int* sizes = new int[12];
	settings.subCellSizes = sizes;
	settings.setSizes(1, 16);
	settings.fieldStyleInd = 0;

	settings.cellMonochrome = false;

	settingsVector.push_back(settings);

	Settings settings2 = settings;
	settings2.fieldStyleInd = 1;

	settingsVector.push_back(settings2);

	Settings settings3 = settings;
	settings3.setSizes(1,5);
	settings3.numOfSubCells = 3;

	settingsVector.push_back(settings3);

	Settings settings4 = settings3;
	settings4.fieldStyleInd = 1;

	settingsVector.push_back(settings4);

	Settings settings5 = settings;
	settings5.grow = true;

	settingsVector.push_back(settings5);

	Settings settings6 = settings5;
	settings6.fieldStyleInd = 1;

	settingsVector.push_back(settings6);

	Settings settings7 = settings;
	settings7.numOfSubCells = 3;
	settings7.setSizes(1,5);
	settings7.grow = true;

	settingsVector.push_back(settings7);

    Settings settings8 = settings;
	settings8.fieldSize = 19;
	settings8.numOfSubCells = 16;
	settings8.setSizes(1,1);

	settingsVector.push_back(settings8);

    Settings settings9 = settings;

	settings9.fieldBackgroundIndex = DSI_BACKGROUND_SETS;
	settings9.frameExists = false;
	settings9.separatingLinesExist = false;
	settings9.fieldStyleInd = 1;

	settingsVector.push_back(settings9);

    Settings settings10 = settings9;
	
	settings10.distCellHalf = 0;

	settingsVector.push_back(settings10);

	Settings settings11 = settings10;
	
	settings11.grow = true;

	settingsVector.push_back(settings11);

	GlobalSettings::settingsInd = 0;
	GlobalSettings::aiSettings.winCombLength = 5;
    GlobalSettings::aiSettings.randDif = 30.0;
	GlobalSettings::aiSettings.level.depth = 7;
	GlobalSettings::aiSettings.level.baseSelNum = 10;
	GlobalSettings::aiSettings.level.selNum = 5;

	GlobalSettings::aiSettings.coeficients.push_back(0.0);
	GlobalSettings::aiSettings.coeficients.push_back(10000000000.0);
	GlobalSettings::aiSettings.coeficients.push_back(100000000.0);
	GlobalSettings::aiSettings.coeficients.push_back(1000000.0);
	GlobalSettings::aiSettings.coeficients.push_back(10000.0);
	GlobalSettings::aiSettings.coeficients.push_back(10.0);

	field = new Field();
	field->initialize(&settings, Point(settings.fieldULX, settings.fieldULY, 0), &DS);
	field->rateField->fld = field;	
	objectsToDelete.push_back(field);
	//field->setMode(Field::MODE_MOVE);

	Graphics::drawSets = &DS;

	Clr backgroundClr(255,255,255);

	GlobalSettings::backgroundClr = backgroundClr;
	GlobalSettings::menuWidth = 200;

	initializeControls();
	initializeDrawSet();

	GlobalSettings::gameLevel = 4;

	gameLevels.push_back(GameLevel(1,1,1));
	gameLevels.push_back(GameLevel(3,15,9));	
	gameLevels.push_back(GameLevel(5,10,5));
	gameLevels.push_back(GameLevel(5,15,9));
	gameLevels.push_back(GameLevel(7,10,5));
	gameLevels.push_back(GameLevel(7,15,9));
	gameLevels.push_back(GameLevel(9,10,5));

	srand((unsigned int)time(0));
}

CTTToeView::~CTTToeView()
{
   
}

void CTTToeView::initializeControls(void)
{	
	buttonStyle = new ButtonStyle();
	objectsToDelete.push_back(buttonStyle);

	buttonStyle->finalColor = Clr(215,62,0);
	buttonStyle->finalColorFrame = Clr(52,48,46);
	buttonStyle->startColor = Clr(62,56,52);
	buttonStyle->startColorFrame = Clr(180,180,180);
	buttonStyle->finalStatus = 16;
	buttonStyle->highlightStatus = 3;
	buttonStyle->textColor = Clr(150,150,150);

	buttonStyle2 = new ButtonStyle();
	objectsToDelete.push_back(buttonStyle2);

	buttonStyle2->finalColor = Clr(215,62,0);
	buttonStyle2->finalColorFrame = Clr(52,48,46);
	buttonStyle2->startColor = Clr(62,56,52);
	buttonStyle2->startColorFrame = Clr(180,180,180);
	buttonStyle2->finalStatus = 16;
	buttonStyle2->highlightStatus = 3;
	buttonStyle2->textColor = Clr(225,82,20);

	buttonStyle3 = new ButtonStyle();
	objectsToDelete.push_back(buttonStyle3);

	buttonStyle3->finalColor = Clr(215,62,0);
	buttonStyle3->finalColorFrame = Clr(52,48,46);
	buttonStyle3->startColor = Clr(42,37,34);
	buttonStyle3->startColorFrame = Clr(180,180,180);
	buttonStyle3->finalStatus = 16;
	buttonStyle3->highlightStatus = 3;
	buttonStyle3->textColor = Clr(225,82,20);

	openButtonStyle = new ButtonStyle();
	objectsToDelete.push_back(openButtonStyle);

	openButtonStyle->finalColor = Clr(120,120,120);
	openButtonStyle->finalColorFrame = Clr(100,100,100);
	openButtonStyle->startColor = Clr(42,37,34);
	openButtonStyle->startColorFrame = Clr(40,45,32);
	openButtonStyle->finalStatus = 50;
	openButtonStyle->highlightStatus = 22;
	openButtonStyle->textColor = Clr(215,62,0);

    string s("Empty");


	string l("<");
	string r(">");

	string easyStr("-");
	string hardStr("+");
	
	swingButtonStyle = new SwingButtonStyle();
	objectsToDelete.push_back(swingButtonStyle);

	swingButtonStyle->buttonStyle = buttonStyle;
	swingButtonStyle->labelBGColor = Clr(62,56,52);
	swingButtonStyle->labelBGTransparent = false;
	swingButtonStyle->labelColor = Clr(200,200,200);

	menuNavButtonStyle = new SwingButtonStyle();
	objectsToDelete.push_back(menuNavButtonStyle);

	menuNavButtonStyle->buttonStyle = buttonStyle3;
	menuNavButtonStyle->labelBGColor = Clr(62,56,52);
	menuNavButtonStyle->labelBGTransparent = false;
	menuNavButtonStyle->labelColor = Clr(200,200,200);
	menuNavButtonStyle->labelExists = false;

	//46/16/30
	swb_FieldSize = new SwingButton(FSIZE_DECREASE,FSIZE_INCREASE,Point(20,50),60,178,20,"",l,r,swingButtonStyle);
	objectsToDelete.push_back(swb_FieldSize);
	swb_SubCellSize = new SwingButton(SUBCELLSIZE_DECREASE,SUBCELLSIZE_INCREASE,Point(20,50),60,178,20,"",l,r,swingButtonStyle);
	objectsToDelete.push_back(swb_SubCellSize);
	swb_SubCellNumber = new SwingButton(NUM_OF_SUBCELLS_DECREASE,NUM_OF_SUBCELLS_INCREASE,Point(20,50),60,178,20,"",l,r,swingButtonStyle);
	objectsToDelete.push_back(swb_SubCellNumber);
	swb_GameLevel = new SwingButton(DECREASE_GAME_LEVEL,INCREASE_GAME_LEVEL,Point(20,50),60,178,20,"",easyStr,hardStr,swingButtonStyle);
	objectsToDelete.push_back(swb_GameLevel);
	swb_CellDistance = new SwingButton(DIST_CELL_DECREASE,DIST_CELL_INCREASE,Point(20,50),60,178,20,"",l,r,swingButtonStyle);
    objectsToDelete.push_back(swb_CellDistance);
	swb_RootCellNum = new SwingButton(DECREASE_BASE_SEL_NUM,INCREASE_BASE_SEL_NUM,Point(20,50),60,178,20,"",easyStr,hardStr,swingButtonStyle);
    objectsToDelete.push_back(swb_RootCellNum);
	swb_SearchDepth = new SwingButton(DECREASE_DEPTH,INCREASE_DEPTH,Point(20,50),60,178,20,"",easyStr,hardStr,swingButtonStyle);
    objectsToDelete.push_back(swb_SearchDepth);
	swb_CellNum = new SwingButton(DECREASE_SEL_NUM,INCREASE_SEL_NUM,Point(20,50),60,178,20,"",easyStr,hardStr,swingButtonStyle);
    objectsToDelete.push_back(swb_CellNum);

    bt_Color = new Button(CHANGE_COLOR,Point(0,0),40,178,"Colour",buttonStyle);
    objectsToDelete.push_back(bt_Color);
	bt_Growing = new Button(CHANGE_GROWING,Point(0,0),40,178,"",buttonStyle);
	objectsToDelete.push_back(bt_Growing);
	bt_StylePreset = new Button(CHANGE_STYLE,Point(0,0),40,178,string("Change Style!"),buttonStyle2);
	objectsToDelete.push_back(bt_StylePreset);
    bt_DoubleBuffering = new Button(CHANGE_DOUBLE_BUFFERING,Point(0,0),40,178,string(""),buttonStyle);
	objectsToDelete.push_back(bt_DoubleBuffering);
	bt_FieldFrame = new Button(SHOW_HIDE_FIELD_FRAME,Point(0,0),40,178,string(""),buttonStyle);
	objectsToDelete.push_back(bt_FieldFrame);
	bt_FieldLines = new Button(SHOW_HIDE_SEP_LINES,Point(0,0),40,178,string(""),buttonStyle);
	objectsToDelete.push_back(bt_FieldLines);
	bt_ShowTrack = new Button(SHOW_TRACK,Point(0,0),40,178,string(""),buttonStyle);
	objectsToDelete.push_back(bt_ShowTrack);
	
	menuStyle = new MenuStyle();
	objectsToDelete.push_back(menuStyle);

	menuStyle->bGColor = Clr(62,56,52);
	menuStyle->labelColor = Clr(210,210,210);
	menuStyle->labelBGTransparent = false;
	menuStyle->labelBGColor = Clr(42,37,34);
	menuStyle->labelHeight = 50;
	menuStyle->openButtonStyle = openButtonStyle;
	menuStyle->navigationButtonStyle = menuNavButtonStyle;
	menuStyle->elementsMarginLeft = 1;
	menuStyle->elementsMarginTop = 20;
	menuStyle->panelWidthUnderNavButton = 15;
	menuStyle->marginBetweenElements = 3;
	menuStyle->closeA = 10;
	menuStyle->closeVo = 0;
	menuStyle->openVo = 6;
	menuStyle->openA = - 1;
	menuStyle->maxStickSpeed = 6;
	menuStyle->reflectionCoef = 0.50;
}


void CTTToeView::initializeMenu(CRect rect)
{

	menu = new Menu(MENU_OPEN_ID, MENU_NAV_LEFT_ID, MENU_NAV_RIGHT_ID, Point(rect.right - GlobalSettings::menuWidth, 0), Point(rect.right - 20, 0), Point(rect.right - GlobalSettings::menuWidth, 0), Menu::MENU_OPENED, rect.Height(), GlobalSettings::menuWidth, 20, 20, 178, string("Options"), menuStyle); 
	objectsToDelete.push_back(menu);

	menu->insertList(vector<ControlElement*>());
	menu->insertList(vector<ControlElement*>());
	
	
	menu->insertElement(0,swb_FieldSize);
	menu->insertElement(0,swb_GameLevel);
	menu->insertElement(0,swb_RootCellNum);
	menu->insertElement(0,swb_SearchDepth);
	menu->insertElement(0,swb_CellNum);
	menu->insertElement(0,bt_DoubleBuffering);
	menu->insertElement(1,swb_CellDistance);
	menu->insertElement(1,swb_SubCellNumber);
	menu->insertElement(1,swb_SubCellSize);
	menu->insertElement(1,bt_Color);
	menu->insertElement(1,bt_Growing);
	menu->insertElement(1,bt_FieldFrame);
	menu->insertElement(1,bt_FieldLines);
	menu->insertElement(1,bt_StylePreset);
	menu->insertElement(1,bt_ShowTrack);

	refreshControlLabels();
}

void CTTToeView::initializeDrawSet()
{
	//--background--------------------------

	PenSet* bgPenSet = new PenSet();
	objectsToDelete.push_back(bgPenSet);

	bgPenSet->addPen(PenObject(0,1,GlobalSettings::backgroundClr.getRGB()));
	DS.setPenSet(DSI_BACKGROUND_SETS, *bgPenSet);

	BrushSet* bgBrushSet = new BrushSet();
	objectsToDelete.push_back(bgBrushSet);

	bgBrushSet->addBrush(BrushObject(GlobalSettings::backgroundClr.getRGB()));
	DS.setBrushSet(DSI_BACKGROUND_SETS, *bgBrushSet);
	//--field background----------------------
	PenSet* fieldBgPenSet = new PenSet();
	objectsToDelete.push_back(fieldBgPenSet);

	fieldBgPenSet->addPen(PenObject(0,1,RGB(20,20,20)));
	DS.setPenSet(DSI_FIELD_BACKGROUND_SETS, *fieldBgPenSet);

	BrushSet* fieldBgBrushSet = new BrushSet();
	objectsToDelete.push_back(fieldBgBrushSet);

	fieldBgBrushSet->addBrush(BrushObject(RGB(20,20,20)));
	DS.setBrushSet(DSI_FIELD_BACKGROUND_SETS, *fieldBgBrushSet);
	//--field frame---------------------------

	PenSet* fieldFramePenSet = new PenSet();
	objectsToDelete.push_back(fieldFramePenSet);

	fieldFramePenSet->addPen(PenObject(0,1,RGB(1,1,2)));
	DS.setPenSet(DSI_FIELD_FRAME_SETS, *fieldFramePenSet);

	BrushSet* fieldFrameBrushSet = new BrushSet();
	objectsToDelete.push_back(fieldFrameBrushSet);

	fieldFrameBrushSet->addBrush(BrushObject(RGB(1,1,2)));
	DS.setBrushSet(DSI_FIELD_FRAME_SETS, *fieldFrameBrushSet);

	//--field background2----------------------
	PenSet* fieldBgPenSet2 = new PenSet();
	objectsToDelete.push_back(fieldBgPenSet2);

	fieldBgPenSet2->addPen(PenObject(0,1,RGB(220,220,220)));
	DS.setPenSet(DSI_FIELD_BACKGROUND_SETS_2, *fieldBgPenSet2);

	BrushSet* fieldBgBrushSet2 = new BrushSet();
	objectsToDelete.push_back(fieldBgBrushSet2);

	fieldBgBrushSet2->addBrush(BrushObject(RGB(220,220,220)));
	DS.setBrushSet(DSI_FIELD_BACKGROUND_SETS_2, *fieldBgBrushSet2);
	//--field frame2---------------------------

	PenSet* fieldFramePenSet2 = new PenSet();
	objectsToDelete.push_back(fieldFramePenSet2);

	fieldFramePenSet2->addPen(PenObject(0,1,RGB(60,60,60)));
	DS.setPenSet(DSI_FIELD_FRAME_SETS_2, *fieldFramePenSet2);

	BrushSet* fieldFrameBrushSet2 = new BrushSet();
	objectsToDelete.push_back(fieldFrameBrushSet2);

	fieldFrameBrushSet2->addBrush(BrushObject(RGB(60,60,60)));
	DS.setBrushSet(DSI_FIELD_FRAME_SETS_2, *fieldFrameBrushSet2);


	//------------------------------------------------

	//--User------------------------------------------
	PenSet* userPenSet = new PenSet();
	objectsToDelete.push_back(userPenSet);

	userPenSet->fill2(Clr(70,35,35), Clr(225,115,115), Clr(185,100,100), Clr(55,20,20), settings.posnum/2 + 1, settings.posnum);

	DS.setPenSet(DSI_CELL_USER_SETS, *userPenSet);

	BrushSet* userBrushSet = new BrushSet();
	objectsToDelete.push_back(userBrushSet);

	userBrushSet->fill(Clr(150,10,10),Clr(255,115,115),settings.posnum);

	DS.setBrushSet(DSI_CELL_USER_SETS, *userBrushSet);

	//--ai---------------------------------------------

	PenSet* aiPenSet = new PenSet();
	objectsToDelete.push_back(aiPenSet);

	aiPenSet->fill2(Clr(35,35,70), Clr(115,115,225), Clr(100,100,185), Clr(20,20,55), settings.posnum/2 + 1, settings.posnum);

	DS.setPenSet(DSI_CELL_AI_SETS, *aiPenSet);

	BrushSet* aiBrushSet = new BrushSet();
	objectsToDelete.push_back(aiBrushSet);

	aiBrushSet->fill(Clr(10,10,150),Clr(115,115,255),settings.posnum);

	DS.setBrushSet(DSI_CELL_AI_SETS, *aiBrushSet);

	//--empty------------------------------------------

	BrushSet* emptyBrushSet = new BrushSet();
	objectsToDelete.push_back(emptyBrushSet);

	emptyBrushSet->fill(Clr(100,100,100),Clr(215,215,215),settings.posnum);

	DS.setBrushSet(DSI_CELL_EMPTY_SETS, *emptyBrushSet);

	PenSet* emptyPenSet = new PenSet();
	objectsToDelete.push_back(emptyPenSet);

	emptyPenSet->fill2(Clr(50,50,50), Clr(220,220,220), Clr(180,180,180), Clr(40,40,40), settings.posnum/2 + 1, settings.posnum);

	DS.setPenSet(DSI_CELL_EMPTY_SETS, *emptyPenSet);

	//--User2------------------------------------------
	PenSet* userPenSet2 = new PenSet();
	objectsToDelete.push_back(userPenSet2);

	userPenSet2->fill2(Clr(190,30,30), Clr(90,20,20), Clr(105,35,35), Clr(255,180,180), settings.posnum/2 + 1, settings.posnum);

	DS.setPenSet(DSI_CELL_USER_SETS_2, *userPenSet2);

	BrushSet* userBrushSet2 = new BrushSet();
	objectsToDelete.push_back(userBrushSet2);

	userBrushSet2->fill(Clr(235,40,40),Clr(130,15,15),settings.posnum);

	DS.setBrushSet(DSI_CELL_USER_SETS_2, *userBrushSet2);

	//--ai2---------------------------------------------

	PenSet* aiPenSet2 = new PenSet();
	objectsToDelete.push_back(aiPenSet2);

	aiPenSet2->fill2(Clr(30,30,190), Clr(20,20,90), Clr(35,35,105), Clr(180,180,255), settings.posnum/2 + 1, settings.posnum);

	DS.setPenSet(DSI_CELL_AI_SETS_2, *aiPenSet2);

	BrushSet* aiBrushSet2 = new BrushSet();
	objectsToDelete.push_back(aiBrushSet2);

	aiBrushSet2->fill(Clr(40,40,235),Clr(15,15,130),settings.posnum);

	DS.setBrushSet(DSI_CELL_AI_SETS_2, *aiBrushSet2);

	//--empty2------------------------------------------

	BrushSet* emptyBrushSet2 = new BrushSet();
	objectsToDelete.push_back(emptyBrushSet2);

	emptyBrushSet2->fill(Clr(165,165,165),Clr(80,80,80),settings.posnum);

	DS.setBrushSet(DSI_CELL_EMPTY_SETS_2, *emptyBrushSet2);

	PenSet* emptyPenSet2 = new PenSet();
	objectsToDelete.push_back(emptyPenSet2);

	emptyPenSet2->fill2(Clr(175,175,175), Clr(60,60,60), Clr(75,75,75), Clr(215,215,215), settings.posnum/2 + 1, settings.posnum);
	
	DS.setPenSet(DSI_CELL_EMPTY_SETS_2, *emptyPenSet2);
}


BOOL CTTToeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTTToe3View drawing

void CTTToeView::OnDraw(CDC* pDC)
{
	if(Graphics::cdc == nullptr){
			Graphics::cdc = new CClientDC(this);
	}

	CRect CWRect;
	GetClientRect(CWRect);

	if(!isViewInitialized){
		(AfxGetMainWnd( ))->SetWindowText(L"TTToe 3");

		SetTimer(TMR_MENU_ID, TMR_MENU_FREQUENCY, NULL);
		SetTimer(TMR_FIELD_ID, TMR_FIELD_FREQUENCY, NULL);
		SetTimer(TMR_MAIN_ID, TMR_MAIN_FREQUENCY, NULL);
		SetTimer(TMR_WINNING_COMB_ID, TMR_WINNING_COMB_FREQUENCY, NULL);
		SetTimer(TMR_STATUS_BAR_ID, TMR_STATUS_BAR_FREQUENCY, NULL);

		initializeMenu(CWRect);
		prevRect = CWRect;

		getMainFrame()->setStPlayersMove();
		getMainFrame()->resetAIPerc();
		getMainFrame()->setPlayerScr(0);
		getMainFrame()->setDrawScr(0);
		getMainFrame()->setAIScr(0);
		getMainFrame()->setMoveNum(0);

		isViewInitialized = true; 
	}
	else{
		if(CWRect != prevRect){
			prevRect = CWRect;
			menu->setOpenedUpperLeft(Point(CWRect.right - GlobalSettings::menuWidth, 0));
		 	menu->setClosedUpperLeft(Point(CWRect.right - 20, 0));
			menu->setHeight(CWRect.Height());
			menu->recalculate();
		}
	
	}
	Graphics::createMemDc(&CWRect);
	CPen *background = new CPen(0,1,GlobalSettings::backgroundClr.getRGB());
	CBrush *backgroundBr = new CBrush(GlobalSettings::backgroundClr.getRGB());

	Graphics::drawRectangle(nullptr, CWRect.left, CWRect.top, CWRect.right, CWRect.bottom, backgroundBr, background, true, true, true);

	menu->display();
	//Graphics::deleteMemDc();

	//Graphics::createMemDc(&CWRect);
	fitFieldInClientArea();
	applySettingsAndRerender(settings);
	//field->display(false, false, true, true);
	Graphics::deleteMemDc();

	delete background;
	delete backgroundBr;
}

int cnt = 0;

UINT CTTToeView::testFunction(LPVOID param){

	return 0;
}

void CTTToeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	Action act = menu->lButtonPressed(point);

	if(!aiThinking){
		if(act.status & Menu::POINT_INSIDE){
			if(act.valueReturned){
				processMenuAction(act.value);
			}	
		}

		Point pt = field->lButtonPressed(point);
	
		// Queueing the ai move
		if(pt.getIsValid()){
			lastMove = pt;

			field->makeMove(lastMove, BaseCell::MODE_PLAYER, true);
			getMainFrame()->setMoveNum(field->getMovesMade());

			moveQueued = true;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CTTToeView::resetMenuActionTimer(){
	KillTimer(TMR_MENU_ACT_SEQ_ID);
	SetTimer(TMR_MENU_ACT_SEQ_ID, TMR_MENU_ACT_SEQ_FREQUENCY, NULL);
}

void CTTToeView::processMenuAction(int value){

	if((value == FSIZE_INCREASE || value == FSIZE_DECREASE || value == CHANGE_STYLE) && field->getGameStarted()){
		processMenuActions();
		CString str("Please, end current game before using this option.");
		MessageBox(str);

		return;
	}

	if(value == FSIZE_INCREASE || value == FSIZE_DECREASE ||
	   value == SUBCELLSIZE_INCREASE || value == SUBCELLSIZE_DECREASE ||
	   value == NUM_OF_SUBCELLS_INCREASE || value == NUM_OF_SUBCELLS_DECREASE ||
	   value == DIST_CELL_INCREASE || value == DIST_CELL_DECREASE){
	
		if(menuSequenceOn){
				if(menuSequenceId == value || menuSequenceId + value == 0){
					if(value > 0){
						sequenceValue++;
					}
					else{
						sequenceValue--;
					}
					resetMenuActionTimer();	
				}
				else{
					processMenuActions();
					if(value > 0){
						sequenceValue = 1;
					}
					else{
						sequenceValue = -1;
					}
					menuSequenceOn = true;
					menuSequenceId = value;
					resetMenuActionTimer();
				}
		}
		else{

			if(value > 0){
				sequenceValue = 1;
			}
			else{
				sequenceValue = -1;
			}
			menuSequenceOn = true;
			menuSequenceId = value;
			resetMenuActionTimer();
		}
	}
	else{
		processMenuActions();

		if(value == CHANGE_COLOR){
			if(settings.fieldStyleInd >= fieldStyles.size()-1){
				settings.fieldStyleInd = 0;

				FieldStyle style = fieldStyles[settings.fieldStyleInd];

				settings.fieldBackgroundIndex = style.bgColorInd;
				settings.frameColPos = style.frameClrInd;
				settings.separatingLineIndex = style.frameClrInd;
				settings.xIndex = style.playerClrInd;
				settings.oIndex = style.aiClrInd;
				settings.emptyIndex = style.emptyClrInd;
			}
			else{
				settings.fieldStyleInd++;

				FieldStyle style = fieldStyles[settings.fieldStyleInd];

				settings.fieldBackgroundIndex = style.bgColorInd;
				settings.frameColPos = style.frameClrInd;
				settings.separatingLineIndex = style.frameClrInd;
				settings.xIndex = style.playerClrInd;
				settings.oIndex = style.aiClrInd;
				settings.emptyIndex = style.emptyClrInd;
			}
			applySettingsAndRerender(settings, true);
		}
		else if(value == CHANGE_GROWING){
			Settings sett = settings;
			sett.grow = !sett.grow;
			applySettingsAndRerender(sett);
		}
		else if(value == DECREASE_GAME_LEVEL){
			if(GlobalSettings::gameLevel > 0){
				GlobalSettings::gameLevel--;
				GlobalSettings::aiSettings.level = gameLevels[GlobalSettings::gameLevel];
			}
		}
		else if(value == INCREASE_GAME_LEVEL){
			if(GlobalSettings::gameLevel < gameLevels.size()-1){
				GlobalSettings::gameLevel++;
				GlobalSettings::aiSettings.level = gameLevels[GlobalSettings::gameLevel];
			}
		}

		else if(value == DECREASE_BASE_SEL_NUM){
			if(GlobalSettings::aiSettings.level.baseSelNum > MIN_NUM_OF_ROOT_CELLS){
				GlobalSettings::aiSettings.level.baseSelNum--;
			}
		}
		else if(value == INCREASE_BASE_SEL_NUM){
			if(GlobalSettings::aiSettings.level.baseSelNum < MAX_NUM_OF_ROOT_CELLS){
				GlobalSettings::aiSettings.level.baseSelNum++;
			}
		}

		else if(value == DECREASE_DEPTH){
			if(GlobalSettings::aiSettings.level.depth > MIN_DEPTH){
				GlobalSettings::aiSettings.level.depth--;
			}
		}
		else if(value == INCREASE_DEPTH){
			if(GlobalSettings::aiSettings.level.depth < MAX_DEPTH){
				GlobalSettings::aiSettings.level.depth++;
			}
		}

		else if(value == DECREASE_SEL_NUM){
			if(GlobalSettings::aiSettings.level.selNum > MIN_NUM_OF_CELLS){
				GlobalSettings::aiSettings.level.selNum--;
			}
		}
		else if(value == INCREASE_SEL_NUM){
			if(GlobalSettings::aiSettings.level.selNum < MAX_NUM_OF_CELLS){
				GlobalSettings::aiSettings.level.selNum++;
			}
		}

		else if(value == CHANGE_STYLE){
			if(GlobalSettings::settingsInd < settingsVector.size() - 1){
				GlobalSettings::settingsInd++;

				Settings sett = settingsVector[GlobalSettings::settingsInd];
				FieldStyle fst = fieldStyles[sett.fieldStyleInd];
				sett.fieldBackgroundIndex = fst.bgColorInd;
				sett.frameColPos = fst.frameClrInd;
				sett.separatingLineIndex = fst.frameClrInd;
				sett.emptyIndex = fst.emptyClrInd;
				sett.xIndex = fst.playerClrInd;
				sett.oIndex = fst.aiClrInd;

				applySettingsAndRerender(sett);
			}
			else{
			    GlobalSettings::settingsInd = 0;				
			   
			    Settings sett = settingsVector[GlobalSettings::settingsInd];
				FieldStyle fst = fieldStyles[sett.fieldStyleInd];
				sett.fieldBackgroundIndex = fst.bgColorInd;
				sett.frameColPos = fst.frameClrInd;
				sett.separatingLineIndex = fst.frameClrInd;
				sett.emptyIndex = fst.emptyClrInd;
				sett.xIndex = fst.playerClrInd;
				sett.oIndex = fst.aiClrInd;

				applySettingsAndRerender(sett);
			}
		
		}
		else if(value == CHANGE_DOUBLE_BUFFERING){
			GlobalSettings::doubleBuffering = !GlobalSettings::doubleBuffering;
		}
		else if(value == SHOW_HIDE_FIELD_FRAME){
			Settings tempSet = settings;
			tempSet.frameExists = !tempSet.frameExists;
			if(isPossibleToFitFieldInClientArea(&tempSet, prevRect)){
				applySettingsAndRerender(tempSet);
			}
		}
		else if(value == SHOW_HIDE_SEP_LINES){
			Settings tempSet = settings;
			tempSet.separatingLinesExist = !tempSet.separatingLinesExist;
			if(isPossibleToFitFieldInClientArea(&tempSet, prevRect)){
				applySettingsAndRerender(tempSet);
			}
		}
		else if(value == SHOW_TRACK){
			settings.showtrack = !settings.showtrack;
		}

		refreshControlLabels();
	}
}

void CTTToeView::refreshControlLabels(){
	char* lblNum = new char[4];
	_itoa(GlobalSettings::aiSettings.level.baseSelNum, lblNum, 10);
	string lbl = "Root cell # - ";
	lbl.append(lblNum);
	if(lbl != swb_RootCellNum->getLabel()){
		swb_RootCellNum->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];
	_itoa(GlobalSettings::aiSettings.level.depth, lblNum, 10);
	lbl = "Search depth - ";
	lbl.append(lblNum);
	if(lbl != swb_SearchDepth->getLabel()){
		swb_SearchDepth->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];
	_itoa(GlobalSettings::aiSettings.level.selNum, lblNum, 10);
	lbl = "Cell # - ";
	lbl.append(lblNum);
	if(lbl != swb_CellNum->getLabel()){
		swb_CellNum->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];

	GameLevel lev = gameLevels[GlobalSettings::gameLevel];

	if(lev.baseSelNum != GlobalSettings::aiSettings.level.baseSelNum ||
	   lev.selNum != GlobalSettings::aiSettings.level.selNum ||
	   lev.depth != GlobalSettings::aiSettings.level.depth ){
	
		lbl = "Game level - Custom";
	}
	else{
		lbl = "Game level - ";
		_itoa(GlobalSettings::gameLevel, lblNum, 10);
		lbl.append(lblNum);
	}

	if(lbl != swb_GameLevel->getLabel()){
		swb_GameLevel->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];
	_itoa(settings.fieldSize, lblNum, 10);
	lbl = "Field size - ";
	lbl.append(lblNum);
	if(lbl != swb_FieldSize->getLabel()){
		swb_FieldSize->setLabel(lbl);
	}

	delete lblNum;	

	lblNum = new char[4];
	_itoa(settings.subCellSizes[settings.posnum - 1], lblNum, 10);
	lbl = "Subcell size - ";
	lbl.append(lblNum);
	if(lbl != swb_SubCellSize->getLabel()){
		swb_SubCellSize->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];
	_itoa(settings.numOfSubCells, lblNum, 10);
	lbl = "# of subcells - ";
	lbl.append(lblNum);
	if(lbl != swb_SubCellNumber->getLabel()){
		swb_SubCellNumber->setLabel(lbl);
	}

	delete lblNum;

	lblNum = new char[4];
	_itoa(settings.distCellHalf, lblNum, 10);
	lbl = "Cell distance - ";
	lbl.append(lblNum);
	if(lbl != swb_CellDistance->getLabel()){
		swb_CellDistance->setLabel(lbl);
	}

	delete lblNum;

	if(settings.grow){
		lbl = "Growing - On";
	}
	else{
		lbl = "Growing - Off";
	}

	if(lbl != bt_Growing->getLabel()){
		bt_Growing->setLabel(lbl);
	}

	if(GlobalSettings::doubleBuffering){
		lbl = "Double buff.- On";
	}
	else{
		lbl = "Double buff.- Off";
	}

	if(lbl != bt_DoubleBuffering->getLabel()){
		bt_DoubleBuffering->setLabel(lbl);
	}

	if(settings.frameExists){
		lbl = "Frame - On";
	}
	else{
		lbl = "Frame - Off";
	}

	if(lbl != bt_FieldFrame->getLabel()){
		bt_FieldFrame->setLabel(lbl);
	}

	if(settings.separatingLinesExist){
		lbl = "Sep. lines - On";
	}
	else{
		lbl = "Sep. lines - Off";
	}

	if(lbl != bt_FieldLines->getLabel()){
		bt_FieldLines->setLabel(lbl);
	}

	if(settings.showtrack){
		lbl = "Track anim.- On";
	}
	else{
		lbl = "Track anim.- Off";
	}

	if(lbl != bt_ShowTrack->getLabel()){
		bt_ShowTrack->setLabel(lbl);
	}

	menu->display();
}

void CTTToeView::processMenuActions(){
	if(menuSequenceOn){
		switch(menuSequenceId){
				case FSIZE_INCREASE:
				case FSIZE_DECREASE:
					if(sequenceValue > 0){
						Settings sett = settings;
					
						for(int i = 0; i < sequenceValue; i++){
							sett.fieldSize++;

							if(!isPossibleToFitFieldInClientArea(&sett,prevRect)){
								sett.fieldSize--;
								break;
							}
						}

						if(sett.fieldSize != settings.fieldSize){
							applySettingsAndRerender(sett);
						}
					}
					else{
						Settings tSett = settings;

						if(sequenceValue + settings.fieldSize >= MIN_FIELD_SIZE){
							tSett.fieldSize += sequenceValue;
						}	
						else{
							tSett.fieldSize = MIN_FIELD_SIZE;
						}

						if(tSett.fieldSize != settings.fieldSize){
							applySettingsAndRerender(tSett);
						}
					}

					menuSequenceOn = false;
					sequenceValue = 0;
				break;

				case SUBCELLSIZE_DECREASE:
				case SUBCELLSIZE_INCREASE:
					if(sequenceValue > 0){
						Settings sett = settings;					

						for(int i = 0; i < sequenceValue; i++){
							sett.setSizes(sett.subCellSizes[0], sett.subCellSizes[sett.posnum-1] + 1);

							if(!isPossibleToFitFieldInClientArea(&sett,prevRect)){
								sett.setSizes(sett.subCellSizes[0], sett.subCellSizes[sett.posnum-1] - 1);
								break;
							}
						}
						if(sett.subCellSizes[sett.posnum - 1] != settings.subCellSizes[settings.posnum - 1]){
							applySettingsAndRerender(sett);
						}
					}
					else{
						Settings sett = settings;

						if(sequenceValue + settings.subCellSizes[settings.posnum-1] >= MIN_SUBCELL_SIZE){
							sett.setSizes(sett.subCellSizes[0], sett.subCellSizes[sett.posnum-1] + sequenceValue);
						}	
						else{
							sett.setSizes(sett.subCellSizes[0], MIN_SUBCELL_SIZE);
						}

						if(sett.subCellSizes[sett.posnum - 1] != settings.subCellSizes[settings.posnum - 1]){
							applySettingsAndRerender(sett);
						}
					}

					menuSequenceOn = false;
					sequenceValue = 0;
				break;

				case NUM_OF_SUBCELLS_DECREASE:
				case NUM_OF_SUBCELLS_INCREASE:
					if(sequenceValue > 0){
						Settings sett = settings;
					
						for(int i = 0; i < sequenceValue; i++){
							sett.numOfSubCells++;

							if(!isPossibleToFitFieldInClientArea(&sett,prevRect)){
								sett.numOfSubCells--;
								break;
							}
						}
						if(sett.numOfSubCells != settings.numOfSubCells){
							applySettingsAndRerender(sett);
						}
					}
					else{
						Settings sett = settings;

						if(sequenceValue + settings.numOfSubCells >= MIN_NUM_OF_SUBCELLS){
							sett.numOfSubCells = sett.numOfSubCells + sequenceValue;
						}	
						else{
							sett.numOfSubCells = MIN_NUM_OF_SUBCELLS;
						}

						if(sett.numOfSubCells != settings.numOfSubCells){
							applySettingsAndRerender(sett);
						}
					}

					menuSequenceOn = false;
					sequenceValue = 0;
				break;

				case DIST_CELL_DECREASE:
				case DIST_CELL_INCREASE:
					if(sequenceValue > 0){
						Settings sett = settings;
					
						for(int i = 0; i < sequenceValue; i++){
							sett.distCellHalf ++;

							if(!isPossibleToFitFieldInClientArea(&sett,prevRect)){
								sett.distCellHalf--;
								break;
							}
						}

						if(sett.distCellHalf != settings.distCellHalf){
							applySettingsAndRerender(sett);
						}
					}
					else{
						Settings sett = settings;

						if(sequenceValue + settings.distCellHalf >= 0){
							sett.distCellHalf = sett.distCellHalf + sequenceValue;
						}	
						else{
							sett.distCellHalf = 0;
						}

						if(sett.distCellHalf != settings.distCellHalf){
							applySettingsAndRerender(sett);
						}
					}

					menuSequenceOn = false;
					sequenceValue = 0;
				break;
		}

		refreshControlLabels();
	}
}

bool CTTToeView::isPossibleToFitFieldInClientArea(Settings* settings, CRect rect){
	CRect size;

	field->getSizeForGivenSettings(settings, size);

	int width = size.right - size.left;

	int rightMax = rect.right - GlobalSettings::menuWidth;
	int bottomMax = rect.bottom;

	if(width > rightMax || width > bottomMax || width > MAX_FIELD_WIDTH || width > MAX_FIELD_HEIGHT){
		return false;
	}

	return true;
}
/*
CRect CTTToeView::getMinimalClientAreaSize(){
	CRect size;

	size = field->getCRect();

	int width = size.right - size.left;

	CRect res;
	res.left = 0;
	res.right = width + GlobalSettings::menuWidth + 1;
	res.top = 0;
	res.bottom = width + 1;

	return res;
}*/

void CTTToeView::fitFieldInClientArea(bool centralize){
	int rightMax = prevRect.right - GlobalSettings::menuWidth;
	int bottomMax = prevRect.bottom;

	Point br = field->getBottomRight();
    Point ul = field->getUpperLeft();

	if(!centralize){
		int horShift;
		int vertShift;

		if(br.x > rightMax){
			horShift = rightMax - br.x;
		}

		if(br.y > bottomMax){
			vertShift = bottomMax - br.y;
		}

		ul.shift(-vertShift, -horShift);
		settings.fieldULX = ul.x;
		settings.fieldULY = ul.y;
		field->setUpperLeft(ul);
		field->initializeCenters(false);
	}
	else{
		int width = br.x - ul.x;
		int newX = (rightMax - width)/2;
		int newY = (bottomMax - width)/2;
		settings.fieldULX = newX;
		settings.fieldULY = newY;
		field->setUpperLeft(Point(newX, newY));
		field->initializeCenters(false);
	}
}

CRect CTTToeView::getSizesOfFieldWhenFit(CRect origRect, bool centralize){
	int rightMax = prevRect.right - GlobalSettings::menuWidth;
	int bottomMax = prevRect.bottom;

	Point br = Point(origRect.right, origRect.bottom);
	Point ul = Point(origRect.left, origRect.top);

	CRect res;

	if(!centralize){
		int horShift;
		int vertShift;

		if(br.x > rightMax){
			horShift = rightMax - br.x;
		}

		if(br.y > bottomMax){
			vertShift = bottomMax - br.y;
		}

		ul.shift(-vertShift, -horShift);
		br.shift(-vertShift, -horShift);

		res.left = ul.x;
		res.top = ul.y;
		res.right = br.x;
		res.bottom = br.y;
	}
	else{
		int width = br.x - ul.x;
		int newX = (rightMax - width)/2;
		int newY = (bottomMax - width)/2;

		br.shift(newX - ul.x, newY - ul.y);

		res.left = newX;
		res.top = newY;
		res.right = br.x;
		res.bottom = br.y;
	}
	return res;
}

void CTTToeView::applySettingsAndRerender(Settings sett, bool displayOnly){
	if(!displayOnly){
		CRect finalRect;
		field->getSizeForGivenSettings(&sett, finalRect);

		finalRect = getSizesOfFieldWhenFit(finalRect, true);

		CRect rect = Utils::getContainingRect(field->getCRect(), finalRect);

		bool keepModes = settings.fieldSize == sett.fieldSize;
		bool resetAnimatedCells = !keepModes || settings.numOfSubCells != sett.numOfSubCells;

		Graphics::createMemDc(&rect);

		field->clear();
		settings = sett;
		field->createCells();
		field->initializeCells(keepModes);
		if(resetAnimatedCells){
			field->resetAnimatedCells();
		}
		fitFieldInClientArea(true);
		field->display(false,false,true,true);

		Graphics::deleteMemDc();
	}
	else{
		CRect rect = field->getCRect();

		Graphics::createMemDc(&rect);
		field->display(false,false,true,true);
		Graphics::deleteMemDc();
	}
}


void CTTToeView::OnRButtonDown(UINT nFlags, CPoint point)
{		
	menu->rButtonPressed(point);

    CView::OnRButtonDown(nFlags, point);
}

void CTTToeView::OnMouseMove(UINT nFlags, CPoint point)
{
    menu->mouseMovedOver(point);
    field->mouseOver(Point(point));

	CView::OnMouseMove(nFlags, point);
}

void CTTToeView::updateStatusBarText(){
	CString txt;
	txt.Append(CString(""));
}

void CTTToeView::animateWinningCombination(){

	FieldLineIterator it;

	for(size_t i = 0; i < winningCombinations.size(); i++){
		Combination comb = winningCombinations[i];

		it.initialize(settings.fieldSize, comb.direction, comb.start, true);

		do{
				if(vicAnimFirstTime || !field->findInListOfAnimatedCells(it.currentPoint)){
					field->animateCell(it.currentPoint);
				}

		} while(it.currentPoint != comb.end && it.getNext()); //order is important!

		vicAnimFirstTime = false;
	}
} 

void CTTToeView::OnTimer(UINT nFlags)
{
	if(nFlags == TMR_FIELD_ID){
		field->updateCells();
	}

	else if (nFlags == TMR_MENU_ID){
		menu->timerTicked();
		menu->displayUpdated();	
	}

	else if(nFlags == TMR_MAIN_ID){
		if(gameStatus != -2){
			if(moveQueued){
				moveMade = false;
				aiThinking = true;

			    aiThread = AfxBeginThread(makeAIMove, &vicCombStartEnd);
				moveQueued = false;
			}
			if(moveMade){
				getMainFrame()->setStPlayersMove();
				getMainFrame()->resetAIPerc();
				getMainFrame()->setMoveNum(field->getMovesMade());

				moveMade = false;
				aiThinking = false;

				if(gameStatus != -1){
					if(aiMove.getIsValid()){
						field->aIPlayed(aiMove);
					}

					vicAnimFirstTime = true;
					OnMouseMove(0,CPoint(0,0));
					
					bool prevValue = settings.showtrack;
					settings.showtrack = true;

					if(gameStatus == RateField::ST_AI_WON){
						gameStatus = -2;
						getMainFrame()->setAIScr(++aiVictoryNumber);
						MessageBox(L"Sorry, you have lost...");
					}
					else if(gameStatus == RateField::ST_PLAYER_WON){
						gameStatus = -2;
						getMainFrame()->setPlayerScr(++playerVicNumber);
						MessageBox(L"Congratulations, you have won!");
					}
					else{
						gameStatus = -1;
						getMainFrame()->setDrawScr(++drawNumber);
						MessageBox(L"Draw!");
					}

					settings.showtrack = prevValue;

					gameStatus = -1;
					field->resetCellModes();
					getMainFrame()->setMoveNum(field->getMovesMade());
					field->display(false,false,false,false);
				}
				else{
					lastMove = aiMove;
					field->aIPlayed(lastMove);
				}
			}
		}
	}

	else if(nFlags == TMR_MENU_ACT_SEQ_ID){
		processMenuActions();
		KillTimer(TMR_MENU_ACT_SEQ_ID);
	}

	else if(nFlags == TMR_WINNING_COMB_ID){
		if(gameStatus == -2){
			animateWinningCombination();
		}
	}

	else if(nFlags == TMR_STATUS_BAR_ID){
		if(aiThinking){
			getMainFrame()->updateStAIThinking();
			getMainFrame()->setAIPerc(field->getAIPercent());
		}
	}

	else if(nFlags == TMR_TEST_ID){
		if(cnt == 0){

		}
		else{
			testProperty = false;
			char* a = new char[20];
			_itoa(cnt, a, 10);
			cnt = 0;
			CString str(a);

			MessageBox(str);
		}	
	}

	CView::OnTimer(nFlags);
}

UINT CTTToeView::makeAIMove(LPVOID param)
{
	if(field->checkIfCompletedCombinationExist()){
		gameStatus = RateField::ST_PLAYER_WON;

		winningCombinations = field->getExtendedCompletedCombinations();

		aiMove.setIsValid(false);
		moveMade = true;

		return 0;
	}
	else if(field->checkIfDrawExists()){
		gameStatus = RateField::ST_DRAW;

		aiMove.setIsValid(false);
		moveMade = true;

		return 0;
	}

	srand((unsigned int)time(0));

	Point a = field->evaluateAIMove();
	field->makeMove(a, BaseCell::MODE_PC, true);

	if(field->checkIfCompletedCombinationExist()){
		gameStatus = RateField::ST_AI_WON;

		winningCombinations = field->getExtendedCompletedCombinations();

		aiMove.setIsValid(false);

		aiMove = a;
		moveMade = true;

		return 0;
	}
	else if(field->checkIfDrawExists()){
		gameStatus = RateField::ST_DRAW;

		aiMove = a;
		aiMove.setIsValid(true);
		moveMade = true;

		return 0;
	}

	aiMove = a;
	moveMade = true;

	return 0;
}

BOOL CTTToeView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CTTToeView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CView::OnGetMinMaxInfo(lpMMI);
}

void CTTToeView::OnClose()
{
	if(aiThread != nullptr){
		field->forceStopThinking();
		WaitForSingleObject(aiThread->m_hThread, INFINITE);
	}

	for(size_t i = 0; i < objectsToDelete.size(); i++){
		delete objectsToDelete[i];
		objectsToDelete[i] = nullptr;
	}

	delete Graphics::cdc;

	CView::OnClose();
}

CMainFrame* CTTToeView::getMainFrame(){
	return (CMainFrame*)AfxGetApp()->GetMainWnd();
}

// CTTToe3View diagnostics

#ifdef _DEBUG
void CTTToeView::AssertValid() const
{
	CView::AssertValid();
}

void CTTToeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTTToeDoc* CTTToeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTTToeDoc)));
	return (CTTToeDoc*)m_pDocument;
}
#endif //_DEBUG


