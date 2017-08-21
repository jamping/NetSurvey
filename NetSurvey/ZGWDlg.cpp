// ZGWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ZGWDlg.h"
#include "afxdialogex.h"
#include "Army.h"

// CZGWDlg dialog

CZGWDlg::CZGWDlg(CZGW* pZGW,CWnd* pParent /*=NULL*/)
	: CExpandingDialog(CZGWDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_SUBARMY,_T("�¼�����>>>"),_T("�¼�����<<<")),m_pZGW(pZGW)
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

	m_subZGW.EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));

	HDITEM item;
	item.cxy=200;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	m_subZGW.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	m_subZGW.EnableDescriptionArea();
	m_subZGW.SetVSDotNetLook();
	m_subZGW.MarkModifiedProperties();
	m_subZGW.SetAlphabeticMode(FALSE);
	m_subZGW.SetShowDragContext();

	CArmy* pArmy = (CArmy*) m_pZGW->BackTo(OBJECT_TYPE_ARMY);
	BOOL bEdit = pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("������λ��������ͨ���")));
	
	CString str;
	switch ( pArmy->GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:	
		{
			CMFCPropertyGridProperty* pGroup11 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup11);

			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].jianzhiTotal),_T("�����Ͻ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].jianzhiCTotal), _T("�����Ͻ��Ƶ�λ��ͨ����������")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].feijianzhiTotal), _T("�����Ϸǽ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[0].feijianzhiCTotal), _T("�����Ϸǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[0].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[0].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[0].jianzhiTotal*100);
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ͻ��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[0].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[0].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[0].feijianzhiTotal*100));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("�����Ϸǽ��Ƶ�λ��������ͨ��")));

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

			CMFCPropertyGridProperty* pGroup12 = new CMFCPropertyGridProperty(_T("ʦ�ü�"));
			apGroup1->AddSubItem(pGroup12);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].jianzhiTotal), _T("ʦ�ü����Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].jianzhiCTotal), _T("ʦ�ü����Ƶ�λ��ͨ����������")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].feijianzhiTotal), _T("ʦ�ü��ǽ��Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[1].feijianzhiCTotal), _T("ʦ�ü��ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[1].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[1].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[1].jianzhiTotal*100);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("ʦ�ü����Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[1].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[1].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[1].feijianzhiTotal*100));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("ʦ�ü��ǽ��Ƶ�λ��������ͨ��")));

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
			CMFCPropertyGridProperty* pGroup13 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup13);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].jianzhiTotal), _T("�����Ž��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].jianzhiCTotal), _T("�����Ž��Ƶ�λ��ͨ����������")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].feijianzhiTotal), _T("�����ŷǽ��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[2].feijianzhiCTotal), _T("�����ŷǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[2].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[2].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[2].jianzhiTotal*100);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ž��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[2].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[2].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[2].feijianzhiTotal*100));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�����ŷǽ��Ƶ�λ��������ͨ��")));			
			
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
			CMFCPropertyGridProperty* pGroup14 = new CMFCPropertyGridProperty(_T("����Ӫ"));
			apGroup1->AddSubItem(pGroup14);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].jianzhiTotal), _T("����Ӫ���Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].jianzhiCTotal), _T("����Ӫ���Ƶ�λ��ͨ����������")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].feijianzhiTotal), _T("����Ӫ�ǽ��Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[3].feijianzhiCTotal), _T("����Ӫ�ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[3].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[3].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[3].jianzhiTotal*100);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("����Ӫ���Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[3].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[3].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[3].feijianzhiTotal*100));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("����Ӫ�ǽ��Ƶ�λ��������ͨ��")));
			
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
			CMFCPropertyGridProperty* pGroup15 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup15);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].jianzhiTotal), _T("���������Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].jianzhiCTotal), _T("���������Ƶ�λ��ͨ����������")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].feijianzhiTotal), _T("�������ǽ��Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_conn[4].feijianzhiCTotal), _T("�������ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(pArmy->m_subZGW.m_conn[4].jianzhiTotal <= 0)?0:pArmy->m_subZGW.m_conn[4].jianzhiCTotal/(float)pArmy->m_subZGW.m_conn[4].jianzhiTotal*100);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("���������Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(pArmy->m_subZGW.m_conn[4].feijianzhiTotal <= 0)?0:(pArmy->m_subZGW.m_conn[4].feijianzhiCTotal/(float)pArmy->m_subZGW.m_conn[4].feijianzhiTotal*100));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�������ǽ��Ƶ�λ��������ͨ��")));
			
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

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ���˸������ͳ��")));

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ����"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nNotConnTotal),_T("δ��ͨ�������ĵ�λ����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��ɹ��˸��������"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nFibreTotal), _T("��ɹ��˸��������")));

	apGroup2->GetSubItem(0)->SetData((DWORD)&(pArmy->m_subZGW.m_nNotConnTotal));
	apGroup2->GetSubItem(1)->SetData((DWORD)&(pArmy->m_subZGW.m_nFibreTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);

	m_subZGW.AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("��������Դ���ͳ��")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), COleVariant(pArmy->m_subZGW.m_lComputerTotal), _T("��ͨ�������ĵ����ն�������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nServerTotal),_T("������������������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(pArmy->m_subZGW.m_nAdminTotal), _T("������������Ա������")));

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

	COleVariant t = pProp->GetValue(); //�ı�֮���ֵ

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