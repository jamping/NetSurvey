// ModifyPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ModifyPassDlg.h"
#include "afxdialogex.h"


// CModifyPassDlg dialog

IMPLEMENT_DYNAMIC(CModifyPassDlg, CDialogEx)

CModifyPassDlg::CModifyPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModifyPassDlg::IDD, pParent)
{
	//  m_strNewPass1 = _T("");
	m_strNewPass2 = _T("");
	m_strOldPass = _T("");
	m_strNewPass1 = _T("");
}

CModifyPassDlg::~CModifyPassDlg()
{
}

void CModifyPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_NEWPASS1, m_strNewPass1);
	DDX_Text(pDX, IDC_NEWPASS2, m_strNewPass2);
	DDX_Text(pDX, IDC_OLDPASS, m_strOldPass);
	DDX_Text(pDX, IDC_NEWPASS1, m_strNewPass1);
}


BEGIN_MESSAGE_MAP(CModifyPassDlg, CDialogEx)
END_MESSAGE_MAP()


// CModifyPassDlg message handlers


BOOL CModifyPassDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CModifyPassDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	if( theApp.GetCurUserPasswd().Compare(m_strOldPass)!=0)
	{
		MessageBox(_T("您输入的旧密码不正确，请重新输入！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	if( m_strNewPass1.GetLength()<6 )
	{
		MessageBox(_T("您输入的新密码小于6个字符，请重新输入！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	if( m_strNewPass1.Compare(m_strNewPass2)!=0)
	{
		MessageBox(_T("您两次输入的新密码不一样，请重新输入！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	
	CDialogEx::OnOK();
}
