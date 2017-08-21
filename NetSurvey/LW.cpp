#include "StdAfx.h"
#include "LW.h"
#include "NetSurvey.h"
#include "Surveys.h"
#include "LWDlg.h"


CLW::CLW(void):CBaseObject(OBJECT_TYPE_LW)
{
//	m_bCanDelete = FALSE;
	SetImg(32,33);
	m_strDesc = _T("蓝网建设情况");
}


CLW::~CLW(void)
{
}
void CLW::SetID(LPCSTR strID,LPCSTR strDesc/* =NULL */)
{
	m_data.m_strLWID=strID;

	CBaseObject::SetID(strID,strDesc);
}
BOOL CLW::CanAccept(CBaseObject *pOB,CCustomTree *pTree)
{
	BOOL bAccept=FALSE;

	return bAccept;
}
void CLW::DoJoin(CBaseObject *pParent)
{
	if( pParent != NULL && pParent->GetType() == OBJECT_TYPE_SURVEYS)
	     m_data.m_strArmyID=static_cast<CSurveys*>(pParent)->GetArmyID();	

	CBaseObject::DoJoin(this);
}		
void CLW::DoLeave(CBaseObject *pParent)
{
	m_data.m_strArmyID.Empty();
	CBaseObject::DoLeave(this);
}
UINT CLW::EditObject(CWnd *pParent,BOOL bReadOnly/*=TRUE*/)
{
	UINT nRet;
	CLWDlg dlg(this);

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
BOOL CLW::SaveToDB(CADODatabase &db,BOOL bChild/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{  
	BOOL bRet=TRUE;
	CString strSql;
	CADORecordset rs(&db);

	strSql.Format("select * from netsurvey_lw where lw_id='%s'",m_data.m_strLWID);
	bRet=rs.Open(strSql,CADORecordset::openQuery);
	if(bRet)
	{
		if(rs.GetRecordCount()==0)
		{
			bRet&=rs.AddNew();
			bRet&=rs.SetFieldValue("lw_id",m_data.m_strLWID);
		}
		else
			rs.Edit();	

		bRet&=rs.SetFieldValue("army_id",m_data.m_strArmyID);
		bRet&=rs.SetFieldValue("is_have",(int)m_data.m_bHad); 
		bRet&=rs.SetFieldValue("is_onlyone",(int)m_data.m_bOnlyOne);
		bRet&=rs.SetFieldValue("is_good",(int)m_data.m_bGood);
		bRet&=rs.SetFieldValue("bad_type",m_data.m_nBadType);
		bRet&=rs.SetFieldValue("lw_computer",m_data.m_nComputer);
		bRet&=rs.SetFieldValue("lw_server",m_data.m_nServer);
		bRet&=rs.SetFieldValue("lw_admin",m_data.m_nAdmin);
		bRet&=rs.SetFieldValue("lw_room",m_data.m_nRoom);
		bRet&=rs.SetFieldValue("lw_newnum",m_data.m_nNewNum);

		bRet&=rs.Update();	

		rs.Close();
	} 
	bRet&=CBaseObject::SaveToDB(db,bChild,pProgressBar);

	return bRet;
}	

BOOL CLW::LoadInfo(CADORecordset &rs)
{
	BOOL bRet=TRUE;

	bRet&=rs.GetFieldValue("lw_id",m_data.m_strLWID);
	bRet&=rs.GetFieldValue("army_id",m_data.m_strArmyID);
	bRet&=rs.GetFieldValue("is_have",m_data.m_bHad);		
	bRet&=rs.GetFieldValue("is_onlyone",m_data.m_bOnlyOne);
	bRet&=rs.GetFieldValue("is_good",m_data.m_bGood);
	bRet&=rs.GetFieldValue("bad_type",m_data.m_nBadType);
	bRet&=rs.GetFieldValue("lw_computer",m_data.m_nComputer);
	bRet&=rs.GetFieldValue("lw_server",m_data.m_nServer);
	bRet&=rs.GetFieldValue("lw_admin",m_data.m_nAdmin);
	bRet&=rs.GetFieldValue("lw_room",m_data.m_nRoom);
	bRet&=rs.GetFieldValue("lw_newnum",m_data.m_nNewNum);

	m_strID=m_data.m_strLWID;

	return bRet;
}
BOOL CLW::DeleteFromDB(CADODatabase &db,BOOL bDelSelf/* =TRUE */,CProgressBar* pProgressBar/* =NULL */)
{
	BOOL bRet=FALSE;

	if((bRet=CBaseObject::DeleteFromDB(db,TRUE,pProgressBar)) == TRUE && bDelSelf)
	{
		CString strSql;
		strSql.Format("delete from netsurvey_lw where lw_id='%s'",m_data.m_strLWID);
		bRet=db.Execute(strSql);
	}

	return bRet;
}	

UINT CLW::PromptMenuID()
{
	return IDR_TREE_MENU;
}
int CLW::PromptMenuPos()
{
	return 3;
}
void CLW::UpdateMenu(CMenu *pMenu)
{
	//UINT delID=RetrieveMenuItemID(pMenu,_T("删除部队信息"));

	//ASSERT( delID!=-1  );

	//	int nLevel=theApp.GetUserLevel();	
	//	pMenu->EnableMenuItem(delID,(nLevel>0)? (MF_BYCOMMAND |MF_ENABLED):(MF_BYCOMMAND |MF_GRAYED));	
}


TiXmlNode*  CLW::StreamOut(TiXmlNode* pNode,BOOL bChild,CProgressBar* pProgressBar/* =NULL */)
{
	TiXmlElement * pTemp=new TiXmlElement(_T("netsurvey_lw"));

	pTemp->SetAttribute(_T("lw_id"),m_data.m_strLWID);
	pTemp->SetAttribute(_T("army_id"),m_data.m_strArmyID);
	pTemp->SetAttribute(_T("is_have"),(int)m_data.m_bHad);	
	pTemp->SetAttribute(_T("is_onlyone"),(int)m_data.m_bOnlyOne);
	pTemp->SetAttribute(_T("is_good"),(int)m_data.m_bGood);
	pTemp->SetAttribute(_T("bad_type"),m_data.m_nBadType);
	pTemp->SetAttribute(_T("lw_computer"),m_data.m_nComputer);
	pTemp->SetAttribute(_T("lw_server"),m_data.m_nServer);
	pTemp->SetAttribute(_T("lw_admin"),m_data.m_nAdmin);
	pTemp->SetAttribute(_T("lw_room"),m_data.m_nRoom);
	pTemp->SetAttribute(_T("lw_newnum"),m_data.m_nNewNum);

	pNode->LinkEndChild(pTemp);

	CBaseObject::StreamOut(pTemp,FALSE,pProgressBar);

	return pTemp;
}
