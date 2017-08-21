// ExportExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ExportExcelDlg.h"
#include "afxdialogex.h"


// CExportExcelDlg dialog

IMPLEMENT_DYNAMIC(CExportExcelDlg, CDialogEx)

CExportExcelDlg::CExportExcelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExportExcelDlg::IDD, pParent)
{
	m_nArmyLevel = 2;
}

CExportExcelDlg::~CExportExcelDlg()
{
}

void CExportExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_ARMY_LEVEL, m_nArmyLevel);
}


BEGIN_MESSAGE_MAP(CExportExcelDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportExcelDlg message handlers


BOOL CExportExcelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
