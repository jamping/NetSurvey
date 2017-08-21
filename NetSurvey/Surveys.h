#pragma once
#include "baseobject.h"

class CSurveys :public CBaseObject
{
public:
	CSurveys(void);
	~CSurveys(void);
	
	CString GetArmyID() { return m_strArmyID; }
	int GetArmyLevel() { return m_nArmyLevel; }
	void CreateSurveys();
	CBaseObject* GetSurveyObject(OBJECT_TYPE type);

	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);

	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);		

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);

	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
private:
	CString     m_strArmyID;
	int         m_nArmyLevel;
};

