// TabbedView.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "TabbedView.h"
#include "InfoView.h"

// CTabbedView

IMPLEMENT_DYNCREATE(CTabbedView, CTabView)

CTabbedView::CTabbedView()
{

}

CTabbedView::~CTabbedView()
{
}

BEGIN_MESSAGE_MAP(CTabbedView, CTabView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabbedView diagnostics

#ifdef _DEBUG
void CTabbedView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CTabbedView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTabbedView message handlers


int CTabbedView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	AddView (RUNTIME_CLASS (CInfoView), _T("政工网普查情况"), 100);
	AddView (RUNTIME_CLASS (CInfoView), _T("蓝网工程普查情况"), 101);	

	GetTabControl().EnableTabSwap( FALSE );

	return 0;
}


BOOL CTabbedView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CTabView::OnEraseBkgnd(pDC);
}

void CTabbedView::ShowObject(CBaseObject* pOb)
{	
	GetGridCtrl(0)->RemoveAll();
	GetGridCtrl(1)->RemoveAll();

	pOb->ShowInTabbedView(this);
}

CMFCPropertyGridCtrl* CTabbedView::GetGridCtrl(int i/* =0 */)
{
	CMFCPropertyGridCtrl* pGridCtrl = &(static_cast<CInfoView*>(GetTabControl().GetTabWnd(i))->m_info);

	return pGridCtrl;
}