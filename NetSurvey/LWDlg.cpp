// LWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "LWDlg.h"
#include "afxdialogex.h"
#include "Army.h"


// CLWDlg dialog


CLWDlg::CLWDlg(CLW* pLW,CWnd* pParent /*=NULL*/)
: CExpandingDialog(CLWDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_SUBARMY,_T("下级数据>>>"),_T("下级数据<<<")),m_pLW(pLW)
{
	m_nAdmin = 0;
	m_nBadType = 0;
	m_nComputer = 0;
	m_nNewNum = 0;
	m_nRoom = 0;
	m_nServer = 0;
	m_bValueChange = FALSE;
}

CLWDlg::~CLWDlg()
{

}

void CLWDlg::SetValue(CLW::LWData& data)
{
	m_nAdmin = data.m_nAdmin;
	m_nBadType = data.m_nBadType;
	m_nComputer = data.m_nComputer;
	m_bGood = data.m_bGood;
	m_bHad = data.m_bHad;
	m_bOnlyOne = data.m_bOnlyOne;
	m_nRoom = data.m_nRoom;
	m_nServer = data.m_nServer;
	m_nNewNum = data.m_nNewNum;
}
BOOL CLWDlg::GetValue(CLW::LWData& data)
{
	BOOL bModify=FALSE;
	if( data.m_nAdmin != m_nAdmin )
	{
		data.m_nAdmin = m_nAdmin;
		bModify |= TRUE;
	}
	if( data.m_nBadType != m_nBadType )
	{
		data.m_nBadType = m_nBadType;
		bModify |= TRUE;
	}
	if( data.m_nComputer != m_nComputer )
	{
		data.m_nComputer = m_nComputer;
		bModify |= TRUE;
	}	
	if( data.m_bGood != m_bGood )
	{
		data.m_bGood = m_bGood;
		bModify |= TRUE;
	}
	if( data.m_bHad != m_bHad )
	{
		data.m_bHad = m_bHad;
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
	if( data.m_nNewNum != m_nNewNum )
	{
		data.m_nNewNum = m_nNewNum;
		bModify |= TRUE;
	}
	
	return bModify;
}
void CLWDlg::DoDataExchange(CDataExchange* pDX)
{
	CExpandingDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADMIN, m_nAdmin);
	DDV_MinMaxInt(pDX, m_nAdmin, 0, 20);
	DDX_CBIndex(pDX, IDC_BADTYPE, m_nBadType);
	DDX_Text(pDX, IDC_COMPUTER, m_nComputer);
	DDV_MinMaxInt(pDX, m_nComputer, 0, 10000);
	DDX_Check(pDX, IDC_IS_GOOD, m_bGood);
	DDX_Check(pDX, IDC_IS_HAD, m_bHad);
	DDX_Text(pDX, IDC_NEWNUM, m_nNewNum);
	DDX_Radio(pDX, IDC_ONLYONE, m_bOnlyOne);
	DDX_Text(pDX, IDC_ROOM, m_nRoom);
	DDX_Text(pDX, IDC_SERVER, m_nServer);
	DDX_Control(pDX, IDC_SUB_LW, m_subLW);
}


BEGIN_MESSAGE_MAP(CLWDlg, CExpandingDialog)
	ON_UPDATE_COMMAND_UI(IDC_IS_GOOD,OnUpdateGood)
	ON_UPDATE_COMMAND_UI(IDC_BADTYPE,OnUpdateBadType)
	ON_UPDATE_COMMAND_UI(IDC_ONLYONE,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_LOCAL_NETWORK,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_NUM,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_ADMIN,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_COMPUTER,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_ROOM,OnUpdateOther)
	ON_UPDATE_COMMAND_UI(IDC_SERVER,OnUpdateOther)
	ON_BN_CLICKED(IDC_IS_HAD, &CLWDlg::OnClickedIsHad)
	ON_BN_CLICKED(IDC_IS_GOOD, &CLWDlg::OnClickedIsGood)
	ON_BN_CLICKED(IDC_SUBARMY, &CLWDlg::OnBnClickedSubarmy)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


// CLWDlg message handlers


BOOL CLWDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateDialogControls(this,FALSE);
	return CExpandingDialog::PreTranslateMessage(pMsg);
}


