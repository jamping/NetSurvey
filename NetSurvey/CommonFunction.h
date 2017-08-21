// CommonFunction.h: interface for the CCommonFunction class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CCommonFunction  
{
public:	
	static void CreateDir(CString FilePath);
	static void Replace(CString& str,CString strOld,CString strNew);  	
    static CString GetFileFullPath(CString filename);
	static CString AllocateID();

	static COleDateTime Int2OleDateTime(int nValue,BOOL bDate=TRUE);
	static int OleDateTime2Int(const COleDateTime& time,BOOL bDate=TRUE);
	static BOOL CCommonFunction::IsSameString(CString str1,CString str2);
	static COleVariant Int2OleVariant(int i);
	static int OleVariant2Int(COleVariant val);
};