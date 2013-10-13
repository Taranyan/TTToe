// TTToeView.h : interface of the CTTToeView class
//


#pragma once

#include "Primitives.h"
#include "MainFrm.h"

class CTTToeView : public CView
{
protected: // create from serialization only
	CTTToeView();
	DECLARE_DYNCREATE(CTTToeView)

// Attributes
public:
	CTTToeDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CTTToeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnClose();

	void initializeDrawSet();
	void initializeMenu(CRect);
	void initializeControls();
	static UINT makeAIMove(LPVOID);
	void resetMenuActionTimer();
	void processMenuAction(int value);
	void processMenuActions();
	void refreshControlLabels();
	void setAILevelLabelManual();

	void animateWinningCombination();
	bool isPossibleToFitFieldInClientArea(Settings*, CRect);
	//CRect getMinimalClientAreaSize();
	void fitFieldInClientArea(bool centralize = true);
	CRect getSizesOfFieldWhenFit(CRect origRect, bool centralize = true);
	void applySettingsAndRerender(Settings, bool displayOnly = false);

	void updateStatusBarText();

	static UINT testFunction(LPVOID param);
	static CMainFrame* getMainFrame();
};

#ifndef _DEBUG  // debug version in TTToeView.cpp
inline CTTToeDoc* CTTToeView::GetDocument() const
   { return reinterpret_cast<CTTToeDoc*>(m_pDocument); }
#endif

