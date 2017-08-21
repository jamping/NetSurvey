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

// NetSurveyView.cpp : CNetSurveyView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "NetSurvey.h"
#endif

#include "NetSurveyDoc.h"
#include "NetSurveyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetSurveyView

IMPLEMENT_DYNCREATE(CNetSurveyView, CView)

BEGIN_MESSAGE_MAP(CNetSurveyView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNetSurveyView ����/����

CNetSurveyView::CNetSurveyView()
{
	// TODO: �ڴ˴���ӹ������

}

CNetSurveyView::~CNetSurveyView()
{
}

BOOL CNetSurveyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CNetSurveyView ����

void CNetSurveyView::OnDraw(CDC* /*pDC*/)
{
	CNetSurveyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CNetSurveyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNetSurveyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CNetSurveyView ���

#ifdef _DEBUG
void CNetSurveyView::AssertValid() const
{
	CView::AssertValid();
}

void CNetSurveyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNetSurveyDoc* CNetSurveyView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetSurveyDoc)));
	return (CNetSurveyDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetSurveyView ��Ϣ�������
