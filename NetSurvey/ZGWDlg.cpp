// ZGWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ZGWDlg.h"
#include "afxdialogex.h"
#include "Army.h"

// CZGWDlg dialog

CZGWDlg::CZGWDlg(CZGW* pZGW,CWnd* pParent /*=NULL*/)
	: CExpandingDialog(CZGWDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_SUBARMY,_T("下级数据>>>"),_T("下级数据<<<")),m_pZGW(pZGW)
{
	m_nAdmin = 0;
	m_nBW = 0;
	m_nComputer = 0;
	m_strIP = _T("");
	m_nRoom = 0;
	m_nServer = 0;
	m_strWebsite = _T("");
	m_bValueChange = FALSE;
}

CZGWDlg::~CZGWDlg()
{
}
void CZGWDlg::SetValue(CZGW::ZGWData& data)
{
	m_nAdmin = data.m_nAdmin;
	m_nBW = data.m_nBW;
	m_nComputer = data.m_nComputer;
	m_strIP = data.m_strIP;
	m_bConnected = data.m_bConnected;
	m_bFibre = data.m_bFibre;
	m_bOnlyOne = data.m_bOnlyOne;
	m_nRoom = data.m_nRoom;
	m_nServer = data.m_nServer;
	m_strWebsite = data.m_strWebsite;
}
BOOL CZGWDlg::GetValue(CZGW::ZGWData& data)
{
	BOOL bModify=FALSE;
	if( data.m_nAdmin != m_nAdmin )
	{
		data.m_nAdmin = m_nAdmin;
		bModify |= TRUE;
	}
	if( data.m_nBW != m_nBW )
	{
		data.m_nBW = m_nBW;
		bModify |= TRUE;
	}
	if( data.m_nComputer != m_nComputer )
	{
		data.m_nComputer = m_nComputer;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strIP,m_strIP))
	{
		data.m_strIP=m_strIP;
		bModify |= TRUE;
	}
	if( data.m_bConnected != m_bConnected )
	{
		data.m_bConnected = m_bConnected;
		bModify |= TRUE;
	}
	if( data.m_bFibre != m_bFibre )
	{
		data.m_bFibre = m_bFibre;
		bModify |= TRUE;
	}
	if( data.m_bOnlyOne != m_bOnlyOne )
	{
		data.m_bOnlyOne = m_bOnlyOne;
		bModify |= TRUE;
	}
	if( data.m_nRoom != m_nRoom )
	{
		data.m_nRoom = m_nRoom;
		bModify |= TRUE;
	}
	if( data.m_nServer != m_nServer )
	{
		data.m_nServer = m_nServer;
		bModify |= TRUE;
	}
	if(!CCommonFunction::IsSameString(data.m_strWebsite,m_strWebsite))
	{
		data.m_strWebsite=m_strWebsite;
		bModify |= TRUE;
	}
	return bModify;
}

void CZGWDlg::DoDataExchange(CDataExchange* pDX)
{
	CExpandingDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADMIN, m_nAdmin);
	DDV_MinMaxInt(pDX, m_nAdmin, 0, 100);
	DDX_Text(pDX, IDC_BW, m_nBW);
	DDV_MinMaxInt(pDX, m_nBW, 0, 1000);
	DDX_Text(pDX, IDC_COMPUTER, m_nComputer);
	DDV_MinMaxInt(pDX, m_nComputer, 0, 10000);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDX_Check(pDX, IDC_IS_CONNECTED, m_bConnected);
	DDX_Check(pDX, IDC_IS_FIBRE, m_bFibre);
	DDX_Radio(pDX, IDC_ONLYONE, m_bOnlyOne);
	DDX_Text(pDX, IDC_ROOM, m_nRoom);
	DDX_Text(pDX, IDC_SERVER, m_nServer);
	DDV_MinMaxInt(pDX, m_nServer, 0, 100);
	DDX_Text(pDX, IDC_WEBSITE, m_strWebsite);
	DDX_Control(pDX, IDC_SUB_ZGW, m_subZGW);
}


BEGIN_MESSAGE_MAP(CZGWDlg, CExpandingDialog)
	ON_UPDATE_COMMAND_UI(IDC_IS_FIBRE,OnUpdateFibre)
	ON_BN_CLICKED(IDC_IS_CONNECTED, &CZGWDlg::OnClickedIsConnected)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CZGWDlg message handlers


