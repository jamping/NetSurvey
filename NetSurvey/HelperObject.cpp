// HelperObject.cpp: implementation of the CHelperObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "netsurvey.h"
#include "HelperObject.h"
#include "Army.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct HELPEROBJECT_TAG
{
	LPCSTR strID;		//
	LPCSTR strDesc;		//
	int nImg;
	int nSelImg;
	UINT nPromptMenuID;	//
	int nPromptMenuPos;	//
}
	g_HelpObjects[] =
{
	/* 0 - HELPER_SOURCE	*/	{ "0","部队", 0,1, IDR_TREE_MENU, 0 }	
};


CHelperObject::CHelperObject(int nClass) : CBaseObject(OBJECT_TYPE_HELPER)
{
	m_bCanDelete=FALSE;
	m_nClass=nClass;
	m_nImg=g_HelpObjects[m_nClass].nImg;
	m_nSelImg=g_HelpObjects[m_nClass].nSelImg;
	m_strID=g_HelpObjects[m_nClass].strID;
	m_strDesc=g_HelpObjects[m_nClass].strDesc;
}

BOOL CHelperObject::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

/*
	HELPER_SOURCE=0,         // 海军
*/
	int nObType,nHelperClass;
	nObType=pOB->GetType();
	if(nObType == OBJECT_TYPE_HELPER)
		nHelperClass=((CHelperObject *)pOB)->m_nClass;

	switch(m_nClass)
	{
	case HELPER_SOURCE:
		if(nObType == OBJECT_TYPE_ARMY)	//数据源
			bAccept=TRUE;
		break;
	}

	return bAccept;
}

CHelperObject::~CHelperObject()
{

}

UINT CHelperObject::PromptMenuID()
{
	return g_HelpObjects[m_nClass].nPromptMenuID;
}

int CHelperObject::PromptMenuPos()		
{
	return g_HelpObjects[m_nClass].nPromptMenuPos;
}

void CHelperObject::UpdateMenu(CMenu *pMenu)
{	
	UINT delID=RetrieveMenuItemID(pMenu,_T("删除部队信息"));

	ASSERT( delID!=-1  );

	int nLevel=theApp.GetCurUserLevel();	
	pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
}
BOOL CHelperObject::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	return CBaseObject::DeleteFromDB(db,TRUE,pProgressBar);
}	
int CHelperObject::GetHelpType()
{
     return m_nClass;
}
void CHelperObject::ShowListCtrl(CListCtrlEx* pListCtrl)
{

}

TiXmlNode*  CHelperObject::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
    TiXmlElement * pTemp=NULL;
	
	pTemp=new TiXmlElement(_T("netsurvey_root"));
//	pTemp->SetAttribute(_T("name"),GetDesc());
	pTemp->SetAttribute(_T("type"),GetHelpType());
	
	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,bChild,pProgressBar);

	return pTemp;
}

BOOL CHelperObject::StreamIn(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	CBaseObject::StreamIn(pNode,bChild,pProgressBar);
	
	TiXmlElement* pElement=NULL;
	CString strID;
	int nType=-1;
	TiXmlHandle handle(pNode);
    handle.ToElement()->QueryIntAttribute(_T("type"),&nType);

	switch(nType)
	{
	case HELPER_SOURCE:
		{
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
					pElement->QueryIntAttribute(_T("army_level"),&((int&)pArmy->m_data.m_nArmyLevel));

					pArmy->SetID(strID,pArmy->m_data.m_strArmyName);					
					pArmy->SaveToDB(*theApp.GetDB(),FALSE,pProgressBar);

					pArmy->StreamIn(pElement,bChild,pProgressBar);
				}
			}
		}
		break;
	
	default:
		ASSERT(FALSE);
	}
	return TRUE;
}
