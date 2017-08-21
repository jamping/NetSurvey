#include "StdAfx.h"
#include "Surveys.h"
#include "NetSurvey.h"
#include "Army.h"
#include "ZGW.h"
#include "LW.h"


CSurveys::CSurveys(void):CBaseObject(OBJECT_TYPE_SURVEYS)
{
	m_bCanDelete = FALSE;
	m_strArmyID = _T("");

	SetImg(24,25);
	SetID(_T("本级普查内容"));
}


CSurveys::~CSurveys(void)
{
}
void CSurveys::CreateSurveys()
{
	CZGW* pZGW = new CZGW;
	pZGW->SetID(CCommonFunction::AllocateID());
	pZGW->AddTo(this);

	CLW* pLW = new CLW;
	pLW->SetID(CCommonFunction::AllocateID());
	pLW->AddTo(this);
}

BOOL CSurveys::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{	
	case OBJECT_TYPE_ZGW:	//政工网
		bAccept=TRUE;
		break;
	case OBJECT_TYPE_LW:	//蓝网
		bAccept=TRUE;
		break;
	}

	return bAccept;
}
void CSurveys::DoJoin(CBaseObject *pParent)
{
	if( pParent!=NULL && pParent->GetType() == OBJECT_TYPE_ARMY )
	{
		m_strArmyID = pParent->GetID();	
		m_nArmyLevel = static_cast<CArmy*>(pParent)->GetArmyLevel();
	}

	CBaseObject::DoJoin(this);
}		
void CSurveys::DoLeave(CBaseObject *pParent)
{
	m_strArmyID.Empty();
	m_nArmyLevel = CNetSurveyApp::unknowArmy;

	CBaseObject::DoLeave(this);
}
BOOL CSurveys::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{	
	CADORecordset rs(&db);
	CString strSql;

	// 读取政工网信息
	strSql.Format("select * from netsurvey_zgw where army_id='%s'",m_strArmyID);

	if(rs.Open(strSql,CADORecordset::openQuery) && !rs.IsEOF())
	{
		CZGW* pZGW = new CZGW;
		pZGW->LoadInfo(rs);
		pZGW->AddTo(this);

		rs.Close();
	}
	
	// 读取蓝网信息
	strSql.Format("select * from netsurvey_lw where army_id='%s'",m_strArmyID);
	if(rs.Open(strSql,CADORecordset::openQuery) && !rs.IsEOF())
	{
		CLW* pLW = new CLW;
		pLW->LoadInfo(rs);
		pLW->AddTo(this);

		rs.Close();
	}	

	return CBaseObject::LoadFromDB(db,pProgressBar);
}
UINT CSurveys::PromptMenuID()
{
	return -1;//IDR_TREE_MENU;
}
int CSurveys::PromptMenuPos()
{
	return -1;
}
void CSurveys::UpdateMenu(CMenu *pMenu)
{
	//UINT delID=RetrieveMenuItemID(pMenu,_T("删除"));

	//ASSERT( delID!=-1  );

	//int nLevel=theApp.GetUserLevel();	
	//pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
	//pMenu->EnableMenuItem(delID,(GetChildCount()>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));
	//pMenu->ModifyMenu(delID,MF_BYCOMMAND,delID,_T("删除所有普查信息"));

}

BOOL CSurveys::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);

	TiXmlElement* pElement=NULL;
	CString strID;
	// 读取政工网信息
	CZGW* pZGW = NULL;
	//for(pElement=pNode->FirstChildElement(_T("netsurvey_zgw"));pElement;pElement=pElement->NextSiblingElement())
	pElement=pNode->FirstChildElement(_T("netsurvey_zgw"));
	if( pElement )
	{
		strID=pElement->Attribute(_T("zgw_id"));
		if(!strID.IsEmpty())
		{
			pZGW=(CZGW*)FindSubObject(OBJECT_TYPE_ZGW,strID);
			if(pZGW==NULL)
			{
				pZGW=new CZGW;
				pZGW->m_data.m_strZGWID = strID;
				pZGW->AddTo(this);
			}				
			pZGW->m_data.m_strArmyID = pElement->Attribute(_T("army_id"));
			pElement->QueryIntAttribute(_T("is_connected"),&((int)pZGW->m_data.m_bConnected));
			pElement->QueryIntAttribute(_T("is_fibre"),&((int)pZGW->m_data.m_bFibre));
			pElement->QueryIntAttribute(_T("is_onlyone"),&((int)pZGW->m_data.m_bOnlyOne));
			pZGW->m_data.m_strIP = pElement->Attribute(_T("zgw_ip"));
			pElement->QueryIntAttribute(_T("zgw_bw"),&(pZGW->m_data.m_nBW));
			pElement->QueryIntAttribute(_T("zgw_computer"),&(pZGW->m_data.m_nComputer));
			pElement->QueryIntAttribute(_T("zgw_server"),&(pZGW->m_data.m_nServer));
			pElement->QueryIntAttribute(_T("zgw_admin"),&(pZGW->m_data.m_nAdmin));
			pElement->QueryIntAttribute(_T("zgw_room"),&(pZGW->m_data.m_nRoom));
			pZGW->m_data.m_strWebsite=pElement->Attribute(_T("zgw_website"));

			pZGW->SetID(strID);
			pZGW->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
		}
	}
	// 读取蓝网信息
	CLW* pLW = NULL;
	//for(pElement=pNode->FirstChildElement(_T("netsurvey_lw"));pElement;pElement=pElement->NextSiblingElement())
	pElement=pNode->FirstChildElement(_T("netsurvey_lw"));
	if( pElement )
	{
		strID=pElement->Attribute(_T("lw_id"));
		if(!strID.IsEmpty())
		{
			pLW = (CLW*)FindSubObject(OBJECT_TYPE_LW,strID);
			if(pLW==NULL)
			{
				pLW=new CLW;
				pLW->m_data.m_strLWID = strID;
				pLW->AddTo(this);
			}	
			pLW->m_data.m_strArmyID = pElement->Attribute(_T("army_id"));
			pElement->QueryIntAttribute(_T("is_have"),&((int)pLW->m_data.m_bHad));			
			pElement->QueryIntAttribute(_T("is_onlyone"),&((int)pLW->m_data.m_bOnlyOne));
			pElement->QueryIntAttribute(_T("is_good"),&((int)pLW->m_data.m_bGood));
			pElement->QueryIntAttribute(_T("bad_type"),&(pLW->m_data.m_nBadType));
			pElement->QueryIntAttribute(_T("lw_computer"),&(pLW->m_data.m_nComputer));
			pElement->QueryIntAttribute(_T("lw_server"),&(pLW->m_data.m_nServer));
			pElement->QueryIntAttribute(_T("lw_admin"),&(pLW->m_data.m_nAdmin));
			pElement->QueryIntAttribute(_T("lw_room"),&(pLW->m_data.m_nRoom));
			pElement->QueryIntAttribute(_T("lw_newnum"),&(pLW->m_data.m_nNewNum));
			
			pLW->SetID(strID);
			pLW->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);
		}
	}
	return TRUE;
}

CBaseObject* CSurveys::GetSurveyObject(OBJECT_TYPE type)
{
	CBaseObject* pOb =NULL;
	POSITION pos = GetFirstChildPosition();
	while( pos )
	{
		pOb = GetNextChild(pos);
		if( pOb && pOb->GetType() == type )
		{			
			break;
		}
	}	
	return pOb;
}