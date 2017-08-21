// ArmyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetSurvey.h"
#include "ArmyDlg.h"
#include "afxdialogex.h"
#include "DataDlg.h"

// CArmyDlg dialog


CArmyDlg::CArmyDlg(CArmy* pArmy,CWnd* pParent /*=NULL*/)
	: CExpandingDialog(CArmyDlg::IDD, pParent,IDC_DEFAULTBOX,IDC_VIEW_DATA,_T("�鿴����"),_T("�鿴����")),m_pArmy(pArmy)
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
	
	m_tab.AddPage(_T("������"), RUNTIME_CLASS(CDataDlg),IDD_DATA);
	m_tab.AddPage(_T("����"), RUNTIME_CLASS(CDataDlg),IDD_DATA);
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

	if( pParent && (int)pParent->GetArmyLevel() > 0 ) // ʦ������
	{
		if( pParent->GetArmyLevel() == CNetSurveyApp::shilvArmy )
		{
			if( m_nArmyLevel == 0 )
			{
				MessageBox(_T("�¼����Ӽ����ܸ����ϼ����ӣ�������ѡ��"),NULL,MB_OK|MB_ICONWARNING);
			    return;
			}
		}
		else if( m_nArmyLevel < pParent->GetArmyLevel())
		{
			MessageBox(_T("�¼����Ӽ����ܸ����ϼ����ӣ�������ѡ��"),NULL,MB_OK|MB_ICONWARNING);
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
	pPropGrid->EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));

	HDITEM item;
	item.cxy=200;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	pPropGrid->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pPropGrid->EnableDescriptionArea();
	pPropGrid->SetVSDotNetLook();
	pPropGrid->MarkModifiedProperties();
	pPropGrid->SetAlphabeticMode(FALSE);
	pPropGrid->SetShowDragContext();

	BOOL bEdit = FALSE;//pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("������λ��������ͨ���")));

	CString str;
	switch ( m_pArmy->GetArmyLevel() )
	{
	case CNetSurveyApp::junArmy:	
		{
			CMFCPropertyGridProperty* pGroup11 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup11);

			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].jianzhiTotal),_T("�����Ͻ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].jianzhiCTotal), _T("�����Ͻ��Ƶ�λ��ͨ����������")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal), _T("�����Ϸǽ��Ƶ�λ����")));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[0].feijianzhiCTotal), _T("�����Ϸǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[0].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[0].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[0].jianzhiTotal*100);
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ͻ��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[0].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[0].feijianzhiTotal*100));
			pGroup11->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("�����Ϸǽ��Ƶ�λ��������ͨ��")));

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
			CMFCPropertyGridProperty* pGroup12 = new CMFCPropertyGridProperty(_T("ʦ�ü�"));
			apGroup1->AddSubItem(pGroup12);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].jianzhiTotal), _T("ʦ�ü����Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].jianzhiCTotal), _T("ʦ�ü����Ƶ�λ��ͨ����������")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal), _T("ʦ�ü��ǽ��Ƶ�λ����")));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[1].feijianzhiCTotal), _T("ʦ�ü��ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[1].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[1].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[1].jianzhiTotal*100);
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("ʦ�ü����Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[1].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[1].feijianzhiTotal*100));
			pGroup12->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("ʦ�ü��ǽ��Ƶ�λ��������ͨ��")));

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
			CMFCPropertyGridProperty* pGroup13 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup13);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].jianzhiTotal), _T("�����Ž��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].jianzhiCTotal), _T("�����Ž��Ƶ�λ��ͨ����������")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal), _T("�����ŷǽ��Ƶ�λ����")));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[2].feijianzhiCTotal), _T("�����ŷǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[2].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[2].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[2].jianzhiTotal*100);
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("�����Ž��Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[2].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[2].feijianzhiTotal*100));
			pGroup13->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�����ŷǽ��Ƶ�λ��������ͨ��")));			

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
			CMFCPropertyGridProperty* pGroup14 = new CMFCPropertyGridProperty(_T("����Ӫ"));
			apGroup1->AddSubItem(pGroup14);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].jianzhiTotal), _T("����Ӫ���Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].jianzhiCTotal), _T("����Ӫ���Ƶ�λ��ͨ����������")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal), _T("����Ӫ�ǽ��Ƶ�λ����")));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[3].feijianzhiCTotal), _T("����Ӫ�ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[3].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[3].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[3].jianzhiTotal*100);
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"), str, _T("����Ӫ���Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[3].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[3].feijianzhiTotal*100));
			pGroup14->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"),str, _T("����Ӫ�ǽ��Ƶ�λ��������ͨ��")));

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
			CMFCPropertyGridProperty* pGroup15 = new CMFCPropertyGridProperty(_T("������"));
			apGroup1->AddSubItem(pGroup15);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].jianzhiTotal), _T("���������Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].jianzhiCTotal), _T("���������Ƶ�λ��ͨ����������")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal), _T("�������ǽ��Ƶ�λ����")));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_conn[4].feijianzhiCTotal), _T("�������ǽ��Ƶ�λ��ͨ����������")));
			str.Format("%.1f%%",(m_pArmy->m_curZGW.m_conn[4].jianzhiTotal <= 0)?0:m_pArmy->m_curZGW.m_conn[4].jianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[4].jianzhiTotal*100);
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("���Ƶ�λ��ͨ��"),str, _T("���������Ƶ�λ��������ͨ��")));
			str.Format(_T("%.1f%%"),(m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal <= 0)?0:(m_pArmy->m_curZGW.m_conn[4].feijianzhiCTotal/(float)m_pArmy->m_curZGW.m_conn[4].feijianzhiTotal*100));
			pGroup15->AddSubItem(new CMFCPropertyGridProperty(_T("�ǽ��Ƶ�λ��ͨ��"), str, _T("�������ǽ��Ƶ�λ��������ͨ��")));

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

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ���˸������ͳ��")));

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("δ��ͨ�������ĵ�λ����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nNotConnTotal),_T("δ��ͨ�������ĵ�λ����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��ɹ��˸��������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nFibreTotal), _T("��ɹ��˸��������")));

	//apGroup2->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nNotConnTotal));
	//apGroup2->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curZGW.m_nFibreTotal));

	apGroup2->GetSubItem(0)->AllowEdit(bEdit);
	apGroup2->GetSubItem(1)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup2.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("��������Դ���ͳ��")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), COleVariant(m_pArmy->m_curZGW.m_lComputerTotal), _T("��ͨ�������ĵ����ն�������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nServerTotal),_T("������������������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curZGW.m_nAdminTotal), _T("������������Ա������")));

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
	pPropGrid->EnableHeaderCtrl(TRUE,_T("ͳ������"),_T("���"));

	HDITEM item;
	item.cxy=200;     //��һ�еĿ��
	item.mask=HDI_WIDTH;

	pPropGrid->GetHeaderCtrl().SetItem(0, new HDITEM(item));

	pPropGrid->EnableDescriptionArea();
	pPropGrid->SetVSDotNetLook();
	pPropGrid->MarkModifiedProperties();
	pPropGrid->SetAlphabeticMode(FALSE);
	pPropGrid->SetShowDragContext();
	
	BOOL bEdit = FALSE; //pArmy->m_armys.GetChildCount()<=0;

	std::auto_ptr<CMFCPropertyGridProperty> apGroup1(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	

	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("�����Ѱ�װ������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nHadTotal), _T("�����Ѱ�װ������")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("������������������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nGoodTotal), _T("������������������")));
	apGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("��Ҫ����������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nNewTotal), _T("��������������")));

	//apGroup1->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curLW.m_nHadTotal));
	//apGroup1->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curLW.m_nGoodTotal));
	//apGroup1->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curLW.m_nNewTotal));

	apGroup1->GetSubItem(0)->AllowEdit(bEdit);
	apGroup1->GetSubItem(1)->AllowEdit(bEdit);
	apGroup1->GetSubItem(2)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup1.release());

	std::auto_ptr<CMFCPropertyGridProperty> apGroup2(new CMFCPropertyGridProperty(_T("�����������ͳ��")));	

	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�������ܹ��ϵ�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTotal), _T("�������ܹ��ϵ�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ȫ���𻵵�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadAllTotal), _T("ȫ���𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("�����𻵵�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTianXianTotal), _T("�����𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("֧���𻵵�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadZhiJiaTotal), _T("֧���𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("��Ƶͷ�𻵵�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadGPTTotal), _T("��Ƶͷ�𻵵�����")));
	apGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("ͨ�ӿ��𻵵�����"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nBadTSKTotal), _T("ͨ�ӿ��𻵵�����")));

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

	std::auto_ptr<CMFCPropertyGridProperty> apGroup3(new CMFCPropertyGridProperty(_T("������Դ���ͳ��")));

	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("�����ն�������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nComputerTotal), _T("��ͨ�����ĵ����ն�������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nServerTotal), _T("����������������")));
	apGroup3->AddSubItem(new CMFCPropertyGridProperty(_T("������Ա������"), CCommonFunction::Int2OleVariant(m_pArmy->m_curLW.m_nAdminTotal), _T("����������Ա������")));

	//apGroup3->GetSubItem(0)->SetData((DWORD)&(m_pArmy->m_curLW.m_nComputerTotal));
	//apGroup3->GetSubItem(1)->SetData((DWORD)&(m_pArmy->m_curLW.m_nServerTotal));
	//apGroup3->GetSubItem(2)->SetData((DWORD)&(m_pArmy->m_curLW.m_nAdminTotal));

	apGroup3->GetSubItem(0)->AllowEdit(bEdit);
	apGroup3->GetSubItem(1)->AllowEdit(bEdit);
	apGroup3->GetSubItem(2)->AllowEdit(bEdit);

	pPropGrid->AddProperty(apGroup3.release());
}