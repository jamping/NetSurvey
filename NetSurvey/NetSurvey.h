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

// NetSurvey.h : NetSurvey Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include "SourceTree.h"
#include "ListViewEx.h"
#include "TabbedView.h"
// CNetSurveyApp:
// �йش����ʵ�֣������ NetSurvey.cpp
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
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


private:
	// �����û���Ϣ
	CString                 m_strUserName;
	CString                 m_strUserDesc;
	CString                 m_strUserPasswd;
	UserLevelType           m_nUserLevel;

	UserLevelType           m_nCurUserLevel;                     
	// Database params	
	CADODatabase*           m_pConn;
	// ******************���й������ش���************************************
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
