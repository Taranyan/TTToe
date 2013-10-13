// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TTToe.h"

#include "MainFrm.h"

#pragma warning( disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT BASED_CODE indicators[] =
{
	ID_ST_GAME_ST,
	ID_ST_AI_PERC,
	ID_ST_PLAYER_SCR,
	ID_ST_DRAW_SCR,
	ID_ST_AI_SCR,
	ID_ST_MOVE_NUM
};



// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	//  cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_BORDER;

      // Size the window to 1/3 screen size and center it
      //cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 3;
     // cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 3;
	    cs.cy = 800;
      cs.cx = 1200;
    //  cs.y = ((cs.cy * 3) - cs.cy) / 2;
     // cs.x = ((cs.cx * 3) - cs.cx) / 2;

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers





void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 1024;
    lpMMI->ptMinTrackSize.y = 800;

	/*CView* pView = GetActiveView();
	if(pView != nullptr){
		pView->PostMessageW(WM_GETMINMAXINFO, (WPARAM)lpMMI);
	}*/
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnClose()
{
	CView* pView = GetActiveView();
	if(pView != nullptr){
		pView->SendMessage(WM_CLOSE);
	}
	CFrameWnd::OnClose();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	thinkingDotNum = -1;

	stBar.Create(this);
	stBar.SetIndicators(indicators,6);

	CRect rect;
	GetClientRect(&rect);

	stBar.SetPaneInfo(0,ID_ST_GAME_ST, SBPS_NORMAL,60);   
	stBar.SetPaneInfo(1,ID_ST_AI_PERC, SBPS_NORMAL,50);   
	stBar.SetPaneInfo(2,ID_ST_PLAYER_SCR, SBPS_NORMAL,100);   
	stBar.SetPaneInfo(3,ID_ST_DRAW_SCR, SBPS_NORMAL,100);   
	stBar.SetPaneInfo(4,ID_ST_AI_SCR, SBPS_NORMAL,100);   
	stBar.SetPaneInfo(5,ID_ST_MOVE_NUM, SBPS_STRETCH,1);   

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST, ID_ST_AI_SCR);

	return 0;
}

void CMainFrame::setStPlayersMove(){
	stBar.SetPaneText(0, CString("Your turn"));
	thinkingDotNum = -1;
}

void CMainFrame::updateStAIThinking(){
	if(thinkingDotNum == 5){
		thinkingDotNum = 0;
	}
	else{
		thinkingDotNum++;
	}

	CString dot(".");
	CString tnk("Thinking");

	int i = 0;

	while(i <= thinkingDotNum){
		tnk.Append(dot);
		i++;
	}

	stBar.SetPaneText(0, tnk);
}

void CMainFrame::resetAIPerc(){
	stBar.SetPaneText(1, CString(""));
}

void CMainFrame::setAIPerc(int perc){
	char* a = new char[5];

	_itoa(perc, a, 10);

	CString str(a);
	
	delete a;

	str.Append(CString("%"));

	stBar.SetPaneText(1, str);
}

void CMainFrame::setPlayerScr(int scr){
	char* a = new char[7];

	_itoa(scr, a, 10);

	CString scrStr(a);
	
	delete a;
	
	CString str("Player : ");
	str.Append(scrStr);
	stBar.SetPaneText(2, str);
}

void CMainFrame::setDrawScr(int scr){
	char* a = new char[7];

	_itoa(scr, a, 10);

	CString scrStr(a);

	delete a;

	CString str("Draws : ");
	str.Append(scrStr);
	stBar.SetPaneText(3, str);
}

void CMainFrame::setAIScr(int scr){
	char* a = new char[7];

	_itoa(scr, a, 10);

	CString scrStr(a);

	delete a;

	CString str("AI : ");
	str.Append(scrStr);
	stBar.SetPaneText(4, str);
}

void CMainFrame::setMoveNum(int num){
	char* a = new char[7];

	_itoa(num, a, 10);

	CString numStr(a);

	delete a;

	CString str("Moves made : ");
	str.Append(numStr);
	stBar.SetPaneText(5, str);
}