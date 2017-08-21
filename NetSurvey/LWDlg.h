#pragma once
#include "LW.h"
#include "ExpandingDialog.h"
// CLWDlg dialog

class CLWDlg : public CExpandingDialog
{

public:
	CLWDlg(CLW* pLW,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLWDlg();

	void SetValue(CLW::LWData& data);
	BOOL GetValue(CLW::LWData& data);
// Dialog Data
	enum { IDD = IDD_LW };
	
	virtual BOOL OnDialogExpanding(BOOL bExpanded);
	virtual void OnDialogExpanded(BOOL bExpanded);
	virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnUpdateGood(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBadType(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOther(CCmdUI* pCmdUI);
	afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	int m_nAdmin;
	int m_nBadType;
	int m_nComputer;
	BOOL m_bGood;
	BOOL m_bHad;
	int m_nNewNum;
	BOOL m_bOnlyOne;
	int m_nRoom;
	int m_nServer;
	afx_msg void OnClickedIsHad();
	afx_msg void OnClickedIsGood();
	afx_msg void OnBnClickedSubarmy();
	CMFCPropertyGridCtrl m_subLW;
    CLW* m_pLW;
	BOOL m_bValueChange;
};
