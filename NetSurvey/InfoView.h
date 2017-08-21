#pragma once



// CInfoView form view

class CInfoView : public CFormView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // protected constructor used by dynamic creation
	virtual ~CInfoView();

public:
	enum { IDD = IDD_INFO };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMFCPropertyGridCtrl m_info;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};


