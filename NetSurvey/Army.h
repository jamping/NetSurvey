// Army.h: interface for the CArmy class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "BaseObject.h"
#include "Armys.h"
#include "Surveys.h"


class CArmy : public CBaseObject  
{
public:
	struct ArmyData
	{
		CString  m_strArmyID;
		CString  m_strParentID;
		CString  m_strArmyName;
		int      m_nArmyProp;
		int      m_nArmyLevel;

		ArmyData()
		{
			m_nArmyProp = 1;  // ���Ʋ���
			m_nArmyLevel = 0; // Ĭ�Ͼ�����
		}
	};	
	struct STAT_ZGWData
	{
		struct ZGW_CONN
		{
			int jianzhiTotal;
			int jianzhiCTotal;
			int feijianzhiTotal;
			int feijianzhiCTotal;

			ZGW_CONN()
			{
				jianzhiTotal = 0;
				jianzhiCTotal = 0;
				feijianzhiTotal= 0;
				feijianzhiCTotal= 0;
			}
			ZGW_CONN operator +=(const ZGW_CONN& data)
			{
				jianzhiTotal += data.jianzhiTotal;
				jianzhiCTotal += data.jianzhiCTotal;
				feijianzhiTotal += data.feijianzhiTotal;
				feijianzhiCTotal += data.feijianzhiCTotal;

				return *this;
			}

			void Init()
			{
				jianzhiTotal = 0;
				jianzhiCTotal = 0;
				feijianzhiTotal= 0;
				feijianzhiCTotal= 0;
			}
		};
		ZGW_CONN m_conn[5];     // ����������ͨ����������
		int m_nNotConnTotal;    // δ��ͨ�������ĵ�λ����
		int m_nFibreTotal;      // δ��ͨ�������ĵ�λ���˸�������
		long m_lComputerTotal;   // ��ͨ�������ĵ����ն�������
		int m_nServerTotal;     // ������������
		int m_nAdminTotal;      // ���������Ա������

		STAT_ZGWData()
		{
			m_nNotConnTotal = 0; 
			m_nFibreTotal = 0;   
			m_lComputerTotal = 0;
			m_nServerTotal = 0;  
			m_nAdminTotal = 0;   
		}
		STAT_ZGWData operator += (const STAT_ZGWData& data)
		{
			for(int i=0;i<sizeof(m_conn)/sizeof(m_conn[0]);i++)
				m_conn[i] += data.m_conn[i];

			m_nNotConnTotal += data.m_nNotConnTotal; 
			m_nFibreTotal += data.m_nFibreTotal;   
			m_lComputerTotal += data.m_lComputerTotal;
			m_nServerTotal += data.m_nServerTotal;  
			m_nAdminTotal += data.m_nAdminTotal;  

			return *this;
		}
		void Init()
		{
			for(int i=0;i<sizeof(m_conn)/sizeof(m_conn[0]);i++)
				m_conn[i].Init();

			m_nNotConnTotal = 0; 
			m_nFibreTotal = 0;   
			m_lComputerTotal = 0;
			m_nServerTotal = 0;  
			m_nAdminTotal = 0;   
		}
		BOOL IsValid()
		{
			BOOL bRet = FALSE;
			for( int i = 0;i < sizeof(m_conn)/sizeof(m_conn[0]);i++)
			{
				bRet = ( m_conn[i].jianzhiTotal != 0 ||
					     m_conn[i].jianzhiCTotal != 0 ||
						 m_conn[i].feijianzhiTotal != 0 ||
						 m_conn[i].feijianzhiCTotal != 0 );

				if( bRet )
					return TRUE;
			}
			return ( m_nNotConnTotal != 0 ||
			         m_nFibreTotal != 0 ||  
			         m_lComputerTotal != 0 ||
			         m_nServerTotal != 0 ||
			         m_nAdminTotal != 0 );  
		}
	};
	struct STAT_LWData
	{		
		int m_nHadTotal;        // ������װ����
		int m_nGoodTotal;       // ����������������
		int m_nNewTotal;        // ��Ҫ����������
		int m_nBadTotal;        // ����������
		int m_nBadAllTotal;     // ����ȫ��������
		int m_nBadTianXianTotal;// ����������
		int m_nBadZhiJiaTotal;  // ֧��������
		int m_nBadGPTTotal;     // ��Ƶͷ������
		int m_nBadTSKTotal;     // ͨ�ӿ�������
		int m_nComputerTotal;   // ���������ն�������
		int m_nServerTotal;     // ������������
		int m_nAdminTotal;      // ���������Ա������

