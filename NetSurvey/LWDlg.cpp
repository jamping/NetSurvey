// LWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "LWDlg.h"
#include "afxdialogex.h"
#include "Army.h"


// CLWDlg dialog


CLWDlg::CLWDlg(CLW* pLW,CWnd* pParent /*=NULL*/)
: CExpandingDialog(CLWDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_SUBARMY,_T("�¼�����>>>"),_T("�¼�����<<<")),m_pLW(pLW)
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

	m_subLW.EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));

	HDITEM item;
	item.cxy=200;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	m_subLW.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	m_subLW.EnableDescriptionArea();
	m_subLW.SetVSDotNetLook();
	m_subLW.MarkModifiedProperties();
	m_subLW.SetAlphabeticMode(FALSE);
	m_subLW.SetShowDragContext();

	CArmy* pArmy = (CArmy*)m_pLW->BackTo(OBJECT_TYPE_ARMY);
	BOOL bEdit = pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	

	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("�����Ѱ�װ������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nHadTotal), _T("�����Ѱ�װ������")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("������������������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nGoodTotal), _T("������������������")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��Ҫ����������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nNewTotal), _T("��������������")));
	
	apGroup1->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subLW.m_nHadTotal));
	apGroup1->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subLW.m_nGoodTotal));
	apGroup1->GetSubItem(2)->SetData((DWORD)&(pArmy->m_subLW.m_nNewTotal));

	apGroup1->GetSubItem(0)->AllowEdit(bEdit);
	apGroup1->GetSubItem(1)->AllowEdit(bEdit);
	apGroup1->GetSubItem(2)->AllowEdit(bEdit);

	m_subLW.AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	
	
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�������ܹ��ϵ�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTotal), _T("�������ܹ��ϵ�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ȫ���𻵵�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadAllTotal), _T("ȫ���𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�����𻵵�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTianXianTotal), _T("�����𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("֧���𻵵�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadZhiJiaTotal), _T("֧���𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��Ƶͷ�𻵵�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadGPTTotal), _T("��Ƶͷ�𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ͨ�ӿ��𻵵�����"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nBadTSKTotal), _T("ͨ�ӿ��𻵵�����")));

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

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("������Դ���ͳ��")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nComputerTotal), _T("��ͨ�����ĵ����ն�������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nServerTotal), _T("����������������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(pArmy->m_subLW.m_nAdminTotal), _T("����������Ա������")));
	
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

	COleVariant t = pProp->GetValue(); //�ı�֮���ֵ

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