BOOL CLWDlg::OnInitDialog()
{
	CExpandingDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Expand(FALSE);

	m_subLW.EnableHeaderCtrl(TRUE,_T("统计内容"),_T("结果"));

	HDITEM item;
	item.cxy=200;     //第一列的宽度
	item.mask=HDI_WIDTH;

	m_subLW.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	m_subLW.EnableDescriptionArea();
	m_subLW.SetVSDotNetLook();
	m_subLW.MarkModifiedProperties();
	m_subLW.SetAlphabeticMode(FALSE);
	m_subLW.SetShowDragContext();

	CArmy* pArmy = (CArmy*)m_pLW->BackTo(OBJECT_TYPE_ARMY);
	BOOL bEdit = pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("蓝网基本情况统计")));	

	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网已安装的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nHadTotal), _T("蓝网已安装的数量")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网功能正常的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nGoodTotal), _T("蓝网功能正常的数量")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("需要蓝网的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nNewTotal), _T("蓝网新增的数量")));
	
	apGroup1->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subLW.m_nHadTotal));
	apGroup1->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subLW.m_nGoodTotal));
	apGroup1->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subLW.m_nNewTotal));

	apGroup1->GetSubItem(0)->AllowEdit(bEdit);
	apGroup1->GetSubItem(1)->AllowEdit(bEdit);
	apGroup1->GetSubItem(2)->AllowEdit(bEdit);

	m_subLW.AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("蓝网故障情况统计")));	
	
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("蓝网功能故障的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTotal), _T("蓝网功能故障的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("全部损坏的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadAllTotal), _T("全部损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("天线损坏的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTianXianTotal), _T("天线损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("支架损坏的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadZhiJiaTotal), _T("支架损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("高频头损坏的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadGPTTotal), _T("高频头损坏的数量")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("通视卡损坏的数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTSKTotal), _T("通视卡损坏的数量")));

	apGroup2->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subLW.m_nBadTotal));
	apGroup2->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subLW.m_nBadAllTotal));
	apGroup2->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subLW.m_nBadTianXianTotal));
	apGroup2->GetSubItem(3)->SetData((DWORD)&(pArmy->m_subLW.m_nBadZhiJiaTotal));
	apGroup2->GetSubItem(4)->SetData((DWORD)&(pArmy->m_subLW.m_nBadGPTTotal));
	apGroup2->GetSubItem(5)->SetData((DWORD)&(pArmy->m_subLW.m_nBadTSKTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);
	apGroup2->GetSubItem(2)->AllowEdit(bEdit);
	apGroup2->GetSubItem(3)->AllowEdit(bEdit);
	apGroup2->GetSubItem(4)->AllowEdit(bEdit);
	apGroup2->GetSubItem(5)->AllowEdit(bEdit);

	m_subLW.AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("蓝网资源情况统计")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("电脑终端总数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nComputerTotal), _T("联通蓝网的电脑终端总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("服务器总数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nServerTotal), _T("蓝网服务器总数量")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("网管人员总数量"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nAdminTotal), _T("蓝网网管人员总数量")));
	
	apGroup3->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subLW.m_nComputerTotal));
	apGroup3->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subLW.m_nServerTotal));
	apGroup3->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subLW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	m_subLW.AddProperty(apGroup3.release());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CLWDlg::OnClickedIsHad()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void CLWDlg::OnClickedIsGood()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
void CLWDlg::OnUpdateGood(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_bHad );
}

void CLWDlg::OnUpdateBadType(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_bHad && !m_bGood );
}
void CLWDlg::OnUpdateOther(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_bHad );
}

BOOL CLWDlg::OnDialogExpanding(BOOL /*bExpanded*/)
{
	UpdateData();
	OutputDebugString("CUserLoginDlg::OnDialogExpanding called.\n");
	return TRUE;
}

void CLWDlg::OnDialogExpanded(BOOL bExpanded)
{
	OutputDebugString("CUserLoginDlg::OnDialogExpanded called.\n");

	UpdateData(FALSE);
}

void CLWDlg::OnBnClickedSubarmy()
{
	// TODO: Add your control notification handler code here

}
LRESULT CLWDlg::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lParam;

	COleVariant t = pProp->GetValue(); //改变之后的值

	int * i = (int*)pProp->GetData ();
	*i = CCommonFunction::OleVariant2Int(t);

	BOOL m_bValueChange = TRUE;

	return 0;
}

void CLWDlg::OnOK()
{
	UpdateData();

	if( m_bValueChange )
	{
		CArmy* pArmy = (CArmy*) m_pLW->BackTo(OBJECT_TYPE_ARMY);
		pArmy->SaveToDB(*theApp.GetDB(),FALSE);
	}

	CExpandingDialog::OnOK();
}