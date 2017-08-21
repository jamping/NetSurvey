// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// NetSurvey.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "NetSurvey.h"
#include "MainFrm.h"

#include "NetSurveyDoc.h"
#include "NetSurveyView.h"
#include "UserLoginDlg.h"
#include "LogsDlg.h"
#include "UserManageDlg.h"
#include "ModifyPassDlg.h"
//#include "MySplashThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetSurveyApp

BEGIN_MESSAGE_MAP(CNetSurveyApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CNetSurveyApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_LOGIN, &CNetSurveyApp::OnFileLogin)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOGIN, &CNetSurveyApp::OnUpdateFileLogin)
	ON_COMMAND(ID_TOOL_VIEW_USERS, &CNetSurveyApp::OnToolViewUsers)
	ON_UPDATE_COMMAND_UI(ID_TOOL_VIEW_USERS, &CNetSurveyApp::OnUpdateToolViewUsers)
	ON_COMMAND(ID_TOOL_VIEW_LOGS, &CNetSurveyApp::OnToolViewLogs)
	ON_UPDATE_COMMAND_UI(ID_TOOL_VIEW_LOGS, &CNetSurveyApp::OnUpdateToolViewLogs)
	ON_COMMAND(ID_MODIFY_PASS, &CNetSurveyApp::OnModifyPass)
	ON_UPDATE_COMMAND_UI(ID_MODIFY_PASS, &CNetSurveyApp::OnUpdateModifyPass)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DB_STATUS, &CNetSurveyApp::OnUpdateIndicatorDbStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_USER_STATUS, &CNetSurveyApp::OnUpdateIndicatorUserStatus)
END_MESSAGE_MAP()


// CNetSurveyApp ����

CNetSurveyApp::CNetSurveyApp()
{
	m_bHiColorIcons = TRUE;
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("NetSurvey.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_strUserName = _T("admin");
	m_nUserLevel = systemManager;
}

// Ψһ��һ�� CNetSurveyApp ����

CNetSurveyApp theApp;


// CNetSurveyApp ��ʼ��

BOOL CNetSurveyApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("NetSurvey"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	//Connect the database 
	if( !ConnectDB() )
	{
		do 
		{
			MessageBox(NULL,_T("���ݿ�����ʧ�ܣ������������Ӻ����ݿ����ò���"),_T("����"),MB_OK|MB_ICONWARNING);

			if(MessageBox(NULL,_T("�Ƿ������ݿ����ò�����"),_T("��ѡ��"),MB_YESNO|MB_ICONQUESTION)==IDNO )
				return FALSE;

			//COptionsDlg dlg(_T("������������"), m_pMainWnd);
			//dlg.SetValue(m_data);

			//if(dlg.DoModal()==IDOK)
			//{
			//	dlg.GetValue(m_data);
			//	//Set the autorun flag
			//	SetAutoRun(m_data.m_data.m_bAutoRun);
			//	//Set the max mission
			//	thread_pool::instance().initialize(m_data.m_data.m_nMaxMission/2,m_data.m_data.m_nMaxMission);
			//	//Save the params to regedit
			//	WriteProfile();
			//}
		} 
		while(!ConnectDB());		
	}

	//�û���½ 
	//�����½����
	for(int i=0;i<3;i++)
	{
		if(!Login())
		{
			if(MessageBox(NULL,_T("��½ʧ�ܣ��Ƿ����µ�½��"),_T("��ѡ��"),MB_YESNO|MB_ICONQUESTION)==IDNO )
				return FALSE;			
		}
		else
			break;
	} 

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNetSurveyDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CNetSurveyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//CSplashThreadDemo	*pThread = new CSplashThreadDemo();
	//pThread->SetParent(m_pMainWnd);
	//pThread->CreateThread();
	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}

int CNetSurveyApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	CloseDB();

	return CWinAppEx::ExitInstance();
}

// CNetSurveyApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CONTEXTMENU()
//	ON_WM_CREATE()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CNetSurveyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CNetSurveyApp �Զ������/���淽��

void CNetSurveyApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CNetSurveyApp::LoadCustomState()
{
}

void CNetSurveyApp::SaveCustomState()
{
}

// CNetSurveyApp ��Ϣ�������





void CAboutDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}
BOOL CNetSurveyApp::LoadProfile()
{

	return TRUE;
}
BOOL CNetSurveyApp::WriteProfile()
{
	
	return TRUE;
}
BOOL CNetSurveyApp::Login()
{
	BOOL bRet=FALSE;
	CUserLoginDlg dlgLog;

	dlgLog.m_strUserName=m_strUserName;	
	dlgLog.m_nUserLevel=(int)m_nUserLevel;

	if(dlgLog.DoModal() == IDOK)
	{
		m_strUserName=dlgLog.m_strUserName;
		m_strUserPasswd=dlgLog.m_strUserPasswd;
		m_nUserLevel=(UserLevelType)dlgLog.m_nUserLevel;
		m_nCurUserLevel=m_nUserLevel;

		bRet=TRUE;
	}
	MessageBox(NULL,bRet?_T("�Ϸ��û�����½�ɹ�"):_T("�Ƿ��û�����½ʧ��"),_T("��ע��"),MB_OK);

	return bRet;
}
BOOL CNetSurveyApp::DBIsOpen()
{
	if(m_pConn != NULL)
		return m_pConn->IsOpen();

	return FALSE;
}

BOOL CNetSurveyApp::ConnectDB()
{
	if(DBIsOpen())
		return TRUE;

	BOOL bRet = FALSE;

	CString strSql;
	strSql.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=auqf_pgf_rd1@#",CCommonFunction::GetFileFullPath(_T("NetSurveyDB")));

	if(m_pConn == NULL)
	{
		m_pConn = new CADODatabase;
	}
	if(m_pConn->Open(strSql))
	{
		bRet = TRUE;
	}
	else
	{
		delete m_pConn;
		m_pConn = NULL;
	}	

	return bRet;
}
void CNetSurveyApp::CloseDB()
{
	if(DBIsOpen())
	{	
		m_pConn->Close();
		delete m_pConn;
		m_pConn=NULL;
	}
}

CString CNetSurveyApp::GetUserDescName(UserLevelType type/* =unknowManager */)
{
	CString str=_T("");
	switch(type)
	{
	case superManager:
		str=_T("��������Ա");
		break;
	case systemManager:
		str=_T("ϵͳ����Ա");
		break;
	case commonManager:
		str=_T("��ͨ����Ա");
		break;
	case unknowManager:
		str=_T("δ֪�û�");
		break;
	default:
		ASSERT(FALSE);
	}
	return str;
}

void CNetSurveyApp::OnFileLogin()
{
	// TODO: Add your command handler code here
	if(GetCurUserLevel()==-1)
	{
		if( Login())
		{
			GetSourceTree()->InitUpdate();
		}		
	}
	else
	{
		if(MessageBox(NULL,_T("ע���û���"),_T("��ȷ��"),MB_YESNO|MB_ICONWARNING)==IDYES)
		{						
			m_nCurUserLevel = unknowManager;			
			m_strUserPasswd=_T("");

			GetSourceTree()->InitUpdate(FALSE);
			CloseDB();
		}
	}
}


void CNetSurveyApp::OnUpdateFileLogin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(GetCurUserLevel()!=-1?_T("ע��"):_T("��½"));
}


void CNetSurveyApp::OnToolViewUsers()
{
	// TODO: Add your command handler code here
	CUserManageDlg dlg;
	dlg.DoModal();
}


void CNetSurveyApp::OnUpdateToolViewUsers(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCurUserLevel()==2);
}


void CNetSurveyApp::OnToolViewLogs()
{
	// TODO: Add your command handler code here
	CWaitCursor wait;

	CLogsDlg dlg;
	dlg.DoModal();
}


void CNetSurveyApp::OnUpdateToolViewLogs(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCurUserLevel()>0);
}




void CNetSurveyApp::OnModifyPass()
{
	// TODO: Add your command handler code here
	CModifyPassDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CString strSql;		
		CDes des;
		
		strSql.Format("update netsurvey_user set user_passwd='%s' where user_name='%s'",
					  des.DesCrypteString(_T("auqfvfhpgu"),dlg.m_strNewPass1,1),m_strUserName);

		if(!theApp.DBIsOpen())
			theApp.ConnectDB();

		BOOL bRet = GetDB()->Execute(strSql);
		if(bRet)
		{
			m_strUserPasswd = dlg.m_strNewPass1;
		}
		MessageBox(NULL,bRet?_T("�����޸ĳɹ������ס�����룡"):_T("�����޸�ʧ�ܣ�"),_T("ע��"),MB_OK);
	   
		CLogs::WriteLogs("�޸��û�����",CLogs::log_admin);
	}
}


void CNetSurveyApp::OnUpdateModifyPass(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCurUserLevel()>0);
}


void CNetSurveyApp::OnUpdateIndicatorDbStatus(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(DBIsOpen()?_T("���ݿ�������"):_T("���ݿ�δ����"));
	pCmdUI->Enable();
}


void CNetSurveyApp::OnUpdateIndicatorUserStatus(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(_T("��ǰ�û���")+m_strUserName);
	pCmdUI->Enable();
}

