#include "StdAfx.h"
#include "SourceTree.h"
#include "NetSurvey.h"
#include "progressbar.h"
#include "FilePathDlg.h"
#include "ExportExcelDlg.h"
#include "Application.h"
#include "Workbooks.h"
#include "Workbook.h"
#include "Worksheets.h"
#include "Worksheet.h"
#include "Range.h"
#include "Borders.h"

CSourceTree::CSourceTree(void):m_objectHelper(HELPER_SOURCE)
{
	m_objectHelper.CanDelete()=FALSE;	
}


CSourceTree::~CSourceTree(void)
{
	// clear memory
	InitUpdate(FALSE);
}
BOOL CSourceTree::DeleteSelection(BOOL bDelSelf/* =TRUE */)
{
	BOOL bRet=TRUE;
	CPtrList list;

	if(IsMultiSelect())
		GetMultiSelectedObjects(list);
	else
		list.AddTail(GetSelectedObject());

	CBaseObject *pOB;

	if(theApp.DBIsOpen())
	{
		if(MessageBox("ȷ��Ҫɾ����ѡ������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			POSITION pos=list.GetHeadPosition();
			while (pos)
			{
				pOB=(CBaseObject*)list.GetNext(pos);
				bRet&=pOB->DeleteFromDB(*theApp.GetDB(),bDelSelf);				
			}

		}
		else
			return FALSE;

		if(!bRet)
		{
			MessageBox("����ɾ��ʧ��");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("���ݿ�������Ͽ�,��������ɾ����ѡ����,������","��ȷ��",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}

	return bRet;
}

void CSourceTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
	CWaitCursor wait;
	// �����������Դ
	POSITION pos=m_objectHelper.GetFirstChildPosition();
	while(pos)
	{
		CBaseObject *pChild=m_objectHelper.GetNextChild(pos);
		if(pChild != NULL)
		{
			DeleteItem(pChild->GetRefItem());
			if(pChild->CanDelete())
			{
				pChild->LeaveFrom(&m_objectHelper);
				delete pChild;
			}
		}
	}

	if(theApp.DBIsOpen() && bReadOfDB)
	{
		CADORecordset rs;
		CString strSql,str;
		CProgressBar* pBar=NULL;
		// �����ݿ����ؽ�����Դ��
		strSql=_T("select * from netsurvey_army where parent_id  = \'0\'");

		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
				pBar=new CProgressBar(_T("׼����ȡ������Ϣ����"),60,5000);
			}

			CArmy *pArmy;
			while(!rs.IsEOF())
			{
				pArmy=new CArmy;
				pArmy->LoadInfo(rs);
					
				pArmy->AddTo(&m_objectHelper);
				pArmy->LoadFromDB(*theApp.GetDB(),pBar);

				rs.MoveNext();
			}
			rs.Close();

			if(bShowInfo)
			{
				pBar->Finish("���ڳ�ʼ�����ӱ���������");	
				delete pBar;
				pBar=NULL;
			}
		}
	}
	m_objectHelper.ShowInTreeView(this,TRUE);
	m_objectHelper.STATData();
}
BOOL CSourceTree::DoDrop(HTREEITEM hItemDragged, HTREEITEM hItemTarget, int nAction)
{
	CBaseObject *pObDrag,*pObTarget;

	pObDrag=(CBaseObject *)GetItemData(hItemDragged);
	pObTarget= (CBaseObject *)GetItemData(hItemTarget);

	if(!pObTarget->CanAccept(pObDrag,this))
		return FALSE;

	//Delete from tree view
	DeleteItem(hItemDragged);   
	//Add objects to target and show in the tree
	pObDrag->AddTo(pObTarget);
	pObDrag->ShowInTreeView(this,TRUE);

	//Save object
	if(theApp.DBIsOpen())
		pObTarget->SaveToDB(*theApp.GetDB());

	std::stack<CBaseObject*> armys;
	armys.push(pObDrag);

	CBaseObject* pTopArmy = pObDrag->BackTo(OBJECT_TYPE_ARMY);
	while( pTopArmy )
	{
		armys.push(pTopArmy);
		pTopArmy = pTopArmy->BackTo(OBJECT_TYPE_ARMY);
	}
	armys.top()->STATData();

	armys.push(pObTarget);
	pTopArmy = pObTarget->BackTo(OBJECT_TYPE_ARMY);
	while( pTopArmy )
	{
		armys.push(pTopArmy);
		pTopArmy = pTopArmy->BackTo(OBJECT_TYPE_ARMY);
	}
	armys.top()->STATData();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CSourceTree, CCustomTree)
	ON_COMMAND(ID_COLLAPSE, &CSourceTree::OnCollapse)
	ON_COMMAND(ID_DELETE, &CSourceTree::OnDelete)
	ON_COMMAND(ID_EDIT, &CSourceTree::OnEdit)
	ON_COMMAND(ID_EXPORT, &CSourceTree::OnExport)
	ON_COMMAND(ID_FRESH, &CSourceTree::OnFresh)
	ON_COMMAND(ID_IMPORT, &CSourceTree::OnImport)
	ON_COMMAND(ID_SAVE, &CSourceTree::OnSave)
	ON_COMMAND(ID_ROOT_NEW, &CSourceTree::OnRootNew)
	ON_COMMAND(ID_ARMY_NEW, &CSourceTree::OnArmyNew)
	ON_COMMAND(ID_ROOT_FRESH, &CSourceTree::OnRootFresh)
	ON_COMMAND(ID_ROOT_DELETE, &CSourceTree::OnRootDelete)
	ON_COMMAND(ID__EXCEL, &CSourceTree::OnExcel)
