#pragma once
#include "baseobject.h"


class CLW :	public CBaseObject
{
public:
	struct LWData
	{
		CString  m_strLWID;     // ����ID
		CString  m_strArmyID;   // ��������ID
		BOOL     m_bHad;        // �����Ƿ�װ	
		BOOL     m_bOnlyOne;    // ������ʽ,����Ϊ����������
		BOOL     m_bGood;       // ���������Ƿ�����
		int      m_nBadType;    // ��������ԭ��0Ϊ��ȫ�𻵣�1Ϊ���ߣ�2Ϊ֧�ܣ�3Ϊ��Ƶͷ��4Ϊͨ�ӿ�
		int      m_nComputer;   // �ն�����
		int      m_nServer;     // ����������		
		int      m_nAdmin;      // ������Ա����
		int      m_nRoom;       // �������
		int      m_nNewNum;     // ��Ҫ����������

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

