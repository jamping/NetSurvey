#pragma once
#include "customtree.h"
#include "HelperObject.h"
#include "Army.h"

class CSourceTree :	public CCustomTree
{
public:
	CSourceTree(void);
	~CSourceTree(void);

	virtual BOOL DeleteSelection(BOOL bDelSelf=TRUE);				//ÏìÓ¦É¾³ý¼üÏûÏ¢
	virtual void InitUpdate(BOOL bReadOfDB=TRUE,BOOL bShowInfo=FALSE);
	virtual BOOL DoDrop(HTREEITEM hItemDragged, HTREEITEM hItemTarget, int nAction);

	void ProcessPendingRead(char* buf);

	CHelperObject m_objectHelper;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCollapse();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnExport();
	afx_msg void OnFresh();
	afx_msg void OnImport();
	afx_msg void OnSave();
	afx_msg void OnRootNew();
	afx_msg void OnArmyNew();
	afx_msg void OnRootFresh();
	afx_msg void OnRootDelete();
	afx_msg void OnExcel();

private:	
	void GetArmys(CArmy* pArmy,CTypedPtrArray<CPtrArray,CArmy*> &armys,int nArmyLevel);
};

