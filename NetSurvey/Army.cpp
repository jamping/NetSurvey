// Army.cpp: implementation of the CArmy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Army.h"
#include "ArmyDlg.h"
#include "ZGW.h"
#include "LW.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArmy::CArmy() :CBaseObject(OBJECT_TYPE_ARMY)
{
	SetImg(2,3);

	m_surveys.AddTo(this);
	m_armys.AddTo(this);
	
}

CArmy::~CArmy()
{

}
void CArmy::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strArmyID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CArmy::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;	

	return bAccept;
}
void CArmy::DoJoin(CBaseObject *pParent)
{
	if( pParent->GetType() == OBJECT_TYPE_ARMYS )
		m_data.m_strParentID = static_cast<CArmys*>(pParent)->GetArmyID();
	else
		m_data.m_strParentID=pParent->GetID();	

	CBaseObject::DoJoin(this);
}		
void CArmy::DoLeave(CBaseObject *pParent)
{
	m_data.m_strParentID.Empty();
	CBaseObject::DoLeave(this);
}
UINT CArmy::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CArmyDlg dlg(this);

	dlg.SetValue(m_data);
	nRet=dlg.DoModal();

	if( nRet== IDOK)
	{	
		if(dlg.GetValue(m_data))
		{
			SetModify();	
			ChangeDesc(m_data.m_strArmyName);
		}	
	}	

	return nRet;
}
BOOL CArmy::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

    strSql.Format("select * from netsurvey_army where army_id='%s'",m_data.m_strArmyID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
		}
		else
			rs.Edit();	
		
		bRet&=rs.SetFieldValue("parent_id",m_data.m_strParentID);
		bRet&=rs.SetFieldValue("army_name",des.DesCrypteString(_T("auqf_pgf_rd1@#"),m_data.m_strArmyName,1)); 
		bRet&=rs.SetFieldValue("is_jianzhi",m_data.m_nArmyProp);
		bRet&=rs.SetFieldValue("army_level",(int)m_data.m_nArmyLevel);

		bRet&=rs.Update();	

		rs.Close();
	} 
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	// save the sub stat data
	if( !IsHaveChildArmy() )
	{
		if( m_subZGW.IsValid() )
		{
			strSql.Format("select * from netsurvey_subzgw_stat where army_id='%s'",m_data.m_strArmyID);
			bRet=rs.Open(strSql,CADORecordset::openQuery);
			if(bRet)
			{
				if(rs.GetRecordCount()==0)
				{
					bRet&=rs.AddNew();
					bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
				}
				else
					rs.Edit();	

				bRet&=rs.SetFieldValue("not_conn_total",m_subZGW.m_nNotConnTotal);
				bRet&=rs.SetFieldValue("fibre_total",m_subZGW.m_nFibreTotal); 
				bRet&=rs.SetFieldValue("computer_total",m_subZGW.m_lComputerTotal);
				bRet&=rs.SetFieldValue("server_total",m_subZGW.m_nServerTotal);
				bRet&=rs.SetFieldValue("admin_total",m_subZGW.m_nAdminTotal);

				bRet&=rs.Update();	

				rs.Close();
			} 
			for(int i=0;i<sizeof(m_subZGW.m_conn)/sizeof(m_subZGW.m_conn[0]);i++)
			{
				strSql.Format("select * from netsurvey_subzgw_level_stat where army_id='%s' and army_level=%d",m_data.m_strArmyID,i);
				bRet=rs.Open(strSql,CADORecordset::openQuery);
				if(bRet)
				{
					if(rs.GetRecordCount()==0)
					{
						bRet&=rs.AddNew();
						bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
						bRet&=rs.SetFieldValue("army_level",i);
					}
					else
						rs.Edit();	

					bRet&=rs.SetFieldValue("jianzhi_total",m_subZGW.m_conn[i].jianzhiTotal);
					bRet&=rs.SetFieldValue("jianzhi_conn_total",m_subZGW.m_conn[i].jianzhiCTotal); 
					bRet&=rs.SetFieldValue("feijianzhi_total",m_subZGW.m_conn[i].feijianzhiTotal);
					bRet&=rs.SetFieldValue("feijianzhi_conn_total",m_subZGW.m_conn[i].feijianzhiCTotal);

					bRet&=rs.Update();	

					rs.Close();
				} 
			}
		}
		if( m_subLW.IsValid() )
		{
			strSql.Format("select * from netsurvey_sublw_stat where army_id='%s'",m_data.m_strArmyID);
			bRet=rs.Open(strSql,CADORecordset::openQuery);
			if(bRet)
			{
				if(rs.GetRecordCount()==0)
				{
					bRet&=rs.AddNew();
					bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
				}
				else
					rs.Edit();	

				bRet&=rs.SetFieldValue("had_total",m_subLW.m_nHadTotal);
				bRet&=rs.SetFieldValue("good_total",m_subLW.m_nGoodTotal); 
				bRet&=rs.SetFieldValue("new_total",m_subLW.m_nNewTotal);
				bRet&=rs.SetFieldValue("bad_total",m_subLW.m_nBadTotal);
				bRet&=rs.SetFieldValue("bad_all_total",m_subLW.m_nBadAllTotal);
				bRet&=rs.SetFieldValue("bad_tianxian_total",m_subLW.m_nBadTianXianTotal);
				bRet&=rs.SetFieldValue("bad_zhijia_total",m_subLW.m_nBadZhiJiaTotal); 
				bRet&=rs.SetFieldValue("bad_gpt_total",m_subLW.m_nBadGPTTotal);
				bRet&=rs.SetFieldValue("bad_tsk_total",m_subLW.m_nBadTSKTotal);
				bRet&=rs.SetFieldValue("computer_total",m_subLW.m_nComputerTotal);
				bRet&=rs.SetFieldValue("server_total",m_subLW.m_nServerTotal);
				bRet&=rs.SetFieldValue("admin_total",m_subLW.m_nAdminTotal);

				bRet&=rs.Update();	

				rs.Close();
			} 
		}
	}
	return bRet;
}	
BOOL CArmy::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet = FALSE;
	bRet = CBaseObject::LoadFromDB(db,pProgressBar);
	// load sub stat data
	if( !IsHaveChildArmy() )
	{
		CADORecordset rs(&db);
		CString strSql;
		// sub zgw data
		strSql.Format("select * from netsurvey_subzgw_stat where army_id ='%s'",m_data.m_strArmyID);
		bRet=rs.Open(strSql,CADORecordset::openQuery);

		if( bRet && !rs.IsEOF() )
		{
			bRet&=rs.GetFieldValue("not_conn_total",m_subZGW.m_nNotConnTotal);
			bRet&=rs.GetFieldValue("fibre_total",m_subZGW.m_nFibreTotal); 
			bRet&=rs.GetFieldValue("computer_total",m_subZGW.m_lComputerTotal);
			bRet&=rs.GetFieldValue("server_total",m_subZGW.m_nServerTotal);
			bRet&=rs.GetFieldValue("admin_total",m_subZGW.m_nAdminTotal);

			rs.Close();
		}
		for(int i=0;i<sizeof(m_subZGW.m_conn)/sizeof(m_subZGW.m_conn[0]);i++)
		{
			strSql.Format("select * from netsurvey_subzgw_level_stat where army_id='%s' and army_level=%d",m_data.m_strArmyID,i);
			bRet=rs.Open(strSql,CADORecordset::openQuery);
			if( bRet && !rs.IsEOF() )
			{
				bRet&=rs.GetFieldValue("jianzhi_total",m_subZGW.m_conn[i].jianzhiTotal);
				bRet&=rs.GetFieldValue("jianzhi_conn_total",m_subZGW.m_conn[i].jianzhiCTotal); 
				bRet&=rs.GetFieldValue("feijianzhi_total",m_subZGW.m_conn[i].feijianzhiTotal);
				bRet&=rs.GetFieldValue("feijianzhi_conn_total",m_subZGW.m_conn[i].feijianzhiCTotal);

				rs.Close();
			} 
		}
		// sub lw data
		strSql.Format("select * from netsurvey_sublw_stat where army_id='%s'",m_data.m_strArmyID);
		bRet=rs.Open(strSql,CADORecordset::openQuery);
		if( bRet && !rs.IsEOF() )
		{
			bRet&=rs.GetFieldValue("had_total",m_subLW.m_nHadTotal);
			bRet&=rs.GetFieldValue("good_total",m_subLW.m_nGoodTotal); 
			bRet&=rs.GetFieldValue("new_total",m_subLW.m_nNewTotal);
			bRet&=rs.GetFieldValue("bad_total",m_subLW.m_nBadTotal);
			bRet&=rs.GetFieldValue("bad_all_total",m_subLW.m_nBadAllTotal);
			bRet&=rs.GetFieldValue("bad_tianxian_total",m_subLW.m_nBadTianXianTotal);
			bRet&=rs.GetFieldValue("bad_zhijia_total",m_subLW.m_nBadZhiJiaTotal); 
			bRet&=rs.GetFieldValue("bad_gpt_total",m_subLW.m_nBadGPTTotal);
			bRet&=rs.GetFieldValue("bad_tsk_total",m_subLW.m_nBadTSKTotal);
			bRet&=rs.GetFieldValue("computer_total",m_subLW.m_nComputerTotal);
			bRet&=rs.GetFieldValue("server_total",m_subLW.m_nServerTotal);
			bRet&=rs.GetFieldValue("admin_total",m_subLW.m_nAdminTotal);

			rs.Close();
		} 
	}
	return bRet;
}
BOOL CArmy::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("army_id",m_data.m_strArmyID);
	bRet&=rs.GetFieldValue("parent_id",m_data.m_strParentID);	
	bRet&=rs.GetFieldValue("army_name",m_data.m_strArmyName);
	bRet&=rs.GetFieldValue("is_jianzhi",m_data.m_nArmyProp);
	bRet&=rs.GetFieldValue("army_level",m_data.m_nArmyLevel);

	m_strID=m_data.m_strArmyID;
	m_data.m_strArmyName = des.DesCrypteString(_T("auqf_pgf_rd1@#"),m_data.m_strArmyName,0);
	m_strDesc=m_data.m_strArmyName;
	
	return bRet;
}
BOOL CArmy::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from netsurvey_army where army_id='%s'",m_data.m_strArmyID);
		bRet=db.Execute(strSql);
		// delete the sub stat data
		if( !IsHaveChildArmy() && bRet )
		{
			strSql.Format("delete from netsurvey_subzgw_level_stat where army_id='%s'",m_data.m_strArmyID);
			bRet&=db.Execute(strSql);

			strSql.Format("delete from netsurvey_subzgw_stat where army_id='%s'",m_data.m_strArmyID);
			bRet&=db.Execute(strSql);

			strSql.Format("delete from netsurvey_sublw_stat where army_id='%s'",m_data.m_strArmyID);
			bRet&=db.Execute(strSql);
		}
	}

	return bRet;
}	

