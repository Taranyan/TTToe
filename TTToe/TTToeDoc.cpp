// TTToeDoc.cpp : implementation of the CTTToeDoc class
//

#include "stdafx.h"
#include "TTToe.h"

#include "TTToeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTTToeDoc

IMPLEMENT_DYNCREATE(CTTToeDoc, CDocument)

BEGIN_MESSAGE_MAP(CTTToeDoc, CDocument)
END_MESSAGE_MAP()


// CTTToeDoc construction/destruction

CTTToeDoc::CTTToeDoc()
{
	// TODO: add one-time construction code here

}

CTTToeDoc::~CTTToeDoc()
{
}

BOOL CTTToeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTTToeDoc serialization

void CTTToeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTTToeDoc diagnostics

#ifdef _DEBUG
void CTTToeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTTToeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTTToeDoc commands
