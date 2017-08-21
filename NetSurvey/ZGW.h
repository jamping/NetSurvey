#pragma once
#include "baseobject.h"
#include "NetSurvey.h"

class CZGW :	public CBaseObject
{
public:

	struct ZGWData
	{
		CString  m_strZGWID;    // ������ID
		CString  m_strArmyID;   // ��������ID
		BOOL     m_bConnected; // ������ͨ���
		BOOL     m_bFibre;      // �Ƿ���ɹ��˸���	
		BOOL     m_bOnlyOne;    // ������ʽ,����Ϊ����������
		CString  m_strIP;       // IP��ַ
		int      m_nBW;         // ����
		int      m_nComputer;   // �ն�����
		int      m_nServer;     // ����������		
		int      m_nAdmin;      // ������Ա����
		int      m_nRoom;       // �������
		CString  m_strWebsite;  // ��վ

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