UINT CArmy::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CArmy::PromptMenuPos()
{
	return 1;
}
void CArmy::UpdateMenu(CMenu *pMenu)
{
	UINT delID=RetrieveMenuItemID(pMenu,_T("ɾ��������Ϣ"));

	ASSERT( delID!=-1  );

	pMenu->EnableMenuItem(delID,(theApp.GetCurUserLevel()>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
}
void CArmy::ShowInTreeView(CCustomTree *pTree,BOOL bChild/*=FALSE*/)
{
	CBaseObject::ShowInTreeView(pTree,bChild);

	if( GetParent()->GetType() != OBJECT_TYPE_ARMYS )
    	((CTreeCtrl *)pTree)->EnsureVisible(m_hItem);
}
void CArmy::ShowListCtrl(CListCtrlEx* pListCtrl)
{
	CBaseObject::ShowListCtrl(pListCtrl);

	CString strText;
	CStringArray strList;

	strText=_T("��������,200;��������,150;���Ӽ���,100;�������Ƿ���ͨ,100");

	pListCtrl->SetHeadings(strText);

	CArmy* pArmy = NULL;
	POSITION pos = m_armys.GetFirstChildPosition();
	while(pos)
	{
		pArmy = (CArmy*)m_armys.GetNextChild(pos);
		if( pArmy )
		{
			strList.RemoveAll();

			strList.Add(pArmy->m_data.m_strArmyName);
			switch(pArmy->m_data.m_nArmyProp)
			{
			case 0:
				strText = _T("�ǽ��Ʋ���");
				break;
			case 1:
				strText = _T("���Ʋ���");
				break;
			case 2:
				strText = _T("���ⲿ��");
				break;
			}
			strList.Add(strText);

			switch(pArmy->m_data.m_nArmyLevel)
			{
			case CNetSurveyApp::junArmy:
				strText = _T("�����ϲ���");
				break;
			case CNetSurveyApp::shilvArmy:
				strText = _T("ʦ�ü�����");
				break;
			case CNetSurveyApp::tuanArmy:
				strText = _T("�ż�����");
				break;
			case CNetSurveyApp::yingArmy:
				strText = _T("Ӫ������");
				break;
			case CNetSurveyApp::lianArmy:
				strText = _T("��������");
				break;
			case CNetSurveyApp::paiArmy:
				strText = _T("�ż�����");
				break;
			case CNetSurveyApp::banArmy:
				strText = _T("�༶����");
				break;
			}
			strList.Add(strText);
			// ��������ͨ���			
			CZGW* pZGW = (CZGW*)pArmy->m_surveys.GetSurveyObject(OBJECT_TYPE_ZGW);
			if( pZGW && pZGW->m_data.m_bConnected )
				strList.Add(_T("��"));
			else
				strList.Add(_T("��"));
				
			pListCtrl->SetItemData(pListCtrl->AddItem(strList),(DWORD)pArmy);
		}		
	}	
}

TiXmlNode*  CArmy::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=new TiXmlElement(_T("netsurvey_army"));

	pTemp->SetAttribute(_T("army_id"),m_data.m_strArmyID);
	pTemp->SetAttribute(_T("parent_id"),m_data.m_strParentID);
	pTemp->SetAttribute(_T("army_name"),des.DesCrypteString(_T("auqf_pgf_rd1@#"),m_data.m_strArmyName,1));
	pTemp->SetAttribute(_T("is_jianzhi"),m_data.m_nArmyProp);
	pTemp->SetAttribute(_T("army_level"),m_data.m_nArmyLevel);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);
	// export sub stat data
	if( !IsHaveChildArmy() )
	{
		if( m_subZGW.IsValid() )
		{
			TiXmlElement * pTemp1=new TiXmlElement(_T("netsurvey_subzgw_stat"));

			pTemp1->SetAttribute(_T("not_conn_total"),m_subZGW.m_nNotConnTotal);
			pTemp1->SetAttribute(_T("fibre_total"),m_subZGW.m_nFibreTotal);
			pTemp1->SetAttribute(_T("computer_total"),m_subZGW.m_lComputerTotal);
			pTemp1->SetAttribute(_T("server_total"),m_subZGW.m_nServerTotal);
			pTemp1->SetAttribute(_T("admin_total"),m_subZGW.m_nAdminTotal);

			pTemp->LinkEndChild(pTemp1);

			for(int i=0;i<sizeof(m_subZGW.m_conn)/sizeof(m_subZGW.m_conn[0]);i++)
			{
				TiXmlElement * pTemp11=new TiXmlElement(_T("netsurvey_subzgw_level_stat"));

				pTemp11->SetAttribute(_T("army_level"),i);
				pTemp11->SetAttribute(_T("jianzhi_total"),m_subZGW.m_conn[i].jianzhiTotal);
				pTemp11->SetAttribute(_T("jianzhi_conn_total"),m_subZGW.m_conn[i].jianzhiCTotal);
				pTemp11->SetAttribute(_T("feijianzhi_total"),m_subZGW.m_conn[i].feijianzhiTotal);
				pTemp11->SetAttribute(_T("feijianzhi_conn_total"),m_subZGW.m_conn[i].feijianzhiCTotal);

				pTemp1->LinkEndChild(pTemp11);
			}

		}
		if( m_subLW.IsValid() )
		{
			TiXmlElement * pTemp2=new TiXmlElement(_T("netsurvey_sublw_stat"));

			pTemp2->SetAttribute(_T("had_total"),m_subLW.m_nHadTotal);
			pTemp2->SetAttribute(_T("good_total"),m_subLW.m_nGoodTotal);
			pTemp2->SetAttribute(_T("new_total"),m_subLW.m_nNewTotal);
			pTemp2->SetAttribute(_T("bad_total"),m_subLW.m_nBadTotal);
			pTemp2->SetAttribute(_T("bad_all_total"),m_subLW.m_nBadAllTotal);
			pTemp2->SetAttribute(_T("bad_tianxian_total"),m_subLW.m_nBadTianXianTotal);
			pTemp2->SetAttribute(_T("bad_zhijia_total"),m_subLW.m_nBadZhiJiaTotal);
			pTemp2->SetAttribute(_T("bad_gpt_total"),m_subLW.m_nBadGPTTotal);
			pTemp2->SetAttribute(_T("bad_tsk_total"),m_subLW.m_nBadTSKTotal);
			pTemp2->SetAttribute(_T("computer_total "),m_subLW.m_nComputerTotal);
			pTemp2->SetAttribute(_T("server_total"),m_subLW.m_nServerTotal);
			pTemp2->SetAttribute(_T("admin_total"),m_subLW.m_nAdminTotal);

			pTemp->LinkEndChild(pTemp2);
		}
	}
	return pTemp;
}
BOOL CArmy::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{	
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);

	CBaseObject *pOB = NULL;
	POSITION pos=GetFirstChildPosition();
	while(pos)
	{
		pOB=GetNextChild(pos);
		if(pOB)
			pOB->StreamIn(pNode,bChild,pProgressBar);
	}
	// import sub stat data
	if( !IsHaveChildArmy() )
	{
		TiXmlElement* pElement=NULL;
		// sub zgw data
		pElement=pNode->FirstChildElement(_T("netsurvey_subzgw_stat"));
		if( pElement )
		{
			pElement->QueryIntAttribute(_T("not_conn_total"),&(m_subZGW.m_nNotConnTotal));
			pElement->QueryIntAttribute(_T("fibre_total"),&(m_subZGW.m_nFibreTotal));
			pElement->QueryIntAttribute(_T("computer_total"),&(int&)(m_subZGW.m_lComputerTotal));
			pElement->QueryIntAttribute(_T("server_total"),&(m_subZGW.m_nServerTotal));
			pElement->QueryIntAttribute(_T("admin_total"),&(m_subZGW.m_nAdminTotal));	

			TiXmlElement* pElement1=NULL;
			
			for(pElement1=pElement->FirstChildElement(_T("netsurvey_subzgw_level_stat"));pElement1;pElement1=pElement1->NextSiblingElement())
			{	
				int i = -1;
				pElement1->QueryIntAttribute(_T("army_level"),&i);

				ASSERT(i>=0);

				pElement1->QueryIntAttribute(_T("jianzhi_total"),&(m_subZGW.m_conn[i].jianzhiTotal));
				pElement1->QueryIntAttribute(_T("jianzhi_conn_total"),&(m_subZGW.m_conn[i].jianzhiCTotal));
				pElement1->QueryIntAttribute(_T("feijianzhi_total"),&(m_subZGW.m_conn[i].feijianzhiTotal));
				pElement1->QueryIntAttribute(_T("feijianzhi_conn_total"),&(m_subZGW.m_conn[i].feijianzhiCTotal));				
			}
		}
		// sub lw data
		pElement=pNode->FirstChildElement(_T("netsurvey_sublw_stat"));
		if( pElement )
		{
			pElement->QueryIntAttribute(_T("had_total"),&(m_subLW.m_nHadTotal));
			pElement->QueryIntAttribute(_T("good_total"),&(m_subLW.m_nGoodTotal));
			pElement->QueryIntAttribute(_T("new_total"),&(m_subLW.m_nNewTotal));
			pElement->QueryIntAttribute(_T("bad_total"),&(m_subLW.m_nBadTotal));
			pElement->QueryIntAttribute(_T("bad_all_total"),&(m_subLW.m_nBadAllTotal));	
			pElement->QueryIntAttribute(_T("bad_tianxian_total"),&(m_subLW.m_nBadTianXianTotal));
			pElement->QueryIntAttribute(_T("bad_zhijia_total"),&(m_subLW.m_nBadZhiJiaTotal));
			pElement->QueryIntAttribute(_T("bad_gpt_total"),&(m_subLW.m_nBadGPTTotal));
			pElement->QueryIntAttribute(_T("bad_tsk_total"),&(m_subLW.m_nBadTSKTotal));
			pElement->QueryIntAttribute(_T("computer_total"),&(m_subLW.m_nComputerTotal));	
			pElement->QueryIntAttribute(_T("server_total"),&(m_subLW.m_nServerTotal));
			pElement->QueryIntAttribute(_T("admin_total"),&(m_subLW.m_nAdminTotal));	
		}
		SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

	}
	return TRUE;
}
void CArmy::CreateSurveys()
{
	m_surveys.CreateSurveys();
}

