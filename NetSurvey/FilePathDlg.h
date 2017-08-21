#pragma once


// CFilePathDlg dialog

class CFilePathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilePathDlg)

public:
	CFilePathDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilePathDlg();

// Dialog Data
	enum { IDD = IDD_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFilePath;
	afx_msg void OnBnClickedButton1();
};
