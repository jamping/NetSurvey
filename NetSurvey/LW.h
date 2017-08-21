#pragma once
#include "baseobject.h"


class CLW :	public CBaseObject
{
public:
	struct LWData
	{
		CString  m_strLWID;     // 蓝网ID
		CString  m_strArmyID;   // 所属部队ID
		BOOL     m_bHad;        // 蓝网是否安装	
		BOOL     m_bOnlyOne;    // 入网方式,否则为局域网接入
		BOOL     m_bGood;       // 蓝网功能是否正常
		int      m_nBadType;    // 蓝网故障原因，0为完全损坏，1为天线，2为支架，3为高频头，4为通视卡
		int      m_nComputer;   // 终端数量
		int      m_nServer;     // 服务器数量		
		int      m_nAdmin;      // 网管人员数量
		int      m_nRoom;       // 机房面积
		int      m_nNewNum;     // 需要蓝网的数量

		LWData()
		{
			m_bHad = FALSE;     
			m_bOnlyOne = TRUE; 
			m_bGood = FALSE;    
			m_nBadType = -1; 
			m_nComputer = 0;
			m_nServer = 0;  
			m_nAdmin = 0;   
			m_nRoom = 0;  
			m_nNewNum = 0;
		}
	};
	

	CLW(void);
	~CLW(void);

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);	

	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);

	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);

	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream	
public:
	LWData m_data;
};

