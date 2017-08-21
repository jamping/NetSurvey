// InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "InfoView.h"


// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CFormView)

CInfoView::CInfoView()
	: CFormView(CInfoView::IDD)
{

}

CInfoView::~CInfoView()
{
}

void CInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_info);
}

BEGIN_MESSAGE_MAP(CInfoView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CInfoView diagnostics

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoView message handlers


void CInfoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_info.GetSafeHwnd())
		m_info.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE|SWP_NOZORDER);
}


void CInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetClientRect(&rect);

	PostMessage(WM_SIZE,SIZE_RESTORED,MAKELONG(rect.Width(),rect.Height()));
}
