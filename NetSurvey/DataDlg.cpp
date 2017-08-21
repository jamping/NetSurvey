// DataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "DataDlg.h"
#include "afxdialogex.h"


// CDataDlg dialog

IMPLEMENT_DYNCREATE(CDataDlg, CDialogEx)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataDlg::IDD, pParent)
{

}

CDataDlg::~CDataDlg()
{
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_propGrid);
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDataDlg message handlers


void CDataDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialogEx::PostNcDestroy();
}


void CDataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if ( m_propGrid.GetSafeHwnd() )
	{
		CRect rect;
		GetWindowRect(&rect);

		rect.DeflateRect(2,2,2,4);
		ScreenToClient(&rect);

		m_propGrid.MoveWindow(rect);
	}
}
