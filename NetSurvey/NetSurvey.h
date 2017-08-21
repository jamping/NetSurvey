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

// NetSurvey.h : NetSurvey 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include "SourceTree.h"
#include "ListViewEx.h"
#include "TabbedView.h"
// CNetSurveyApp:
// 有关此类的实现，请参阅 NetSurvey.cpp
//

class CNetSurveyApp : public CWinAppEx
{
public:
	enum ArmyLevelType { unknowArmy=-1,junArmy,shilvArmy,tuanArmy,yingArmy,lianArmy,paiArmy,banArmy};
	enum UserLevelType { unknowManager=-1,commonManager,systemManager,superManager};

	CNetSurveyApp();

	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	CString GetUserDescName(UserLevelType type=unknowManager);
	int  GetCurUserLevel() { return (int)m_nCurUserLevel; }
	CString GetCurUserName() { return m_strUserName; }
	CString GetCurUserDesc() { return m_strUserDesc; }
	CString GetCurUserPasswd() { return m_strUserPasswd; }

	void SetSourceTree(CWnd *pWnd) {m_pwndSourceTree=(CSourceTree*)pWnd;}
	CSourceTree* GetSourceTree(){return m_pwndSourceTree;}
	void SetShowSelfView(CWnd* pWnd) { m_pwndShowSelfView=(CTabbedView*)pWnd; }
	CTabbedView* GetShowSelfView() { return m_pwndShowSelfView; }
	void SetShowChildView(CWnd* pWnd) { m_pwndShowChildView=(CListViewEx*)pWnd; }
	CListViewEx* GetShowChildView() { return m_pwndShowChildView; }

	BOOL DBIsOpen();	
	BOOL ConnectDB();	
	void CloseDB();
	CADODatabase* GetDB() { return m_pConn; };

	BOOL LoadProfile();
	BOOL WriteProfile();
	BOOL Login();
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


private:
	// 保存用户信息
	CString                 m_strUserName;
	CString                 m_strUserDesc;
	CString                 m_strUserPasswd;
	UserLevelType           m_nUserLevel;

	UserLevelType           m_nCurUserLevel;                     
	// Database params	
	CADODatabase*           m_pConn;
	// ******************集中管理各相关窗口************************************
	CSourceTree*            m_pwndSourceTree;
	CTabbedView*            m_pwndShowSelfView;
	CListViewEx*            m_pwndShowChildView;
public:
	afx_msg void OnFileLogin();
	afx_msg void OnUpdateFileLogin(CCmdUI *pCmdUI);
	afx_msg void OnToolViewUsers();
	afx_msg void OnUpdateToolViewUsers(CCmdUI *pCmdUI);
	afx_msg void OnToolViewLogs();
	afx_msg void OnUpdateToolViewLogs(CCmdUI *pCmdUI);
	afx_msg void OnModifyPass();
	afx_msg void OnUpdateModifyPass(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorDbStatus(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorUserStatus(CCmdUI *pCmdUI);
};

extern CNetSurveyApp theApp;
