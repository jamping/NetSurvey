// HelperObject.h: interface for the CHelperObject class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseObject.h"
/************************************************************************
 辅助对象
************************************************************************/

enum HELPER_TYPE
{
	HELPER_SOURCE=0         //数据源
};


class CHelperObject : public CBaseObject
{
protected:
	int  m_nClass;
public:
	CHelperObject(int nClass);
	virtual ~CHelperObject();

public:
	int GetHelpType();
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl);
	UINT PromptMenuID();
	int PromptMenuPos();		
	void UpdateMenu(CMenu *pMenu);

	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
};




















