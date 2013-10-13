// MainFrm.h : interface of the CMainFrame class
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar stBar;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	int thinkingDotNum;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void setStPlayersMove();
	void updateStAIThinking();
	void resetAIPerc();
	void setAIPerc(int perc);
	void setPlayerScr(int scr);
	void setDrawScr(int scr);
	void setAIScr(int scr);
	void setMoveNum(int num);
};


