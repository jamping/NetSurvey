#pragma once


// CModifyPassDlg dialog

class CModifyPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPassDlg)

public:
	CModifyPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyPassDlg();

// Dialog Data
	enum { IDD = IDD_MODIFY_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CString m_strNewPass1;
	CString m_strNewPass2;
	CString m_strOldPass;
	CString m_strNewPass1;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
