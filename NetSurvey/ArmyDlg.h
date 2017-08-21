#pragma once

#include "Army.h"
#include "ExpandingDialog.h"
#include "ConTabCtrl.h"
#include "resource.h"
// CArmyDlg dialog

class CArmyDlg : public CExpandingDialog
{
public:
	CArmyDlg(CArmy* pArmy,CWnd* pParent = NULL);   // standard constructor
	virtual ~CArmyDlg();

	void SetValue(CArmy::ArmyData& data);
	BOOL GetValue(CArmy::ArmyData& data);

	virtual BOOL OnDialogExpanding(BOOL bExpanded);
	virtual void OnDialogExpanded(BOOL bExpanded);
// Dialog Data
	enum { IDD = IDD_ARMYINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void InitZGWData();
	void InitLWData();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	CString m_strArmyName;
	int m_nArmyLevel;
	int m_nArmyProp;
	afx_msg void OnBnClickedViewData();
	CConTabCtrl m_tab;
	CArmy*  m_pArmy;
};
