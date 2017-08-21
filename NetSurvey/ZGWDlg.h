#pragma once
#include "ExpandingDialog.h"
#include "ZGW.h"

// CZGWDlg dialog

class CZGWDlg : public CExpandingDialog
{
public:
	CZGWDlg(CZGW* pZGW,CWnd* pParent = NULL);   // standard constructor
	virtual ~CZGWDlg();

	void SetValue(CZGW::ZGWData& data);
	BOOL GetValue(CZGW::ZGWData& data);
// Dialog Data
	enum { IDD = IDD_ZGW };

	virtual BOOL OnDialogExpanding(BOOL bExpanded);
	virtual void OnDialogExpanded(BOOL bExpanded);
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnUpdateFibre(CCmdUI* pCmdUI);	
	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	int m_nAdmin;
	int m_nBW;
	int m_nComputer;
	CString m_strIP;
	BOOL m_bConnected;
	BOOL m_bFibre;
	BOOL m_bOnlyOne;
	int m_nRoom;
	int m_nServer;
	CString m_strWebsite;

	CZGW* m_pZGW;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedIsConnected();
	CMFCPropertyGridCtrl m_subZGW;
	BOOL m_bValueChange;
};
