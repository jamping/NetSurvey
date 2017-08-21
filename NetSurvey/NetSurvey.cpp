// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// NetSurvey.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
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


// CNetSurveyApp 构造

CNetSurveyApp::CNetSurveyApp()
{
	m_bHiColorIcons = TRUE;
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("NetSurvey.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_strUserName = _T("admin");
	m_nUserLevel = systemManager;
}

// 唯一的一个 CNetSurveyApp 对象

CNetSurveyApp theApp;


// CNetSurveyApp 初始化

BOOL CNetSurveyApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("NetSurvey"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	//Connect the database 
	if( !ConnectDB() )
	{
		do 
		{
			MessageBox(NULL,_T("数据库连接失败，请检查网络连接和数据库配置参数"),_T("错误"),MB_OK|MB_ICONWARNING);

			if(MessageBox(NULL,_T("是否检查数据库配置参数？"),_T("请选择"),MB_YESNO|MB_ICONQUESTION)==IDNO )
				return FALSE;

			//COptionsDlg dlg(_T("基本参数配置"), m_pMainWnd);
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

	//用户登陆 
	//允许登陆三次
	for(int i=0;i<3;i++)
	{
		if(!Login())
		{
			if(MessageBox(NULL,_T("登陆失败，是否重新登陆？"),_T("请选择"),MB_YESNO|MB_ICONQUESTION)==IDNO )
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

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNetSurveyDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CNetSurveyView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//CSplashThreadDemo	*pThread = new CSplashThreadDemo();
	//pThread->SetParent(m_pMainWnd);
	//pThread->CreateThread();
	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int CNetSurveyApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	CloseDB();

	return CWinAppEx::ExitInstance();
}

// CNetSurveyApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CNetSurveyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CNetSurveyApp 自定义加载/保存方法

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

// CNetSurveyApp 消息处理程序





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
	MessageBox(NULL,bRet?_T("合法用户，登陆成功"):_T("非法用户，登陆失败"),_T("请注意"),MB_OK);

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
		str=_T("超级管理员");
		break;
	case systemManager:
		str=_T("系统管理员");
		break;
	case commonManager:
		str=_T("普通操作员");
		break;
	case unknowManager:
		str=_T("未知用户");
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
		if(MessageBox(NULL,_T("注销用户吗？"),_T("请确认"),MB_YESNO|MB_ICONWARNING)==IDYES)
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
	pCmdUI->SetText(GetCurUserLevel()!=-1?_T("注销"):_T("登陆"));
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
		MessageBox(NULL,bRet?_T("密码修改成功，请记住新密码！"):_T("密码修改失败！"),_T("注意"),MB_OK);
	   
		CLogs::WriteLogs("修改用户密码",CLogs::log_admin);
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
	pCmdUI->SetText(DBIsOpen()?_T("数据库已连接"):_T("数据库未连接"));
	pCmdUI->Enable();
}


void CNetSurveyApp::OnUpdateIndicatorUserStatus(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetText(_T("当前用户：")+m_strUserName);
	pCmdUI->Enable();
}

