#pragma once
#include "afxdockablepane.h"
#include "SourceTree.h"

class CSourceBar :	public CDockablePane
{
public:
	CSourceBar(void);
	~CSourceBar(void);

	CSourceTree    m_wndTree;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};