END_MESSAGE_MAP()


void CSourceTree::OnCollapse()
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	Expand(GetSelectedObject()->GetRefItem(),TVE_COLLAPSE,TRUE);
}


void CSourceTree::OnDelete()
{
	// TODO: Add your command handler code here
	DeleteSelection();
	m_objectHelper.STATData();
}


void CSourceTree::OnEdit()
{
	// TODO: Add your command handler code here
	if(GetSelectedObject()->EditObject(this,FALSE/*!(theApp.GetUserLevel()>0)*/)==IDOK &&
		GetSelectedObject()->IsModified() &&
		MessageBox(_T("�����Ѿ����£��Ƿ񱣴浽���ݿ�"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
	{
		GetSelectedObject()->SaveToDB(*theApp.GetDB(),FALSE);

		std::stack<CBaseObject*> armys;
		armys.push(GetSelectedObject());

		CBaseObject* pTopArmy = GetSelectedObject()->BackTo(OBJECT_TYPE_ARMY);
		while( pTopArmy )
		{
			armys.push(pTopArmy);
			pTopArmy = pTopArmy->BackTo(OBJECT_TYPE_ARMY);
		}
		armys.top()->STATData();
	}
}


void CSourceTree::OnExport()
{
	// TODO: Add your command handler code here
	BOOL bRet = FALSE;
	CFilePathDlg dlg;

	if(dlg.DoModal()==IDOK && !dlg.m_strFilePath.IsEmpty())
	{   
		CWaitCursor wait;

		CProgressBar bar(_T("�����ڵ���������"),60,5000);
        //װ�ظ��׽ڵ�
		std::stack<CBaseObject*> s;
		CBaseObject* pOb=GetSelectedObject()->GetParent();
		while (pOb)
		{
			s.push(pOb);
			pOb=pOb->GetParent();
		}
		TiXmlDocument doc;
		TiXmlDeclaration * pDecl=new TiXmlDeclaration(_T("1.0"),_T("gb2312"),_T("yes"));
		
		doc.LinkEndChild(pDecl);
		TiXmlNode* pNode=(TiXmlNode*)&doc;
		while (s.size())
		{
			pNode=s.top()->StreamOut(pNode,FALSE,&bar);
			s.pop();
		}
		GetSelectedObject()->StreamOut(pNode,TRUE,&bar);

		CString strFile;
		strFile.Format("%s\\%s.xml",dlg.m_strFilePath,GetSelectedObject()->GetDesc());
		CCommonFunction::CreateDir(strFile);
		bRet=doc.SaveFile(strFile);			

		bar.Finish(_T("�����������"));
	}
	
	MessageBox(bRet?_T("���ݵ����ɹ�"):_T("���ݵ���ʧ��"));
}


void CSourceTree::OnFresh()
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	//Delete the old content,but not from database
	CProgressBar* pBar=new CProgressBar(_T("ˢ��ѡ��Ľڵ�����"),60,5000);

	DeleteAllChildItem(GetSelectedObject()->GetRefItem());
	GetSelectedObject()->LoadFromDB(*theApp.GetDB(),pBar);
	GetSelectedObject()->ShowInTreeView(this,TRUE);

	pBar->Finish(_T("ˢ�½ڵ����"));
	delete pBar;
	pBar=NULL;
}


void CSourceTree::OnImport()
{
	// TODO: Add your command handler code here
	char szFilters[]="Xml Files (*.xml)|*.xml||";
	CString strFileName;
	BOOL bRet=TRUE;

	CFileDialog fileDlg(TRUE,"","",OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if( fileDlg.DoModal ()==IDOK )
	{
		CWaitCursor wait;
		strFileName= fileDlg.GetPathName();
		TiXmlDocument doc(strFileName);
		bRet&=(BOOL)doc.LoadFile();

		if(bRet)
		{
			TiXmlHandle hDoc(&doc);		    
			TiXmlElement* pElement=NULL;

			CProgressBar bar(_T("���벿������"),60,5000);
			pElement=hDoc.FirstChildElement(_T("netsurvey_root")).ToElement();

			if(pElement)
			{
				int nType=-1;	
				pElement->QueryIntAttribute(_T("type"),&nType);
				if(nType==0)
				{
					bRet&=GetSelectedObject()->StreamIn(pElement,TRUE,&bar);
					GetSelectedObject()->ShowInTreeView(this,TRUE);
					GetSelectedObject()->STATData();
				}
				else
				{
					MessageBox(_T("���ݸ�ʽ��ƥ��"));
					bRet=FALSE;
				}			
			}
			bar.Finish(_T("�������ݵ������"));
		}
	}
	else
	{
		bRet=FALSE;
	}

	MessageBox(bRet?_T("�������ݵ���ɹ�"):_T("�������ݵ���ʧ��"));
}



void CSourceTree::OnSave()
{
	// TODO: Add your command handler code here
	CString str;
	str.Format("����%s������Ϣ��?",GetSelectedObject()->GetDesc());

	if(MessageBox(str,_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	str.Format("���ڱ���%s������Ϣ",GetSelectedObject()->GetDesc());
	CProgressBar* pBar=new CProgressBar(str,60,5000);	

	if(GetSelectedObject()->SaveToDB(*theApp.GetDB(),TRUE,pBar))
	{
		std::stack<CBaseObject*> armys;
		armys.push(GetSelectedObject());

		CBaseObject* pTopArmy = GetSelectedObject()->BackTo(OBJECT_TYPE_ARMY);
		while( pTopArmy )
		{
			armys.push(pTopArmy);
			pTopArmy = pTopArmy->BackTo(OBJECT_TYPE_ARMY);
		}
		armys.top()->STATData();

		MessageBox(_T("����ɹ�!"));
	}
	else
		MessageBox(_T("����ʧ��!"));

	pBar->Finish(_T("�������!"));
	delete pBar;
	pBar=NULL;
}


void CSourceTree::OnRootNew()
{
	// TODO: Add your command handler code here
	CArmy *pArmy = new CArmy;

	if(pArmy->EditObject(this,FALSE) == IDOK)
	{
		pArmy->SetID(CCommonFunction::AllocateID());		
		pArmy->AddTo(&m_objectHelper);
		pArmy->CreateSurveys();
		pArmy->ShowInTreeView(this,TRUE);

		if(MessageBox(_T("�Ƿ񱣴浽���ݿ�"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			pArmy->SaveToDB(*theApp.GetDB());				
		}
	}
	else
		delete pArmy;
}


void CSourceTree::OnArmyNew()
{
	// TODO: Add your command handler code here
	CArmy *pArmy = new CArmy;

	CArmy* pParent = (CArmy*) GetSelectedObject();	

	pArmy->SetID(CCommonFunction::AllocateID());
	pArmy->AddTo(&(pParent->m_armys));

	if(pArmy->EditObject(this,FALSE) == IDOK)
	{				
		pArmy->CreateSurveys();
		pArmy->ShowInTreeView(this,TRUE);

		if(MessageBox(_T("�Ƿ񱣴浽���ݿ�"),_T("��ȷ��"),MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			pArmy->SaveToDB(*theApp.GetDB());

			std::stack<CBaseObject*> armys;
			armys.push(pArmy);

			CBaseObject* pTopArmy = pArmy->BackTo(OBJECT_TYPE_ARMY);
			while( pTopArmy )
			{
				armys.push(pTopArmy);
				pTopArmy = pTopArmy->BackTo(OBJECT_TYPE_ARMY);
			}
			armys.top()->STATData();
		}
	}
	else
	{
		pArmy->LeaveFrom(&pParent->m_armys);
		delete pArmy;
	}
}


void CSourceTree::OnRootFresh()
{
	// TODO: Add your command handler code here
	InitUpdate(TRUE,TRUE);
}


void CSourceTree::OnRootDelete()
{
	// TODO: Add your command handler code here
	DeleteSelection(FALSE);
}


void CSourceTree::OnExcel()
{
	// TODO: Add your command handler code here
	CExportExcelDlg dlg;
	if( dlg.DoModal() == IDCANCEL )
	{
		 MessageBox(_T("���ݵ�����Excel�ļ�ʧ��"));
		 return;
	}
	
	CWaitCursor wait;

	CApplication ExcelApp; 
	CWorkbooks wbsMyBooks; 
	CWorkbook wbMyBook; 
	CWorksheets wssMysheets; 
	CWorksheet wsMysheet; 
	CRange rgMyRge; 	
	CBorders borders;
	//����Excel ������(����Excel) 
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
		AfxMessageBox("����Excel����ʧ��!"); 
		exit(1); 
	} 
	ExcelApp.put_Visible(true); 
	//����ģ���ļ��������ĵ� 

	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks(),true); 
	wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(CCommonFunction::GetFileFullPath(_T("netsurvey")))));
	//�õ�Worksheets 
	wssMysheets.AttachDispatch(wbMyBook.get_Worksheets(),true);
	//�õ�������������ͳ�ơ�sheet
	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("����������ͳ��")),true);

	// �õ�������Ҫ����Ĳ��Ӷ���
	CTypedPtrArray<CPtrArray,CArmy*> armys;
	GetArmys((CArmy*)GetSelectedObject(),armys,dlg.m_nArmyLevel);
	// ���������ݱ���
	// ����һ�����飬���������
	VARIANT arrZGW;
	arrZGW.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sabZGW[2];
	//���������С
	sabZGW[0].lLbound = 0; sabZGW[0].cElements = armys.GetSize();
	sabZGW[1].lLbound = 0; sabZGW[1].cElements = ('V'-'A')+1;

	arrZGW.parray = SafeArrayCreate(VT_VARIANT, 2, sabZGW);

	// ��ʼ����������
	VARIANT tmp;	
	long indices[] = {0,0};
	CStringArray data;
	// װ������
	for(int i=0; i<armys.GetSize(); i++) 
	{
		CArmy* pArmy = armys.GetAt(i);	
		pArmy->GetZGWReportData(data);

		indices[0] = i;
		for(int j=0;j<data.GetSize();j++)
		{
			indices[1] = j;
			// ������ݵ�������
			VariantInit(&tmp);
			tmp.vt = VT_BSTR;
			tmp.bstrVal = data.GetAt(j).AllocSysString();			

			SafeArrayPutElement(arrZGW.parray, indices, (void *)&tmp);
		}
	}

	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("����������ͳ��")),true);		
	//����2��1�еĵ�Ԫ��ֵ 
	CString str=COleDateTime::GetCurrentTime().Format("�������ڣ�%Y��%m��%d��");
	rgMyRge = wsMysheet.get_Range(_variant_t("A2"),_variant_t("A2"));
	rgMyRge.put_Value2(_variant_t(str)); 
	rgMyRge.ReleaseDispatch();

	str.Format("V%d",armys.GetSize()+5);
	rgMyRge = wsMysheet.get_Range(_variant_t("A6"),_variant_t(str));

	rgMyRge.put_Value2(arrZGW); 

	borders.AttachDispatch(rgMyRge.get_Borders(),true);
	borders.put_LineStyle(_variant_t((long)1));	
	borders.ReleaseDispatch();
	rgMyRge.ReleaseDispatch();

	SafeArrayDestroy(arrZGW.parray);
	arrZGW.parray = NULL;
	VariantClear(&arrZGW);	
	// �������ݱ���
	// ����һ�����飬���������
	//���������С
	VARIANT arrLW;
	arrLW.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sabLW[2];
	//���������С
	sabLW[0].lLbound = 0; sabLW[0].cElements = armys.GetSize();
	sabLW[1].lLbound = 0; sabLW[1].cElements = ('W'-'A')+1;

	arrLW.parray = SafeArrayCreate(VT_VARIANT, 2, sabLW);

	// ��ʼ����������
	// װ������
	for(int i=0; i<armys.GetSize(); i++) 
	{
		CArmy* pArmy = armys.GetAt(i);	
		pArmy->GetLWReportData(data);

		indices[0] = i;
		for(int j=0;j<data.GetSize();j++)
		{
			indices[1] = j;
			// ������ݵ�������
			VariantInit(&tmp);
			tmp.vt = VT_BSTR;
			tmp.bstrVal = data.GetAt(j).AllocSysString();			

			SafeArrayPutElement(arrLW.parray, indices, (void *)&tmp);
		}
	}
	wsMysheet.ReleaseDispatch();
	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("��������ͳ��")),true);		
	//����2��1�еĵ�Ԫ��ֵ 
	str=COleDateTime::GetCurrentTime().Format("�������ڣ�%Y��%m��%d��");
	rgMyRge = wsMysheet.get_Range(_variant_t("A2"),_variant_t("A2"));
	rgMyRge.put_Value2(_variant_t(str)); 
	rgMyRge.ReleaseDispatch();

	str.Format("W%d",armys.GetSize()+5);
	rgMyRge = wsMysheet.get_Range(_variant_t("A6"),_variant_t(str));
	rgMyRge.put_Value2(arrLW); 

	borders.AttachDispatch(rgMyRge.get_Borders(),true);
	borders.put_LineStyle(_variant_t((long)1));	
	borders.ReleaseDispatch();
	rgMyRge.ReleaseDispatch();

	SafeArrayDestroy(arrLW.parray);	
	arrLW.parray = NULL;
	VariantClear(&arrLW);	
	//����񱣴�
	// 	strPath = path;
	// 	strPath += "\\�����ղ����ݱ���.xls";
	// 	wsMysheet.SaveAs(strPath,vtMissing,vtMissing,vtMissing,vtMissing,
	// 		vtMissing,vtMissing,vtMissing,vtMissing);
	//�ͷŶ��� 
	wsMysheet.ReleaseDispatch(); 
	wssMysheets.ReleaseDispatch(); 
	wbMyBook.ReleaseDispatch(); 
	wbsMyBooks.ReleaseDispatch(); 
	ExcelApp.ReleaseDispatch(); 	
}

void CSourceTree::GetArmys(CArmy* pArmy,CTypedPtrArray<CPtrArray,CArmy*> &armys,int nArmyLevel)
{
	if(pArmy)
	{
		if( !pArmy->IsXuniArmy() && pArmy->GetArmyLevel() <= nArmyLevel  )
		{
			armys.Add(pArmy);
		}
		if( pArmy->GetArmyLevel() <= nArmyLevel  )
		{
			// �¼����в���
			POSITION pos=pArmy->m_armys.GetFirstChildPosition();
			while (pos)
			{
				CArmy* pChild=(CArmy*)pArmy->m_armys.GetNextChild(pos);
				if(pChild)
				{
					GetArmys(pChild,armys,nArmyLevel);	
				}
			}
		}		
	}
}