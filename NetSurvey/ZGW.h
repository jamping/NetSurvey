#pragma once
#include "baseobject.h"
#include "NetSurvey.h"

class CZGW :	public CBaseObject
{
public:

	struct ZGWData
	{
		CString  m_strZGWID;    // 政工网ID
		CString  m_strArmyID;   // 所属部队ID
		BOOL     m_bConnected; // 网络联通情况
		BOOL     m_bFibre;      // 是否完成光纤辅设	
		BOOL     m_bOnlyOne;    // 入网方式,否则为局域网接入
		CString  m_strIP;       // IP地址
		int      m_nBW;         // 带宽
		int      m_nComputer;   // 终端数量
		int      m_nServer;     // 服务器数量		
		int      m_nAdmin;      // 网管人员数量
		int      m_nRoom;       // 机房面积
		CString  m_strWebsite;  // 网站

		ZGWData()
		{
			m_bConnected = FALSE;
			m_bFibre = FALSE;
			m_bOnlyOne = FALSE;
			m_nBW = 0;			
			m_nComputer = 0;
			m_nServer = 0;
			m_nAdmin = 0;
			m_nRoom = 0;
			m_strIP = _T("");
			m_strWebsite = _T("");
		}
	};	

	CZGW(void);
	~CZGW(void);

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
	ZGWData m_data;
};

