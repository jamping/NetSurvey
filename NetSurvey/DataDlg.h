#pragma once


// CDataDlg dialog

class CDataDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CDataDlg)
public:
	CDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataDlg();

// Dialog Data
	enum { IDD = IDD_DATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_propGrid;
	virtual void PostNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
