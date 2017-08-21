// ArmyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ArmyDlg.h"
#include "afxdialogex.h"
#include "DataDlg.h"

// CArmyDlg dialog


CArmyDlg::CArmyDlg(CArmy* pArmy,CWnd* pParent /*=NULL*/)
	: CExpandingDialog(CArmyDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_VIEW_DATA,_T("查看数据"),_T("查看数据")),m_pArmy(pArmy)
{
	m_strArmyName = _T("");
	m_nArmyLevel = 0;
	m_nArmyProp = 1;
}

CArmyDlg::~CArmyDlg()
{
}
void CArmyDlg::SetValue(CArmy::ArmyData& data)
{
	m_strArmyName = data.m_strArmyName;
	m_nArmyLevel = data.m_nArmyLevel;
	m_nArmyProp = data.m_nArmyProp;
}
BOOL CArmyDlg::GetValue(CArmy::ArmyData& data)
{
	BOOL bModify=FALSE;
	if(!CCommonFunction::IsSameString(data.m_strArmyName,m_strArmyName))
	{
		data.m_strArmyName=m_strArmyName;
		bModify |= TRUE;
	}
	
	if(data.m_nArmyLevel!=m_nArmyLevel)
	{
		data.m_nArmyLevel = m_nArmyLevel;
		bModify |= TRUE;
	}
	if(data.m_nArmyProp!=m_nArmyProp)
	{
		data.m_nArmyProp=m_nArmyProp;
		bModify |= TRUE;
	}	

	return bModify;
}
void CArmyDlg::DoDataExchange(CDataExchange* pDX)
{
	CExpandingDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ARMY_NAME, m_strArmyName);
	DDX_CBIndex(pDX, IDC_ARMY_LEVEL, m_nArmyLevel);
	DDX_Radio(pDX, IDC_IS_JIANZHI, m_nArmyProp);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}


BEGIN_MESSAGE_MAP(CArmyDlg, CExpandingDialog)
	ON_BN_CLICKED(IDC_VIEW_DATA, &CArmyDlg::OnBnClickedViewData)
END_MESSAGE_MAP()


// CArmyDlg message handlers