void CArmy::ReportZGWData(STAT_ZGWData& superZGWData)
{
	CZGW* pZGW = (CZGW*)m_surveys.GetSurveyObject(OBJECT_TYPE_ZGW);	

	if( pZGW )
	{	
		if( !IsXuniArmy() )
		{
			// ͳ�Ƹ�����ͨ��
			int i = GetArmyLevel();

			if( IsJianZhi())
			{
				superZGWData.m_conn[i].jianzhiTotal += (m_subZGW.m_conn[i].jianzhiTotal +1);
				superZGWData.m_conn[i].jianzhiCTotal += (m_subZGW.m_conn[i].jianzhiCTotal + (pZGW->m_data.m_bConnected?1:0));
			}
			else
			{
				superZGWData.m_conn[i].feijianzhiTotal += (m_subZGW.m_conn[i].feijianzhiTotal +1);
				superZGWData.m_conn[i].feijianzhiCTotal += (m_subZGW.m_conn[i].feijianzhiCTotal + (pZGW->m_data.m_bConnected?1:0));
			}
			for(i=i+1;i<5;i++)
			{
				superZGWData.m_conn[i].jianzhiTotal += m_subZGW.m_conn[i].jianzhiTotal;
				superZGWData.m_conn[i].jianzhiCTotal += m_subZGW.m_conn[i].jianzhiCTotal;
				superZGWData.m_conn[i].feijianzhiTotal += m_subZGW.m_conn[i].feijianzhiTotal;
				superZGWData.m_conn[i].feijianzhiCTotal += m_subZGW.m_conn[i].feijianzhiCTotal;
			}
			// ͳ���������
			superZGWData.m_nNotConnTotal += (m_subZGW.m_nNotConnTotal + (pZGW->m_data.m_bConnected?0:1));
			superZGWData.m_nFibreTotal += (m_subZGW.m_nFibreTotal + ((pZGW->m_data.m_bConnected == FALSE && pZGW->m_data.m_bFibre)?1:0));

			superZGWData.m_lComputerTotal += m_subZGW.m_lComputerTotal;
			superZGWData.m_nServerTotal += m_subZGW.m_nServerTotal;
			superZGWData.m_nAdminTotal += m_subZGW.m_nAdminTotal;
		}		
		else
		{
			superZGWData += m_subZGW;			
		}	
		superZGWData.m_lComputerTotal += pZGW->m_data.m_nComputer;
		superZGWData.m_nServerTotal += pZGW->m_data.m_nServer;
		superZGWData.m_nAdminTotal += pZGW->m_data.m_nAdmin;
	}
	else
	{
		superZGWData += m_subZGW;
	}
}
void CArmy::ReportLWData(STAT_LWData& superLWData)
{	
	superLWData += m_subLW;

	CLW* pLW = (CLW*)m_surveys.GetSurveyObject(OBJECT_TYPE_LW);	

	if( pLW )
	{	
		superLWData.m_nHadTotal += (pLW->m_data.m_bHad?1:0);
		superLWData.m_nGoodTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood)?1:0);
		superLWData.m_nNewTotal +=  pLW->m_data.m_nNewNum;

		superLWData.m_nBadTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood == FALSE)?1:0);
		superLWData.m_nBadAllTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood==FALSE && pLW->m_data.m_nBadType==0)?1:0);
		superLWData.m_nBadTianXianTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood==FALSE && pLW->m_data.m_nBadType==1)?1:0);
		superLWData.m_nBadZhiJiaTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood==FALSE && pLW->m_data.m_nBadType==2)?1:0);
		superLWData.m_nBadGPTTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood==FALSE && pLW->m_data.m_nBadType==3)?1:0);
		superLWData.m_nBadTSKTotal += ((pLW->m_data.m_bHad && pLW->m_data.m_bGood==FALSE && pLW->m_data.m_nBadType==4)?1:0);

		superLWData.m_nComputerTotal += pLW->m_data.m_nComputer;
		superLWData.m_nServerTotal += pLW->m_data.m_nServer;
		superLWData.m_nAdminTotal += pLW->m_data.m_nAdmin;	
	}
}

