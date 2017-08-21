// UserLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "netsurvey.h"
#include "UserLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog


CUserLoginDlg::CUserLoginDlg(CWnd* pParent /* = NULL */)
	:CDialogEx(CUserLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLoginDlg)
	m_nUserLevel = -1;
	m_strUserName = _T("");
	m_strUserPasswd = _T("");
	//}}AFX_DATA_INIT
}


void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLoginDlg)
	DDX_CBIndex(pDX, IDC_COMBO_PRIVS, m_nUserLevel);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_USER_PASSWD, m_strUserPasswd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialogEx)
	//{{AFX_MSG_MAP(CUserLoginDlg)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg message handlers

void CUserLoginDlg::OnOK() 
{
	if(!theApp.DBIsOpen())
		theApp.ConnectDB();
	// TODO: Add extra validation here
	UpdateData();

	CString strSql,strUserPasswd;
		
	strSql.Format("select * from netsurvey_user where user_name ='%s'",m_strUserName);
	
	CADORecordset rs(theApp.GetDB());
	if(rs.Open(strSql,CADORecordset::openQuery))
	{
		if(rs.IsEOF())
		{
			MessageBox(_T("用户不存在，请与管理员联系"),_T("注意"),MB_OK|MB_ICONWARNING);
			return ;
		}
		
		rs.GetFieldValue("user_passwd",strUserPasswd);
		CDes des;
		if(des.DesCrypteString("auqfvfhpgu",strUserPasswd,0).CompareNoCase(m_strUserPasswd)==0)
		{
			int nLevel;
			rs.GetFieldValue("user_level",nLevel);
			if(nLevel!=m_nUserLevel)
			{
				MessageBox(_T("用户身份不正确，请重新选择"),_T("注意"),MB_OK|MB_ICONWARNING);
				return;
			}
		}
		else
		{
			MessageBox(_T("密码不正确，请重新输入"),_T("注意"),MB_OK|MB_ICONWARNING);
			return;
		}
		rs.GetFieldValue("user_desc",m_strUserDesc);
		rs.Close();
	}
	CDialogEx::OnOK();
}

BOOL CUserLoginDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CUserLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CDialogEx::PreTranslateMessage(pMsg);
}
