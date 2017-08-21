#include "StdAfx.h"
#include "ZGW.h"
#include "NetSurvey.h"
#include "Surveys.h"
#include "ZGWDlg.h"

CZGW::CZGW(void):CBaseObject(OBJECT_TYPE_ZGW)
{
//	m_bCanDelete = FALSE;
	SetImg(34,35);
	m_strDesc = _T("政工网建设情况");
}


CZGW::~CZGW(void)
{
}
void CZGW::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strZGWID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CZGW::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	return bAccept;
}
void CZGW::DoJoin(CBaseObject *pParent)
{	
	if( pParent != NULL && pParent->GetType() == OBJECT_TYPE_SURVEYS)
	{
		m_data.m_strArmyID = static_cast<CSurveys*>(pParent)->GetArmyID();
	}

	CBaseObject::DoJoin(this);
}		
void CZGW::DoLeave(CBaseObject *pParent)
{
	m_data.m_strArmyID.Empty();

	CBaseObject::DoLeave(this);
}
UINT CZGW::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CZGWDlg dlg(this);

	dlg.SetValue(m_data);

	nRet=dlg.DoModal();

	if( nRet== IDOK)
	{	
		if(dlg.GetValue(m_data))
		{
			SetModify();
		}	
	}	

	return nRet;
}
BOOL CZGW::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

	strSql.Format("select * from netsurvey_zgw where zgw_id='%s'",m_data.m_strZGWID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("zgw_id",m_data.m_strZGWID);
		}
		else
			rs.Edit();	

		bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
		bRet&=rs.SetFieldValue("is_connected",(int)m_data.m_bConnected); 
		bRet&=rs.SetFieldValue("is_fibre",(int)m_data.m_bFibre);
		bRet&=rs.SetFieldValue("is_onlyone",(int)m_data.m_bOnlyOne);
		bRet&=rs.SetFieldValue("zgw_ip",m_data.m_strIP);
		bRet&=rs.SetFieldValue("zgw_bw",m_data.m_nBW);
		bRet&=rs.SetFieldValue("zgw_computer",m_data.m_nComputer);
		bRet&=rs.SetFieldValue("zgw_server",m_data.m_nServer);
		bRet&=rs.SetFieldValue("zgw_admin",m_data.m_nAdmin);
		bRet&=rs.SetFieldValue("zgw_room",m_data.m_nRoom);
		bRet&=rs.SetFieldValue("zgw_website",m_data.m_strWebsite);

		bRet&=rs.Update();	

		rs.Close();
	} 
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	

BOOL CZGW::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("zgw_id",m_data.m_strZGWID);
	bRet&=rs.GetFieldValue("army_id",m_data.m_strArmyID);
	bRet&=rs.GetFieldValue("is_connected",m_data.m_bConnected);	
	bRet&=rs.GetFieldValue("is_fibre",m_data.m_bFibre);
	bRet&=rs.GetFieldValue("is_onlyone",m_data.m_bOnlyOne);
	bRet&=rs.GetFieldValue("zgw_ip",m_data.m_strIP);
	bRet&=rs.GetFieldValue("zgw_bw",m_data.m_nBW);
	bRet&=rs.GetFieldValue("zgw_computer",m_data.m_nComputer);
	bRet&=rs.GetFieldValue("zgw_server",m_data.m_nServer);
	bRet&=rs.GetFieldValue("zgw_admin",m_data.m_nAdmin);
	bRet&=rs.GetFieldValue("zgw_room",m_data.m_nRoom);
	bRet&=rs.GetFieldValue("zgw_website",m_data.m_strWebsite);

	m_strID=m_data.m_strZGWID;

	return bRet;
}
BOOL CZGW::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from netsurvey_zgw where zgw_id='%s'",m_data.m_strZGWID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CZGW::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CZGW::PromptMenuPos()
{
	return 2;
}
void CZGW::UpdateMenu(CMenu *pMenu)
{
	//UINT delID=RetrieveMenuItemID(pMenu,_T("删除政工网信息"));

	//ASSERT( delID!=-1  );

	//	int nLevel=theApp.GetUserLevel();	
	//	pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
}


TiXmlNode*  CZGW::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	TiXmlElement * pTemp=new TiXmlElement(_T("netsurvey_zgw"));

	pTemp->SetAttribute(_T("zgw_id"),m_data.m_strZGWID);
	pTemp->SetAttribute(_T("army_id"),m_data.m_strArmyID);
	pTemp->SetAttribute(_T("is_connected"),(int)m_data.m_bConnected);
	pTemp->SetAttribute(_T("is_fibre"),(int)m_data.m_bFibre);
	pTemp->SetAttribute(_T("is_onlyone"),(int)m_data.m_bOnlyOne);
	pTemp->SetAttribute(_T("zgw_ip"),m_data.m_strIP);
	pTemp->SetAttribute(_T("zgw_bw"),m_data.m_nBW);
	pTemp->SetAttribute(_T("zgw_computer"),m_data.m_nComputer);
	pTemp->SetAttribute(_T("zgw_server"),m_data.m_nServer);
	pTemp->SetAttribute(_T("zgw_admin"),m_data.m_nAdmin);
	pTemp->SetAttribute(_T("zgw_room"),m_data.m_nRoom);
	pTemp->SetAttribute(_T("zgw_website"),m_data.m_strWebsite);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,FALSE,pProgressBar);

	return pTemp;
}