void CArmy::STATData(void)
{
	TRACE("��ʼ����ǰ�ڵ㣺%s\n",GetDesc());

	m_curZGW.Init();
	m_curLW.Init();

	if( IsHaveChildArmy() )
	{
		m_subZGW.Init();
		m_subLW.Init();
	}	

	CBaseObject::STATData();		

	ReportZGWData(m_curZGW);
	ReportLWData(m_curLW);	

	CArmy* pParent = (CArmy*) BackTo(OBJECT_TYPE_ARMY);
	if( pParent )
	{
		TRACE("��ǰͳ�ƽڵ㣺%s  ���ڵ㣺%s\n",GetDesc(),pParent->GetDesc());

		ReportZGWData(pParent->m_subZGW);
		ReportLWData(pParent->m_subLW);
	}
}

void CArmy::ShowInTabbedView(CView* pView)
{
	CTabbedView* pTabView = (CTabbedView*)pView;
	//////////////////////////////////////////////////////////////////////////
	// ������ͳ�����
	CMFCPropertyGridCtrl* pGridCtrl1 = pTabView->GetGridCtrl(0);

	pGridCtrl1->EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));

	HDITEM item;
	item.cxy=300;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	pGridCtrl1->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pGridCtrl1->EnableDescriptionArea();
	pGridCtrl1->SetVSDotNetLook();
	pGridCtrl1->MarkModifiedProperties();
	pGridCtrl1->SetAlphabeticMode(FALSE);
	pGridCtrl1->SetShowDragContext();

	BOOL bEdit = FALSE;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("������λ��������ͨ���")));

	CString str;
	switch ( GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:	
		{
			CMFCPropertyGridProperty* pGroup11 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup11);

			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[0].jianzhiTotal),_T("�����Ͻ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[0].jianzhiCTotal), _T("�����Ͻ��Ƶ�λ��ͨ����������")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[0].feijianzhiTotal), _T("�����Ϸǽ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[0].feijianzhiCTotal), _T("�����Ϸǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_curZGW.m_conn[0].jianzhiTotal <= 0)?0:m_curZGW.m_conn[0].jianzhiCTotal/(float)m_curZGW.m_conn[0].jianzhiTotal*100);
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ͻ��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_curZGW.m_conn[0].feijianzhiTotal <= 0)?0:(m_curZGW.m_conn[0].feijianzhiCTotal/(float)m_curZGW.m_conn[0].feijianzhiTotal*100));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("�����Ϸǽ��Ƶ�λ��������ͨ��")));

			/*pGroup11->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_conn[0].jianzhiTotal));
			pGroup11->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_conn[0].jianzhiCTotal));
			pGroup11->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_conn[0].feijianzhiTotal));
			pGroup11->GetSubItem(3)->SetData((DWORD)&(m_curZGW.m_conn[0].feijianzhiCTotal));*/

			pGroup11->GetSubItem(0)->AllowEdit(bEdit);
			pGroup11->GetSubItem(1)->AllowEdit(bEdit);
			pGroup11->GetSubItem(2)->AllowEdit(bEdit);
			pGroup11->GetSubItem(3)->AllowEdit(bEdit);
			pGroup11->GetSubItem(4)->AllowEdit(FALSE);
			pGroup11->GetSubItem(5)->AllowEdit(FALSE);
			
		}	
	case CNetSurveyApp::shilvArmy:
		{
			CMFCPropertyGridProperty* pGroup12 = new CMFCPropertyGridProperty(_T("ʦ�ü�"));
			apGroup1->AddSubItem(pGroup12);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[1].jianzhiTotal), _T("ʦ�ü����Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[1].jianzhiCTotal), _T("ʦ�ü����Ƶ�λ��ͨ����������")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[1].feijianzhiTotal), _T("ʦ�ü��ǽ��Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[1].feijianzhiCTotal), _T("ʦ�ü��ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_curZGW.m_conn[1].jianzhiTotal <= 0)?0:m_curZGW.m_conn[1].jianzhiCTotal/(float)m_curZGW.m_conn[1].jianzhiTotal*100);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("ʦ�ü����Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_curZGW.m_conn[1].feijianzhiTotal <= 0)?0:(m_curZGW.m_conn[1].feijianzhiCTotal/(float)m_curZGW.m_conn[1].feijianzhiTotal*100));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("ʦ�ü��ǽ��Ƶ�λ��������ͨ��")));

			//pGroup12->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_conn[1].jianzhiTotal));
			//pGroup12->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_conn[1].jianzhiCTotal));
			//pGroup12->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_conn[1].feijianzhiTotal));
			//pGroup12->GetSubItem(3)->SetData((DWORD)&(m_curZGW.m_conn[1].feijianzhiCTotal));

			pGroup12->GetSubItem(0)->AllowEdit(bEdit);
			pGroup12->GetSubItem(1)->AllowEdit(bEdit);
			pGroup12->GetSubItem(2)->AllowEdit(bEdit);
			pGroup12->GetSubItem(3)->AllowEdit(bEdit);
			pGroup12->GetSubItem(4)->AllowEdit(FALSE);
			pGroup12->GetSubItem(5)->AllowEdit(FALSE);			
		}
	case CNetSurveyApp::tuanArmy:
		{
			CMFCPropertyGridProperty* pGroup13 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup13);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[2].jianzhiTotal), _T("�����Ž��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[2].jianzhiCTotal), _T("�����Ž��Ƶ�λ��ͨ����������")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[2].feijianzhiTotal), _T("�����ŷǽ��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[2].feijianzhiCTotal), _T("�����ŷǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_curZGW.m_conn[2].jianzhiTotal <= 0)?0:m_curZGW.m_conn[2].jianzhiCTotal/(float)m_curZGW.m_conn[2].jianzhiTotal*100);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ž��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_curZGW.m_conn[2].feijianzhiTotal <= 0)?0:(m_curZGW.m_conn[2].feijianzhiCTotal/(float)m_curZGW.m_conn[2].feijianzhiTotal*100));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�����ŷǽ��Ƶ�λ��������ͨ��")));			

			//pGroup13->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_conn[2].jianzhiTotal));
			//pGroup13->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_conn[2].jianzhiCTotal));
			//pGroup13->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_conn[2].feijianzhiTotal));
			//pGroup13->GetSubItem(3)->SetData((DWORD)&(m_curZGW.m_conn[2].feijianzhiCTotal));

			pGroup13->GetSubItem(0)->AllowEdit(bEdit);
			pGroup13->GetSubItem(1)->AllowEdit(bEdit);
			pGroup13->GetSubItem(2)->AllowEdit(bEdit);
			pGroup13->GetSubItem(3)->AllowEdit(bEdit);
			pGroup13->GetSubItem(4)->AllowEdit(FALSE);
			pGroup13->GetSubItem(5)->AllowEdit(FALSE);
			
		}
	case CNetSurveyApp::yingArmy:
		{
			CMFCPropertyGridProperty* pGroup14 = new CMFCPropertyGridProperty(_T("����Ӫ"));
			apGroup1->AddSubItem(pGroup14);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[3].jianzhiTotal), _T("����Ӫ���Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[3].jianzhiCTotal), _T("����Ӫ���Ƶ�λ��ͨ����������")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[3].feijianzhiTotal), _T("����Ӫ�ǽ��Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[3].feijianzhiCTotal), _T("����Ӫ�ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_curZGW.m_conn[3].jianzhiTotal <= 0)?0:m_curZGW.m_conn[3].jianzhiCTotal/(float)m_curZGW.m_conn[3].jianzhiTotal*100);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("����Ӫ���Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_curZGW.m_conn[3].feijianzhiTotal <= 0)?0:(m_curZGW.m_conn[3].feijianzhiCTotal/(float)m_curZGW.m_conn[3].feijianzhiTotal*100));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("����Ӫ�ǽ��Ƶ�λ��������ͨ��")));

			//pGroup14->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_conn[3].jianzhiTotal));
			//pGroup14->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_conn[3].jianzhiCTotal));
			//pGroup14->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_conn[3].feijianzhiTotal));
			//pGroup14->GetSubItem(3)->SetData((DWORD)&(m_curZGW.m_conn[3].feijianzhiCTotal));

			pGroup14->GetSubItem(0)->AllowEdit(bEdit);
			pGroup14->GetSubItem(1)->AllowEdit(bEdit);
			pGroup14->GetSubItem(2)->AllowEdit(bEdit);
			pGroup14->GetSubItem(3)->AllowEdit(bEdit);
			pGroup14->GetSubItem(4)->AllowEdit(FALSE);
			pGroup14->GetSubItem(5)->AllowEdit(FALSE);
		}
	case CNetSurveyApp::lianArmy:
		{
			CMFCPropertyGridProperty* pGroup15 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup15);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[4].jianzhiTotal), _T("���������Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[4].jianzhiCTotal), _T("���������Ƶ�λ��ͨ����������")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[4].feijianzhiTotal), _T("�������ǽ��Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_conn[4].feijianzhiCTotal), _T("�������ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_curZGW.m_conn[4].jianzhiTotal <= 0)?0:m_curZGW.m_conn[4].jianzhiCTotal/(float)m_curZGW.m_conn[4].jianzhiTotal*100);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("���������Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_curZGW.m_conn[4].feijianzhiTotal <= 0)?0:(m_curZGW.m_conn[4].feijianzhiCTotal/(float)m_curZGW.m_conn[4].feijianzhiTotal*100));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�������ǽ��Ƶ�λ��������ͨ��")));

			//pGroup15->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_conn[4].jianzhiTotal));
			//pGroup15->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_conn[4].jianzhiCTotal));
			//pGroup15->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_conn[4].feijianzhiTotal));
			//pGroup15->GetSubItem(3)->SetData((DWORD)&(m_curZGW.m_conn[4].feijianzhiCTotal));

			pGroup15->GetSubItem(0)->AllowEdit(bEdit);
			pGroup15->GetSubItem(1)->AllowEdit(bEdit);
			pGroup15->GetSubItem(2)->AllowEdit(bEdit);
			pGroup15->GetSubItem(3)->AllowEdit(bEdit);
			pGroup15->GetSubItem(4)->AllowEdit(FALSE);
			pGroup15->GetSubItem(5)->AllowEdit(FALSE);
		}
	}
	apGroup1->Expand();
	pGridCtrl1->AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ���˸������ͳ��")));

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ����"), CCommonFunction::Int2OleVariant(m_curZGW.m_nNotConnTotal),_T("δ��ͨ�������ĵ�λ����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��ɹ��˸��������"), CCommonFunction::Int2OleVariant(m_curZGW.m_nFibreTotal), _T("��ɹ��˸��������")));

	//apGroup2->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_nNotConnTotal));
	//apGroup2->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_nFibreTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);

	pGridCtrl1->AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("��������Դ���ͳ��")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), COleVariant(m_curZGW.m_lComputerTotal), _T("��ͨ�������ĵ����ն�������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(m_curZGW.m_nServerTotal),_T("������������������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(m_curZGW.m_nAdminTotal), _T("������������Ա������")));

	//apGroup3->GetSubItem(0)->SetData((DWORD)&(m_curZGW.m_nComputerTotal));
	//apGroup3->GetSubItem(1)->SetData((DWORD)&(m_curZGW.m_nServerTotal));
	//apGroup3->GetSubItem(2)->SetData((DWORD)&(m_curZGW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	pGridCtrl1->AddProperty(apGroup3.release());

	//////////////////////////////////////////////////////////////////////////
	// ����ͳ�����
	CMFCPropertyGridCtrl* pGridCtrl2 = pTabView->GetGridCtrl(1);
	pGridCtrl2->EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));
	
	item.cxy=300;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	pGridCtrl2->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pGridCtrl2->EnableDescriptionArea();
	pGridCtrl2->SetVSDotNetLook();
	pGridCtrl2->MarkModifiedProperties();
	pGridCtrl2->SetAlphabeticMode(FALSE);
	pGridCtrl2->SetShowDragContext();


	std::auto_ptr<CMFCPropertyGridProperty> apLWGroup1(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	

	apLWGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("�����Ѱ�װ������"), CCommonFunction::Int2OleVariant(m_curLW.m_nHadTotal), _T("�����Ѱ�װ������")));
	apLWGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("������������������"), CCommonFunction::Int2OleVariant(m_curLW.m_nGoodTotal), _T("������������������")));
	apLWGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��Ҫ����������"), CCommonFunction::Int2OleVariant(m_curLW.m_nNewTotal), _T("��������������")));

	//apLWGroup1->GetSubItem(0)->SetData((DWORD)&(m_curLW.m_nHadTotal));
	//apLWGroup1->GetSubItem(1)->SetData((DWORD)&(m_curLW.m_nGoodTotal));
	//apLWGroup1->GetSubItem(2)->SetData((DWORD)&(m_curLW.m_nNewTotal));

	apLWGroup1->GetSubItem(0)->AllowEdit(bEdit);
	apLWGroup1->GetSubItem(1)->AllowEdit(bEdit);
	apLWGroup1->GetSubItem(2)->AllowEdit(bEdit);

	pGridCtrl2->AddProperty(apLWGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apLWGroup2(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	

	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�������ܹ��ϵ�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadTotal), _T("�������ܹ��ϵ�����")));
	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ȫ���𻵵�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadAllTotal), _T("ȫ���𻵵�����")));
	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�����𻵵�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadTianXianTotal), _T("�����𻵵�����")));
	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("֧���𻵵�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadZhiJiaTotal), _T("֧���𻵵�����")));
	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��Ƶͷ�𻵵�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadGPTTotal), _T("��Ƶͷ�𻵵�����")));
	apLWGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ͨ�ӿ��𻵵�����"), CCommonFunction::Int2OleVariant(m_curLW.m_nBadTSKTotal), _T("ͨ�ӿ��𻵵�����")));

	//apLWGroup2->GetSubItem(0)->SetData((DWORD)&(m_curLW.m_nBadTotal));
	//apLWGroup2->GetSubItem(1)->SetData((DWORD)&(m_curLW.m_nBadAllTotal));
	//apLWGroup2->GetSubItem(2)->SetData((DWORD)&(m_curLW.m_nBadTianXianTotal));
	//apLWGroup2->GetSubItem(3)->SetData((DWORD)&(m_curLW.m_nBadZhiJiaTotal));
	//apLWGroup2->GetSubItem(4)->SetData((DWORD)&(m_curLW.m_nBadGPTTotal));
	//apLWGroup2->GetSubItem(5)->SetData((DWORD)&(m_curLW.m_nBadTSKTotal));

	apLWGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apLWGroup2->GetSubItem(1)->AllowEdit(bEdit);
	apLWGroup2->GetSubItem(2)->AllowEdit(bEdit);
	apLWGroup2->GetSubItem(3)->AllowEdit(bEdit);
	apLWGroup2->GetSubItem(4)->AllowEdit(bEdit);
	apLWGroup2->GetSubItem(5)->AllowEdit(bEdit);

	pGridCtrl2->AddProperty(apLWGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apLWGroup3(new CMFCPropertyGridProperty(_T("������Դ���ͳ��")));

	apLWGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), CCommonFunction::Int2OleVariant(m_curLW.m_nComputerTotal), _T("��ͨ�����ĵ����ն�������")));
	apLWGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(m_curLW.m_nServerTotal), _T("����������������")));
	apLWGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(m_curLW.m_nAdminTotal), _T("����������Ա������")));

	//apLWGroup3->GetSubItem(0)->SetData((DWORD)&(m_curLW.m_nComputerTotal));
	//apLWGroup3->GetSubItem(1)->SetData((DWORD)&(m_curLW.m_nServerTotal));
	//apLWGroup3->GetSubItem(2)->SetData((DWORD)&(m_curLW.m_nAdminTotal));

	apLWGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apLWGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apLWGroup3->GetSubItem(2)->AllowEdit(bEdit);

	pGridCtrl2->AddProperty(apLWGroup3.release());
}

