// FilePathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "FilePathDlg.h"
#include "afxdialogex.h"


// CFilePathDlg dialog

IMPLEMENT_DYNAMIC(CFilePathDlg, CDialogEx)

CFilePathDlg::CFilePathDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilePathDlg::IDD, pParent)
{

	m_strFilePath = _T("d:\\");
}

CFilePathDlg::~CFilePathDlg()
{
}

void CFilePathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE_PATH, m_strFilePath);
}


BEGIN_MESSAGE_MAP(CFilePathDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFilePathDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFilePathDlg message handlers


void CFilePathDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(theApp.GetShellManager()->BrowseForFolder(m_strFilePath,this,m_strFilePath,_T("Ñ¡ÔñÎÄ¼þ¼Ð")))
	{
		UpdateData(FALSE);	
	}
}