BOOL CZGWDlg::OnInitDialog()
{
	CExpandingDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Expand(FALSE);

	m_subZGW.EnableHeaderCtrl(TRUE,_T("统计内容"),_T("结果"));

	HDITEM item;
	item.cxy=200;     //第一列的宽度
	item.mask=HDI_WIDTH;

	m_subZGW.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	m_subZGW.EnableDescriptionArea();
	m_subZGW.SetVSDotNetLook();
	m_subZGW.MarkModifiedProperties();
	m_subZGW.SetAlphabeticMode(FALSE);
	m_subZGW.SetShowDragContext();

	CArmy* pArmy = (CArmy*) m_pZGW->BackTo(OBJECT_TYPE_ARMY);
	BOOL bEdit = pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("各级单位政工网联通情况")));
	
	CString str;
	switch ( pArmy->GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:	
		{
			CMFCPropertyGridProperty* pGroup11 = new CMFCPropertyGridProperty(_T("军以上"));
			apGroup1->AddSubItem(pGroup11);

			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].jianzhiTotal),_T("军以上建制单位总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].jianzhiCTotal), _T("军以上建制单位联通政工网总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].feijianzhiTotal), _T("军以上非建制单位总数")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].feijianzhiCTotal), _T("军以上非建制单位联通政工网总数")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[0].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[0].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[0].jianzhiTotal*100);
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("军以上建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[0].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[0].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[0].feijianzhiTotal*100));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"),str, _T("军以上非建制单位政工网联通率")));

			pGroup11->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[0].jianzhiTotal));
			pGroup11->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[0].jianzhiCTotal));
			pGroup11->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[0].feijianzhiTotal));
			pGroup11->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[0].feijianzhiCTotal));
			
			pGroup11->GetSubItem(0)->AllowEdit(bEdit);
			pGroup11->GetSubItem(1)->AllowEdit(bEdit);
			pGroup11->GetSubItem(2)->AllowEdit(bEdit);
			pGroup11->GetSubItem(3)->AllowEdit(bEdit);
			pGroup11->GetSubItem(4)->AllowEdit(FALSE);
			pGroup11->GetSubItem(5)->AllowEdit(FALSE);

			CMFCPropertyGridProperty* pGroup12 = new CMFCPropertyGridProperty(_T("师旅级"));
			apGroup1->AddSubItem(pGroup12);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].jianzhiTotal), _T("师旅级建制单位总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].jianzhiCTotal), _T("师旅级建制单位联通政工网总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].feijianzhiTotal), _T("师旅级非建制单位总数")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].feijianzhiCTotal), _T("师旅级非建制单位联通政工网总数")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[1].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[1].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[1].jianzhiTotal*100);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"),str, _T("师旅级建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[1].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[1].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[1].feijianzhiTotal*100));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("师旅级非建制单位政工网联通率")));

			pGroup12->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[1].jianzhiTotal));
			pGroup12->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[1].jianzhiCTotal));
			pGroup12->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[1].feijianzhiTotal));
			pGroup12->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[1].feijianzhiCTotal));

			pGroup12->GetSubItem(0)->AllowEdit(bEdit);
			pGroup12->GetSubItem(1)->AllowEdit(bEdit);
			pGroup12->GetSubItem(2)->AllowEdit(bEdit);
			pGroup12->GetSubItem(3)->AllowEdit(bEdit);
			pGroup12->GetSubItem(4)->AllowEdit(FALSE);
			pGroup12->GetSubItem(5)->AllowEdit(FALSE);
		}	
	case CNetSurveyApp::shilvArmy:
		{
			CMFCPropertyGridProperty* pGroup13 = new CMFCPropertyGridProperty(_T("各类团"));
			apGroup1->AddSubItem(pGroup13);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].jianzhiTotal), _T("各类团建制单位总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].jianzhiCTotal), _T("各类团建制单位联通政工网总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].feijianzhiTotal), _T("各类团非建制单位总数")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].feijianzhiCTotal), _T("各类团非建制单位联通政工网总数")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[2].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[2].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[2].jianzhiTotal*100);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("各类团建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[2].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[2].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[2].feijianzhiTotal*100));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("各类团非建制单位政工网联通率")));			
			
			pGroup13->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[2].jianzhiTotal));
			pGroup13->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[2].jianzhiCTotal));
			pGroup13->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[2].feijianzhiTotal));
			pGroup13->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[2].feijianzhiCTotal));

			pGroup13->GetSubItem(0)->AllowEdit(bEdit);
			pGroup13->GetSubItem(1)->AllowEdit(bEdit);
			pGroup13->GetSubItem(2)->AllowEdit(bEdit);
			pGroup13->GetSubItem(3)->AllowEdit(bEdit);
			pGroup13->GetSubItem(4)->AllowEdit(FALSE);
			pGroup13->GetSubItem(5)->AllowEdit(FALSE);
		}
	case CNetSurveyApp::tuanArmy:
		{
			CMFCPropertyGridProperty* pGroup14 = new CMFCPropertyGridProperty(_T("各类营"));
			apGroup1->AddSubItem(pGroup14);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].jianzhiTotal), _T("各类营建制单位总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].jianzhiCTotal), _T("各类营建制单位联通政工网总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].feijianzhiTotal), _T("各类营非建制单位总数")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].feijianzhiCTotal), _T("各类营非建制单位联通政工网总数")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[3].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[3].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[3].jianzhiTotal*100);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"), str, _T("各类营建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[3].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[3].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[3].feijianzhiTotal*100));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"),str, _T("各类营非建制单位政工网联通率")));
			
			pGroup14->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[3].jianzhiTotal));
			pGroup14->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[3].jianzhiCTotal));
			pGroup14->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[3].feijianzhiTotal));
			pGroup14->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[3].feijianzhiCTotal));

			pGroup14->GetSubItem(0)->AllowEdit(bEdit);
			pGroup14->GetSubItem(1)->AllowEdit(bEdit);
			pGroup14->GetSubItem(2)->AllowEdit(bEdit);
			pGroup14->GetSubItem(3)->AllowEdit(bEdit);
			pGroup14->GetSubItem(4)->AllowEdit(FALSE);
			pGroup14->GetSubItem(5)->AllowEdit(FALSE);
		}
	case CNetSurveyApp::yingArmy:
		{
			CMFCPropertyGridProperty* pGroup15 = new CMFCPropertyGridProperty(_T("各类连"));
			apGroup1->AddSubItem(pGroup15);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].jianzhiTotal), _T("各类连建制单位总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].jianzhiCTotal), _T("各类连建制单位联通政工网总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].feijianzhiTotal), _T("各类连非建制单位总数")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].feijianzhiCTotal), _T("各类连非建制单位联通政工网总数")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[4].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[4].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[4].jianzhiTotal*100);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("建制单位联通率"),str, _T("各类连建制单位政工网联通率")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[4].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[4].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[4].feijianzhiTotal*100));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("非建制单位联通率"), str, _T("各类连非建制单位政工网联通率")));
			
			pGroup15->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[4].jianzhiTotal));
			pGroup15->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[4].jianzhiCTotal));
			pGroup15->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[4].feijianzhiTotal));
			pGroup15->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subZGW.m_conn[4].feijianzhiCTotal));

			pGroup15->GetSubItem(0)->AllowEdit(bEdit);
			pGroup15->GetSubItem(1)->AllowEdit(bEdit);
			pGroup15->GetSubItem(2)->AllowEdit(bEdit);
			pGroup15->GetSubItem(3)->AllowEdit(bEdit);
			pGroup15->GetSubItem(4)->AllowEdit(FALSE);
			pGroup15->GetSubItem(5)->AllowEdit(FALSE);
		}
	}
	apGroup1->Expand();
	m_subZGW.AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("未联通政工网的单位光纤辅设情况统计")));

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("未联通政工网的单位总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nNotConnTotal),_T("未联通政工网的单位总数")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("完成光纤辅设的总数"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nFibreTotal), _T("完成光纤辅设的总数")));

	apGroup2->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_nNotConnTotal));
	apGroup2->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_nFibreTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);

	m_subZGW.AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("政工网资源情况统计")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("电脑终端总数量"), COleVariant(pArmy->m_subZGW.m_lComputerTotal), _T("联通政工网的电脑终端总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("服务器总数量"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nServerTotal),_T("政工网服务器总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("网管人员总数量"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nAdminTotal), _T("政工网网管人员总数量")));

	apGroup3->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_lComputerTotal));
	apGroup3->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_nServerTotal));
	apGroup3->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subZGW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	m_subZGW.AddProperty(apGroup3.release());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CZGWDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);

	return CExpandingDialog::PreTranslateMessage(pMsg);
}
void CZGWDlg::OnUpdateFibre(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( !m_bConnected );
}

void CZGWDlg::OnClickedIsConnected()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
BOOL CZGWDlg::OnDialogExpanding(BOOL /*bExpanded*/)
{
	UpdateData();
	OutputDebugString("CUserLoginDlg::OnDialogExpanding called.\n");
	return TRUE;
}

void CZGWDlg::OnDialogExpanded(BOOL bExpanded)
{
	OutputDebugString("CUserLoginDlg::OnDialogExpanded called.\n");

	UpdateData(FALSE);
}

LRESULT CZGWDlg::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

	COleVariant t = pProp->GetValue(); //改变之后的值

	int * i = (int*)pProp->GetData ();
	*i = CCommonFunction::OleVariant2Int(t);

	m_bValueChange = TRUE;

	return 0;
}

void CZGWDlg::OnOK()
{
	UpdateData();

	if( m_bValueChange )
	{
		CArmy* pArmy = (CArmy*) m_pZGW->BackTo(OBJECT_TYPE_ARMY);
		pArmy->SaveToDB(*theApp.GetDB(),FALSE);
	}

	CExpandingDialog::OnOK();
}