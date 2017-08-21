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
		if(MessageBox("确认要删除所选内容吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
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
			MessageBox("数据删除失败");
		}
		else
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}
	else
	{
		if(MessageBox("数据库服务器断开,不能真正删除所选内容,继续吗","请确认",MB_YESNO|MB_ICONQUESTION) == IDYES)
			bRet=CCustomTree::DeleteSelection(bDelSelf);
	}

	return bRet;
}

void CSourceTree::InitUpdate(BOOL bReadOfDB/* =TRUE */,BOOL bShowInfo/* =FALSE */)
{
	CWaitCursor wait;
	// 清空现有数据源
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
		// 从数据库中重建数据源树
		strSql=_T("select * from netsurvey_army where parent_id  = \'0\'");

		if(rs.Open(theApp.GetDB()->GetActiveConnection(),strSql,CADORecordset::openQuery))
		{
			if(bShowInfo)
			{
				pBar=new CProgressBar(_T("准备读取部队信息……"),60,5000);
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
				pBar->Finish("正在初始化部队编制树……");	
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
		MessageBox(_T("数据已经更新，是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
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

		CProgressBar bar(_T("导出节点所有内容"),60,5000);
        //装载父亲节点
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

		bar.Finish(_T("导出数据完毕"));
	}
	
	MessageBox(bRet?_T("数据导出成功"):_T("数据导出失败"));
}


void CSourceTree::OnFresh()
{
	// TODO: Add your command handler code here
	CWaitCursor wait;
	//Delete the old content,but not from database
	CProgressBar* pBar=new CProgressBar(_T("刷新选择的节点内容"),60,5000);

	DeleteAllChildItem(GetSelectedObject()->GetRefItem());
	GetSelectedObject()->LoadFromDB(*theApp.GetDB(),pBar);
	GetSelectedObject()->ShowInTreeView(this,TRUE);

	pBar->Finish(_T("刷新节点完毕"));
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

			CProgressBar bar(_T("导入部队数据"),60,5000);
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
					MessageBox(_T("数据格式不匹配"));
					bRet=FALSE;
				}			
			}
			bar.Finish(_T("部队数据导入完毕"));
		}
	}
	else
	{
		bRet=FALSE;
	}

	MessageBox(bRet?_T("部队数据导入成功"):_T("部队数据导入失败"));
}



void CSourceTree::OnSave()
{
	// TODO: Add your command handler code here
	CString str;
	str.Format("保存%s部队信息吗?",GetSelectedObject()->GetDesc());

	if(MessageBox(str,_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	str.Format("正在保存%s部队信息",GetSelectedObject()->GetDesc());
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

		MessageBox(_T("保存成功!"));
	}
	else
		MessageBox(_T("保存失败!"));

	pBar->Finish(_T("保存完毕!"));
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

		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
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

		if(MessageBox(_T("是否保存到数据库"),_T("请确认"),MB_YESNO|MB_ICONQUESTION) == IDYES)
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
		 MessageBox(_T("数据导出到Excel文件失败"));
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
	//创建Excel 服务器(启动Excel) 
	if (!ExcelApp.CreateDispatch("Excel.Application",NULL)) 
	{ 
		AfxMessageBox("创建Excel服务失败!"); 
		exit(1); 
	} 
	ExcelApp.put_Visible(true); 
	//利用模板文件建立新文档 

	wbsMyBooks.AttachDispatch(ExcelApp.get_Workbooks(),true); 
	wbMyBook.AttachDispatch(wbsMyBooks.Add(_variant_t(CCommonFunction::GetFileFullPath(_T("netsurvey")))));
	//得到Worksheets 
	wssMysheets.AttachDispatch(wbMyBook.get_Worksheets(),true);
	//得到“政工网数据统计”sheet
	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("政工网数据统计")),true);

	// 得到所有需要报表的部队对象
	CTypedPtrArray<CPtrArray,CArmy*> armys;
	GetArmys((CArmy*)GetSelectedObject(),armys,dlg.m_nArmyLevel);
	// 政工网数据报表
	// 创建一个数组，用于填充表格
	VARIANT arrZGW;
	arrZGW.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sabZGW[2];
	//设置数组大小
	sabZGW[0].lLbound = 0; sabZGW[0].cElements = armys.GetSize();
	sabZGW[1].lLbound = 0; sabZGW[1].cElements = ('V'-'A')+1;

	arrZGW.parray = SafeArrayCreate(VT_VARIANT, 2, sabZGW);

	// 初始化数组内容
	VARIANT tmp;	
	long indices[] = {0,0};
	CStringArray data;
	// 装载数据
	for(int i=0; i<armys.GetSize(); i++) 
	{
		CArmy* pArmy = armys.GetAt(i);	
		pArmy->GetZGWReportData(data);

		indices[0] = i;
		for(int j=0;j<data.GetSize();j++)
		{
			indices[1] = j;
			// 添加数据到数组中
			VariantInit(&tmp);
			tmp.vt = VT_BSTR;
			tmp.bstrVal = data.GetAt(j).AllocSysString();			

			SafeArrayPutElement(arrZGW.parray, indices, (void *)&tmp);
		}
	}

	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("政工网数据统计")),true);		
	//设置2行1列的单元的值 
	CString str=COleDateTime::GetCurrentTime().Format("报表日期：%Y年%m月%d日");
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
	// 蓝网数据报表
	// 创建一个数组，用于填充表格
	//设置数组大小
	VARIANT arrLW;
	arrLW.vt = VT_ARRAY | VT_VARIANT;
	SAFEARRAYBOUND sabLW[2];
	//设置数组大小
	sabLW[0].lLbound = 0; sabLW[0].cElements = armys.GetSize();
	sabLW[1].lLbound = 0; sabLW[1].cElements = ('W'-'A')+1;

	arrLW.parray = SafeArrayCreate(VT_VARIANT, 2, sabLW);

	// 初始化数组内容
	// 装载数据
	for(int i=0; i<armys.GetSize(); i++) 
	{
		CArmy* pArmy = armys.GetAt(i);	
		pArmy->GetLWReportData(data);

		indices[0] = i;
		for(int j=0;j<data.GetSize();j++)
		{
			indices[1] = j;
			// 添加数据到数组中
			VariantInit(&tmp);
			tmp.vt = VT_BSTR;
			tmp.bstrVal = data.GetAt(j).AllocSysString();			

			SafeArrayPutElement(arrLW.parray, indices, (void *)&tmp);
		}
	}
	wsMysheet.ReleaseDispatch();
	wsMysheet.AttachDispatch(wssMysheets.get_Item(_variant_t("蓝网数据统计")),true);		
	//设置2行1列的单元的值 
	str=COleDateTime::GetCurrentTime().Format("报表日期：%Y年%m月%d日");
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
	//将表格保存
	// 	strPath = path;
	// 	strPath += "\\网络普查数据报表.xls";
	// 	wsMysheet.SaveAs(strPath,vtMissing,vtMissing,vtMissing,vtMissing,
	// 		vtMissing,vtMissing,vtMissing,vtMissing);
	//释放对象 
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
			// 下级所有部队
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