void CArmy::GetZGWReportData(CStringArray& data)
{
	data.RemoveAll();
	//�������� 
	data.Add(GetDesc());
	//���Ӽ���
	CString str = _T("");
	switch( GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:
		str = _T("������");
		break;
	case CNetSurveyApp::shilvArmy:
		str = _T("ʦ�ü�");
		break;
	case CNetSurveyApp::tuanArmy:
		str = _T("�ż�");
		break;
	case CNetSurveyApp::yingArmy:
		str = _T("Ӫ��");
		break;
	case CNetSurveyApp::lianArmy:
		str = _T("����");
		break;
	case CNetSurveyApp::paiArmy:
		str = _T("�ż�");
		break;
	case CNetSurveyApp::banArmy:
		str = _T("�༶");
		break;
	}
	data.Add(str);
	// ��������������ͳ��
	CZGW* pZGW = (CZGW*)m_surveys.GetSurveyObject(OBJECT_TYPE_ZGW);	
	if( pZGW )
	{	
		data.Add( pZGW->m_data.m_bConnected?_T("��"):_T("��") );
		data.Add( (pZGW->m_data.m_bConnected == FALSE && pZGW->m_data.m_bFibre )?_T("��"):_T("��") );
		data.Add( pZGW->m_data.m_bOnlyOne?_T("��������"):_T("����������") );
		str.Format("%d��",pZGW->m_data.m_nBW);
		data.Add( str );
		str.Format("%d",pZGW->m_data.m_nComputer);
		data.Add( str );
		str.Format("%d",pZGW->m_data.m_nServer);
		data.Add( str );
		str.Format("%d",pZGW->m_data.m_nAdmin);
		data.Add( str );
		str.Format("%d",pZGW->m_data.m_nRoom);
		data.Add( str );
		str = pZGW->m_data.m_strWebsite;
		data.Add( str );
		str =  pZGW->m_data.m_strIP;
		data.Add( str );
	}
	else
	{
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
	}
	// �����������¼���������ͳ��	
	for( int i=0;i<sizeof(m_curZGW.m_conn)/sizeof(m_curZGW.m_conn[0]);i++)
	{
		str.Format("%.1f%%",((m_curZGW.m_conn[i].jianzhiTotal+m_curZGW.m_conn[i].feijianzhiTotal) <= 0)?0:(m_curZGW.m_conn[i].jianzhiCTotal+m_curZGW.m_conn[i].feijianzhiCTotal)/(float)(m_curZGW.m_conn[i].jianzhiTotal+m_curZGW.m_conn[i].feijianzhiTotal)*100);
		data.Add( str );
	}
	str.Format("%d",m_curZGW.m_nNotConnTotal);
	data.Add( str );
	str.Format("%d",m_curZGW.m_nFibreTotal);
	data.Add( str );
	str.Format("%ld",m_curZGW.m_lComputerTotal);
	data.Add( str );
	str.Format("%d",m_curZGW.m_nServerTotal);
	data.Add( str );
	str.Format("%d",m_curZGW.m_nAdminTotal);
	data.Add( str );
}
void CArmy::GetLWReportData(CStringArray& data)
{
	data.RemoveAll();

	data.Add(GetDesc());
	//���Ӽ���
	CString str = _T("");
	switch( GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:
		str = _T("������");
		break;
	case CNetSurveyApp::shilvArmy:
		str = _T("ʦ�ü�");
		break;
	case CNetSurveyApp::tuanArmy:
		str = _T("�ż�");
		break;
	case CNetSurveyApp::yingArmy:
		str = _T("Ӫ��");
		break;
	case CNetSurveyApp::lianArmy:
		str = _T("����");
		break;
	case CNetSurveyApp::paiArmy:
		str = _T("�ż�");
		break;
	case CNetSurveyApp::banArmy:
		str = _T("�༶");
		break;
	}
	data.Add(str);
	// ������������ͳ��
	CLW* pLW = (CLW*)m_surveys.GetSurveyObject(OBJECT_TYPE_LW);	
	if( pLW )
	{
		data.Add( pLW->m_data.m_bHad?_T("��"):_T("��") );
		data.Add( (pLW->m_data.m_bHad && pLW->m_data.m_bGood )?_T("��"):_T("��") );

		str = _T("");
		if(pLW->m_data.m_bHad && !pLW->m_data.m_bGood )
		{
			switch( pLW->m_data.m_nBadType )
			{
				// ��������ԭ��0Ϊ��ȫ�𻵣�1Ϊ���ߣ�2Ϊ֧�ܣ�3Ϊ��Ƶͷ��4Ϊͨ�ӿ�
			case 0:
				str = _T("��ȫ��");
				break;
			case 1:
				str = _T("������");
				break;
			case 2:
				str = _T("֧����");
				break;
			case 3:
				str = _T("��Ƶͷ��");
				break;
			case 4:
				str = _T("ͨ�ӿ���");
				break;
			}
		}

		data.Add(str);

		data.Add( pLW->m_data.m_bOnlyOne?_T("��������"):_T("����������") );
		str.Format("%d",pLW->m_data.m_nNewNum);
		data.Add( str );
		str.Format("%d",pLW->m_data.m_nComputer);
		data.Add( str );
		str.Format("%d",pLW->m_data.m_nServer);
		data.Add( str );
		str.Format("%d",pLW->m_data.m_nAdmin);
		data.Add( str );
		str.Format("%d",pLW->m_data.m_nRoom);
		data.Add( str );
	}
	else
	{
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
		data.Add( _T("") );
	}
	// �����������¼���������ͳ��
	str.Format("%d",m_curLW.m_nHadTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nGoodTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nNewTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadAllTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadTianXianTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadZhiJiaTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadGPTTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nBadTSKTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nComputerTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nServerTotal);
	data.Add( str );
	str.Format("%d",m_curLW.m_nAdminTotal);
	data.Add( str );
}