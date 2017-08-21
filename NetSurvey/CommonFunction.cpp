// CommonFunction.cpp: implementation of the CCommonFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetSurvey.h"
#include "CommonFunction.h"
#include <stack>
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CCommonFunction::Replace(CString& str,CString strOld,CString strNew)
{
	if(strOld.IsEmpty())
		return ;

	std::string s(str);
	std::string s1(strOld);
	std::string s2(strNew);

	int nPos=0;
	while ((nPos=s.find(strOld,nPos)) != -1) 
	{
 		s=s.substr(0,nPos)+s2+s.substr(nPos+s1.length());
		nPos+=s2.length();
	}
	str=s.c_str();
}
void CCommonFunction::CreateDir(CString FilePath)
{		
	Replace(FilePath,"/","\\");
    
	CString strTemp;
	std::stack<CString> dirs;

	for( int i = FilePath.GetLength()-1; i > 2; i-- )
	{
		if( FilePath[i] == '\\' )
		{
			 strTemp = FilePath.Left(i);

			 if( PathIsDirectory(strTemp) )
				 break;
			 else
				 dirs.push(strTemp);
		}
	}
	while ( dirs.size() ) 
	{
		CreateDirectory(dirs.top(), NULL); 
		dirs.pop();
	}
	
}


CString CCommonFunction::GetFileFullPath(CString filename)
{
	CString fullpath;
	fullpath=_T("");
	char path[256];

	::GetModuleFileName(NULL,path,256);
	fullpath=(CString)path;
	fullpath=fullpath.Left(fullpath.ReverseFind('\\')+1)+filename;
    
	return fullpath;
}
CString CCommonFunction::AllocateID()
{
	CString strID=_T("");
	GUID guid = GUID_NULL;

	if (S_OK == ::CoCreateGuid(&guid))
	{
		strID.Format("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}
 	return strID;
}
int CCommonFunction::OleDateTime2Int(const COleDateTime& time,BOOL bDate/* =TRUE */)
{
	// Output format is 20061212 or 083030
	int nRet=0;
	if(bDate)
	{
        nRet=time.GetYear()*10000+time.GetMonth()*100+time.GetDay();
	}
	else
	{
		nRet=time.GetHour()*10000+time.GetMinute()*100+time.GetSecond();
	}
	return nRet;
}
COleDateTime CCommonFunction::Int2OleDateTime(int nValue,BOOL bDate/* =TRUE */)
{
	COleDateTime time;
	int nLeft,nMiddle,nRight;
	
	nLeft=nValue/10000;
	nMiddle=(nValue-nLeft*10000)/100;
	nRight=nValue-nLeft*10000-nMiddle*100;

	if(bDate)
	{
		time.SetDate(nLeft,nMiddle,nRight);
	}
	else
	{
		time.SetTime(nLeft,nMiddle,nRight);
	}
	if(time.GetStatus()!=COleDateTime::invalid)
	   return time;
	else
		return COleDateTime::GetCurrentTime();
}
BOOL CCommonFunction::IsSameString(CString str1,CString str2)
{
	return str1.CompareNoCase(str2)==0;
}

COleVariant CCommonFunction::Int2OleVariant(int i)
{
	COleVariant val;
	val.vt = VT_I2;
	val.iVal = i;

	return val;
}
int CCommonFunction::OleVariant2Int(COleVariant val)
{
	int nRet = (int)NULL;
		
	switch(val.vt)
	{
	case VT_BOOL:
		nRet = val.boolVal;
		break;
	case VT_I2:
	case VT_UI1:
		nRet = val.iVal;
		break;
	case VT_INT:
		nRet = val.intVal;
		break;
	case VT_NULL:
	case VT_EMPTY:
		nRet = 0;
		break;
	default:
		nRet = val.iVal;
	}	
	return nRet;
}