		STAT_LWData()
		{
			m_nHadTotal = 0;        
			m_nGoodTotal = 0;       
			m_nNewTotal = 0;        
			m_nBadTotal = 0;        
			m_nBadAllTotal = 0;     
			m_nBadTianXianTotal = 0;
			m_nBadZhiJiaTotal = 0;  
			m_nBadGPTTotal = 0;     
			m_nBadTSKTotal = 0; 			      
			m_nComputerTotal = 0;
			m_nServerTotal = 0;  
			m_nAdminTotal = 0;   
		}
		STAT_LWData operator += (const STAT_LWData& data)
		{
			m_nHadTotal += data.m_nHadTotal;        
			m_nGoodTotal += data.m_nGoodTotal;       
			m_nNewTotal += data.m_nNewTotal;        
			m_nBadTotal += data.m_nBadTotal;        
			m_nBadAllTotal += data.m_nBadAllTotal;     
			m_nBadTianXianTotal += data.m_nBadTianXianTotal;
			m_nBadZhiJiaTotal += data.m_nBadZhiJiaTotal;  
			m_nBadGPTTotal += data.m_nBadGPTTotal;
			m_nBadTSKTotal += data.m_nBadTSKTotal; 			      
			m_nComputerTotal += data.m_nComputerTotal;
			m_nServerTotal += data.m_nServerTotal;  
			m_nAdminTotal += data.m_nAdminTotal;   

			return *this;
		}
		void Init()
		{
			m_nHadTotal = 0;        
			m_nGoodTotal = 0;       
			m_nNewTotal = 0;        
			m_nBadTotal = 0;        
			m_nBadAllTotal = 0;     
			m_nBadTianXianTotal = 0;
			m_nBadZhiJiaTotal = 0;  
			m_nBadGPTTotal = 0;     
			m_nBadTSKTotal = 0; 			      
			m_nComputerTotal = 0;
			m_nServerTotal = 0;  
			m_nAdminTotal = 0;   
		}
		BOOL IsValid()
		{
			return ( m_nHadTotal != 0 ||
				     m_nGoodTotal != 0 ||     
			         m_nNewTotal != 0 ||       
			         m_nBadTotal != 0 ||       
			         m_nBadAllTotal != 0 ||     
			         m_nBadTianXianTotal != 0 ||
			         m_nBadZhiJiaTotal != 0 ||  
			         m_nBadGPTTotal != 0 ||    
			         m_nBadTSKTotal != 0 ||			      
			         m_nComputerTotal != 0 ||
			         m_nServerTotal != 0 ||
			         m_nAdminTotal != 0 );  
		}
	};

	CArmy();
	virtual ~CArmy();    

	void CreateSurveys();

	void ReportZGWData(STAT_ZGWData& superZGWData);
	void ReportLWData(STAT_LWData& superLWData);	

	BOOL IsHaveChildArmy() { return m_armys.GetChildCount()>0; }
	BOOL IsJianZhi() { return m_data.m_nArmyProp == 1; }
	BOOL IsXuniArmy() { return m_data.m_nArmyProp == 2; }  // ���ⲿ��
	int  GetArmyLevel() { return m_data.m_nArmyLevel; }

	void SetID(LPCSTR strID,LPCSTR strDesc=NULL);
	virtual BOOL CanAccept(CBaseObject *pOB,CCustomTree *pTree);
	virtual void DoJoin(CBaseObject *pParent);		
	virtual void DoLeave(CBaseObject *pParent);
	virtual void ShowInTreeView(CCustomTree *pTree,BOOL bChild=FALSE);	
	virtual void ShowListCtrl(CListCtrlEx* pListCtrl);
	virtual void ShowInTabbedView(CView* pView);

	virtual UINT EditObject(CWnd *pParent,BOOL bReadOnly=TRUE);

	virtual BOOL SaveToDB(CADODatabase &db,BOOL bChild=TRUE,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar=NULL);	
	virtual BOOL LoadInfo(CADORecordset &rs);	
	virtual BOOL DeleteFromDB(CADODatabase &db,BOOL bDelSelf=TRUE,CProgressBar* pProgressBar=NULL);	

	virtual UINT PromptMenuID();
	virtual int PromptMenuPos();
	virtual void UpdateMenu(CMenu *pMenu);
	
	virtual TiXmlNode* StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Save to an stream	
	virtual BOOL StreamIn (TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar=NULL);	//Load to an stream
	virtual void STATData(void);

	void GetZGWReportData(CStringArray& data);
	void GetLWReportData(CStringArray& data);
public:
	ArmyData m_data;

	STAT_ZGWData m_curZGW;    // ��������
	STAT_ZGWData m_subZGW;    // �¼�����
	STAT_LWData  m_curLW;
	STAT_LWData  m_subLW;

	CDes     des;	

	CSurveys    m_surveys;
	CArmys      m_armys;
};