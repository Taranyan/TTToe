// TTToeDoc.h : interface of the CTTToeDoc class
//


#pragma once


class CTTToeDoc : public CDocument
{
protected: // create from serialization only
	CTTToeDoc();
	DECLARE_DYNCREATE(CTTToeDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CTTToeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