BOOL CArmyDlg::OnInitDialog()
{
	CExpandingDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Expand(FALSE);
	
	m_tab.AddPage(_T("政工网"), RUNTIME_CLASS(CDataDlg),IDD_DATA);
	m_tab.AddPage(_T("蓝网"), RUNTIME_CLASS(CDataDlg),IDD_DATA);
	m_tab.ActivePage(0);

	InitZGWData();
	InitLWData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CArmyDlg::OnOK()
{
	UpdateData();

	CArmy* pParent = (CArmy*) m_pArmy->BackTo(OBJECT_TYPE_ARMY);

	if( pParent && (int)pParent->GetArmyLevel() > 0 ) // 师旅以下
	{
		if( pParent->GetArmyLevel() == CNetSurveyApp::shilvArmy )
		{
			if( m_nArmyLevel == 0 )
			{
				MessageBox(_T("下级部队级别不能高于上级部队，请重新选择！"),NULL,MB_OK|MB_ICONWARNING);
			    return;
			}
		}
		else if( m_nArmyLevel < pParent->GetArmyLevel())
		{
			MessageBox(_T("下级部队级别不能高于上级部队，请重新选择！"),NULL,MB_OK|MB_ICONWARNING);
			return;
		}
		
	}
	CExpandingDialog::OnOK();
}

void CArmyDlg::OnBnClickedViewData()
{
	// TODO: Add your control notification handler code here
}
BOOL CArmyDlg::OnDialogExpanding(BOOL /*bExpanded*/)
{
	UpdateData();
	OutputDebugString("CUserLoginDlg::OnDialogExpanding called.\n");
	return TRUE;
}

void CArmyDlg::OnDialogExpanded(BOOL bExpanded)
{
	OutputDebugString("CUserLoginDlg::OnDialogExpanded called.\n");

	UpdateData(FALSE);
}

void CArmyDlg::InitZGWData()
{
	CMFCPropertyGridCtrl* pPropGrid = &(static_cast<CDataDlg*>(m_tab.GetPage(0))->m_propGrid);
	pPropGrid->EnableHeaderCtrl(TRUE,_T("统计内容"),_T("结果"));

	HDITEM item;
	item.cxy=200;     //第一列的宽度
	item.mask=HDI_WIDTH;

	pPropGrid->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pPropGrid->EnableDescriptionArea();
	pPropGrid->SetVSDotNetLook();
	pPropGrid->MarkModifiedProperties();
	pPropGrid->SetAlphabeticMode(FALSE);
	pPropGrid->SetShowDragContext();

	BOOL bEdit = FALSE;//pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("各级单位政工网联通情况")));

	CString str;
	switch ( m_pArmy->GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:	
		{
			CMFCPropertyGridProperty* pGroup11 = new CMFCPropertyGridProperty(_T("军以上"));
			apGroup1->AddSubItem(pGroup11);

			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].jianzhiTotal),_T("军以上建制单位总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].jianzhiCTotal), _T("军以上建制单位联通政工网总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal), _T("军以上非建制单位总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].feijianzhiCTotal), _T("军以上非建制单位联通政工网总数")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[0].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[0].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[0].jianzhiTotal*100);
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("军以上建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[0].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal*100));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"),str, _T("军以上非建制单位政工网联通率")));

			/*pGroup11->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[0].jianzhiTotal));
			pGroup11->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[0].jianzhiCTotal));
			pGroup11->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal));
			pGroup11->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[0].feijianzhiCTotal));*/

			pGroup11->GetSubItem(0)->AllowEdit(bEdit);
			pGroup11->GetSubItem(1)->AllowEdit(bEdit);
			pGroup11->GetSubItem(2)->AllowEdit(bEdit);
			pGroup11->GetSubItem(3)->AllowEdit(bEdit);
			pGroup11->GetSubItem(4)->AllowEdit(FALSE);
			pGroup11->GetSubItem(5)->AllowEdit(FALSE);
			
		}	
	case CNetSurveyApp::shilvArmy:
		{
			CMFCPropertyGridProperty* pGroup12 = new CMFCPropertyGridProperty(_T("师旅级"));
			apGroup1->AddSubItem(pGroup12);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].jianzhiTotal), _T("师旅级建制单位总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].jianzhiCTotal), _T("师旅级建制单位联通政工网总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal), _T("师旅级非建制单位总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].feijianzhiCTotal), _T("师旅级非建制单位联通政工网总数")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[1].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[1].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[1].jianzhiTotal*100);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"),str, _T("师旅级建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[1].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal*100));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("师旅级非建制单位政工网联通率")));

			//pGroup12->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[1].jianzhiTotal));
			//pGroup12->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[1].jianzhiCTotal));
			//pGroup12->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal));
			//pGroup12->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[1].feijianzhiCTotal));

			pGroup12->GetSubItem(0)->AllowEdit(bEdit);
			pGroup12->GetSubItem(1)->AllowEdit(bEdit);
			pGroup12->GetSubItem(2)->AllowEdit(bEdit);
			pGroup12->GetSubItem(3)->AllowEdit(bEdit);
			pGroup12->GetSubItem(4)->AllowEdit(FALSE);
			pGroup12->GetSubItem(5)->AllowEdit(FALSE);			
		}
	case CNetSurveyApp::tuanArmy:
		{
			CMFCPropertyGridProperty* pGroup13 = new CMFCPropertyGridProperty(_T("各类团"));
			apGroup1->AddSubItem(pGroup13);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].jianzhiTotal), _T("各类团建制单位总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].jianzhiCTotal), _T("各类团建制单位联通政工网总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal), _T("各类团非建制单位总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].feijianzhiCTotal), _T("各类团非建制单位联通政工网总数")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[2].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[2].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[2].jianzhiTotal*100);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("各类团建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[2].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal*100));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("各类团非建制单位政工网联通率")));			

			//pGroup13->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[2].jianzhiTotal));
			//pGroup13->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[2].jianzhiCTotal));
			//pGroup13->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal));
			//pGroup13->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[2].feijianzhiCTotal));

			pGroup13->GetSubItem(0)->AllowEdit(bEdit);
			pGroup13->GetSubItem(1)->AllowEdit(bEdit);
			pGroup13->GetSubItem(2)->AllowEdit(bEdit);
			pGroup13->GetSubItem(3)->AllowEdit(bEdit);
			pGroup13->GetSubItem(4)->AllowEdit(FALSE);
			pGroup13->GetSubItem(5)->AllowEdit(FALSE);
			
		}
	case CNetSurveyApp::yingArmy:
		{
			CMFCPropertyGridProperty* pGroup14 = new CMFCPropertyGridProperty(_T("各类营"));
			apGroup1->AddSubItem(pGroup14);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].jianzhiTotal), _T("各类营建制单位总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].jianzhiCTotal), _T("各类营建制单位联通政工网总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal), _T("各类营非建制单位总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].feijianzhiCTotal), _T("各类营非建制单位联通政工网总数")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[3].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[3].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[3].jianzhiTotal*100);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("各类营建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[3].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal*100));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"),str, _T("各类营非建制单位政工网联通率")));

			//pGroup14->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[3].jianzhiTotal));
			//pGroup14->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[3].jianzhiCTotal));
			//pGroup14->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal));
			//pGroup14->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[3].feijianzhiCTotal));

			pGroup14->GetSubItem(0)->AllowEdit(bEdit);
			pGroup14->GetSubItem(1)->AllowEdit(bEdit);
			pGroup14->GetSubItem(2)->AllowEdit(bEdit);
			pGroup14->GetSubItem(3)->AllowEdit(bEdit);
			pGroup14->GetSubItem(4)->AllowEdit(FALSE);
			pGroup14->GetSubItem(5)->AllowEdit(FALSE);
		}
	case CNetSurveyApp::lianArmy:
		{
			CMFCPropertyGridProperty* pGroup15 = new CMFCPropertyGridProperty(_T("各类连"));
			apGroup1->AddSubItem(pGroup15);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].jianzhiTotal), _T("各类连建制单位总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].jianzhiCTotal), _T("各类连建制单位联通政工网总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal), _T("各类连非建制单位总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].feijianzhiCTotal), _T("各类连非建制单位联通政工网总数")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[4].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[4].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[4].jianzhiTotal*100);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"),str, _T("各类连建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[4].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal*100));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("各类连非建制单位政工网联通率")));

			//pGroup15->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[4].jianzhiTotal));
			//pGroup15->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[4].jianzhiCTotal));
			//pGroup15->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal));
			//pGroup15->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curZGW.m_conn[4].feijianzhiCTotal));

			pGroup15->GetSubItem(0)->AllowEdit(bEdit);
			pGroup15->GetSubItem(1)->AllowEdit(bEdit);
			pGroup15->GetSubItem(2)->AllowEdit(bEdit);
			pGroup15->GetSubItem(3)->AllowEdit(bEdit);
			pGroup15->GetSubItem(4)->AllowEdit(FALSE);
			pGroup15->GetSubItem(5)->AllowEdit(FALSE);
		}
	}
	apGroup1->Expand();
	pPropGrid->AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("未联通政工网的单位光纤辅设情况统计")));

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("未联通政工网的单位总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nNotConnTotal),_T("未联通政工网的单位总数")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("完成光纤辅设的总数"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nFibreTotal), _T("完成光纤辅设的总数")));

	//apGroup2->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nNotConnTotal));
	//apGroup2->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nFibreTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("政工网资源情况统计")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("电脑终端总数量"), COleVariant(m_pArmy->m_curZGW.m_lComputerTotal), _T("联通政工网的电脑终端总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("服务器总数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nServerTotal),_T("政工网服务器总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("网管人员总数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nAdminTotal), _T("政工网网管人员总数量")));

	//apGroup3->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nComputerTotal));
	//apGroup3->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nServerTotal));
	//apGroup3->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup3.release());
}
void CArmyDlg::InitLWData()
{
	CMFCPropertyGridCtrl* pPropGrid = &(static_cast<CDataDlg*>(m_tab.GetPage(1))->m_propGrid);
	pPropGrid->EnableHeaderCtrl(TRUE,_T("统计内容"),_T("结果"));

	HDITEM item;
	item.cxy=200;     //第一列的宽度
	item.mask=HDI_WIDTH;

	pPropGrid->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pPropGrid->EnableDescriptionArea();
	pPropGrid->SetVSDotNetLook();
	pPropGrid->MarkModifiedProperties();
	pPropGrid->SetAlphabeticMode(FALSE);
	pPropGrid->SetShowDragContext();
	
	BOOL bEdit = FALSE; //pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("蓝网基本情况统计")));	

	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网已安装的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nHadTotal), _T("蓝网已安装的数量")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网功能正常的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nGoodTotal), _T("蓝网功能正常的数量")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("需要蓝网的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nNewTotal), _T("蓝网新增的数量")));

	//apGroup1->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curLW.m_nHadTotal));
	//apGroup1->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curLW.m_nGoodTotal));
	//apGroup1->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curLW.m_nNewTotal));

	apGroup1->GetSubItem(0)->AllowEdit(bEdit);
	apGroup1->GetSubItem(1)->AllowEdit(bEdit);
	apGroup1->GetSubItem(2)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("蓝网故障情况统计")));	

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网功能故障的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTotal), _T("蓝网功能故障的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("全部损坏的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadAllTotal), _T("全部损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("天线损坏的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTianXianTotal), _T("天线损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("支架损坏的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadZhiJiaTotal), _T("支架损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("高频头损坏的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadGPTTotal), _T("高频头损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("通视卡损坏的数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTSKTotal), _T("通视卡损坏的数量")));

	//apGroup2->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadTotal));
	//apGroup2->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadAllTotal));
	//apGroup2->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadTianXianTotal));
	//apGroup2->GetSubItem(3)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadZhiJiaTotal));
	//apGroup2->GetSubItem(4)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadGPTTotal));
	//apGroup2->GetSubItem(5)->SetData((DWORD)&(m_pArmy->m_curLW.m_nBadTSKTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);
	apGroup2->GetSubItem(2)->AllowEdit(bEdit);
	apGroup2->GetSubItem(3)->AllowEdit(bEdit);
	apGroup2->GetSubItem(4)->AllowEdit(bEdit);
	apGroup2->GetSubItem(5)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("蓝网资源情况统计")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("电脑终端总数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nComputerTotal), _T("联通蓝网的电脑终端总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("服务器总数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nServerTotal), _T("蓝网服务器总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("网管人员总数量"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nAdminTotal), _T("蓝网网管人员总数量")));

	//apGroup3->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curLW.m_nComputerTotal));
	//apGroup3->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curLW.m_nServerTotal));
	//apGroup3->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curLW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup3.release());
}