#pragma once
#include "baseobject.h"

class CArmys :	public CBaseObject
{
public:
	CArmys(void);
	~CArmys(void);

	CString GetArmyID() { return m_strArmyID; }

	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
		
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);		
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl);

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);

	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream	
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream

private:
	CString     m_strArmyID;

};

