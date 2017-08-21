#pragma once


// CExportExcelDlg dialog

class CExportExcelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportExcelDlg)

public:
	CExportExcelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportExcelDlg();

// Dialog Data
	enum { IDD = IDD_EXCEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nArmyLevel;
	virtual BOOL OnInitDialog();
};
