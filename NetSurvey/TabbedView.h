#pragma once


// CTabbedView view

class CTabbedView : public CTabView
{
	DECLARE_DYNCREATE(CTabbedView)

protected:
	CTabbedView();           // protected constructor used by dynamic creation
	virtual ~CTabbedView();
public:
    CMFCPropertyGridCtrl* GetGridCtrl(int i=0);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	void ShowObject(CBaseObject* pOb);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


