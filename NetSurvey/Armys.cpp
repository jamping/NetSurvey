#include "StdAfx.h"
#include "NetSurvey.h"
#include "Armys.h"
#include "Army.h"
#include "ZGW.h"

CArmys::CArmys(void):CBaseObject(OBJECT_TYPE_ARMYS)
{
	m_bCanDelete = FALSE;
	m_strArmyID = _T("");

	SetImg(12,13);
	SetID(_T("�����¼�����"));
}


CArmys::~CArmys(void)
{
}
BOOL CArmys::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	switch(pOB->GetType())
	{
	case OBJECT_TYPE_ARMY:
		bAccept=TRUE;        //�¼�����
		break;	
	}

	return bAccept;
}
void CArmys::DoJoin(CBaseObject *pParent)
{
	if( pParent!=NULL && pParent->GetType() == OBJECT_TYPE_ARMY )
		m_strArmyID = pParent->GetID();	

	CBaseObject::DoJoin(this);
}		
void CArmys::DoLeave(CBaseObject *pParent)
{
	m_strArmyID.Empty();

	CBaseObject::DoLeave(this);
}
BOOL CArmys::LoadFromDB(CADODatabase &db,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=TRUE;
	CADORecordset rs(&db);
	CString strSql;

	// Load sub armys
	strSql.Format("select * from netsurvey_army where parent_id='%s'",m_strArmyID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	CArmy* pArmy=NULL;

	if(bRet)
	{
		while (!rs.IsEOF())
		{
			pArmy=new CArmy;
			pArmy->LoadInfo(rs);
			pArmy->AddTo(this);

			rs.MoveNext();
		}	
		rs.Close();
	}
	
	return CBaseObject::LoadFromDB(db,pProgressBar);
}
UINT CArmys::PromptMenuID()
{
	return -1;//IDR_TREE_MENU;
}
int CArmys::PromptMenuPos()
{
	return -1;//4;
}
void CArmys::UpdateMenu(CMenu *pMenu)
{	
	//UINT delID=RetrieveMenuItemID(pMenu,_T("ɾ��"));

	//ASSERT( delID!=-1  );

	//int nLevel=theApp.GetUserLevel();	
	//   pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
	/*pMenu->ModifyMenu(delID,MF_BYCOMMAND,delID,_T("ɾ�����в�����Ϣ"));
	pMenu->EnableMenuItem(delID,(GetChildCount()>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));*/
	

}
void CArmys::ShowListCtrl(CListCtrlEx* pListCtrl)
{
	CBaseObject::ShowListCtrl(pListCtrl);

	CString strText;
	CStringArray strList;

	strText=_T("��������,200;��������,150;���Ӽ���,100;�������Ƿ���ͨ,100");

	pListCtrl->SetHeadings(strText);

	CArmy* pArmy = NULL;
	POSITION pos = GetFirstChildPosition();
	while(pos)
	{
		pArmy = (CArmy*)GetNextChild(pos);
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
TiXmlNode*  CArmys::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamOut(pNode,bChild,pProgressBar);

	return pNode;
}
BOOL CArmys::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);

	TiXmlElement* pElement=NULL;
	CString strID;
	CArmy* pArmy=NULL;
	for(pElement=pNode->FirstChildElement(_T("netsurvey_army"));pElement;pElement=pElement->NextSiblingElement())
	{
		strID=pElement->Attribute(_T("army_id"));
		if(!strID.IsEmpty())
		{
			pArmy=(CArmy*)FindSubObject(OBJECT_TYPE_ARMY,strID);
			if(pArmy==NULL)
			{
				pArmy=new CArmy;
				pArmy->m_data.m_strArmyID=strID;
				pArmy->AddTo(this);
			}			
			pArmy->m_data.m_strParentID=pElement->Attribute(_T("parent_id"));
			CDes des;
			pArmy->m_data.m_strArmyName = des.DesCrypteString(_T("auqf_pgf_rd1@#"),pElement->Attribute(_T("army_name")),0);
			pElement->QueryIntAttribute(_T("is_jianzhi"),&(pArmy->m_data.m_nArmyProp));
			pElement->QueryIntAttribute(_T("army_level"),&(pArmy->m_data.m_nArmyLevel));			

			pArmy->SetID(strID,pArmy->m_data.m_strArmyName);
			pArmy->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

			pArmy->StreamIn(pElement,bChild,pProgressBar);
		}

	}
	return TRUE;
}
