// UserLoginDlg.h : header file
//
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog

class CUserLoginDlg : public CDialogEx
{
// Construction
public:
	CUserLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLoginDlg)
	enum { IDD = IDD_USER_LOGIN };
	int		m_nUserLevel;
	CString	m_strUserName;
	CString	m_strUserPasswd;
	//}}AFX_DATA
	CString m_strUserDesc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserLoginDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
public:
};