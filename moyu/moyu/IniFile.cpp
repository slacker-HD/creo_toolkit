// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "IniFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{
	nMaxSize_All_SectionNames = 1024;
	nMaxSize_A_Section = 1024;
	SetDefaultFilePathName();
}
CIniFile::CIniFile(CString FileFullName)
{
	nMaxSize_All_SectionNames = 1024;
	nMaxSize_A_Section = 1024;
	m_strFileFullName = FileFullName;
}
CIniFile::~CIniFile()
{
}

BOOL CIniFile::IsIniFileExist()
{
	CFile MyFile;
	if (!MyFile.Open(m_strFileFullName, CFile::modeRead))
		return 0;
	MyFile.Close();
	return 1;
}

BOOL CIniFile::EnsureIniFileExist()
{
	if (IsIniFileExist())
		return 1;

	CFile MyFile;

	if (!MyFile.Open(m_strFileFullName, CFile::modeCreate))
		return FALSE;

	MyFile.Close();
	IniFile_InitializeForCreate();
	return TRUE;
}

void CIniFile::SetDefaultFilePathName()
{
	TCHAR exeFullPath[MAX_PATH];
	int len2 = GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	CString strDir(exeFullPath);
	strDir.TrimLeft();
	strDir.TrimRight();

	CString strTemp = strDir.Left(strDir.GetLength() - 3);
	strDir = strTemp;
	strDir += _T("ini");
	m_strFileFullName = strDir;
	return;
}

BOOL CIniFile::IniFile_WriteString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	return WritePrivateProfileString(lpSectionName, lpKeyName, lpString, m_strFileFullName);
}

BOOL CIniFile::IniFile_WriteInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, int nValue)
{
	CString strTemp;
	strTemp.Format(_T("%d"), nValue);
	return WritePrivateProfileString(lpSectionName, lpKeyName, strTemp, m_strFileFullName);
}

UINT CIniFile::IniFile_GetInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, INT nDefault)
{
	return GetPrivateProfileInt(lpSectionName, lpKeyName, nDefault, m_strFileFullName);
}

DWORD CIniFile::IniFile_GetString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	return GetPrivateProfileString(lpSectionName, lpKeyName, lpDefault, lpReturnedString, nSize, m_strFileFullName);
}

DWORD CIniFile::IniFile_GetString(CString strSectionName, CString strKeyName, CString strDefault, CString &strReturnedString)
{
	char buf[256];
	DWORD len = GetPrivateProfileString(strSectionName, strKeyName, strDefault, buf, 256, m_strFileFullName);
	buf[len] = 0;
	strReturnedString.Format("%s", buf);
	return len;
}

CString CIniFile::GetFilePathName()
{
	return m_strFileFullName;
}

BOOL CIniFile::IniFile_ModifyInt(LPCTSTR lpSectionName, LPCTSTR lpKeyName, int nValue)
{
	return IniFile_WriteInt(lpSectionName, lpKeyName, nValue);
}

BOOL CIniFile::IniFile_ModifyString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	return IniFile_WriteString(lpSectionName, lpKeyName, lpString);
}

BOOL CIniFile::IniFile_WriteLongString(LPCTSTR lpValue, LPCTSTR lpSectionName, LPCTSTR lpKeyName, ...)
{
	// 此函数不允许键名和键值为空的
	va_list pArgList;
	LPCTSTR p;

	if (lpValue == NULL || lpSectionName == NULL || lpKeyName == NULL)
		return FALSE;
	CString strSectionName(lpSectionName);
	CString strKeyName(lpKeyName);
	BOOL result = FALSE;
	va_start(pArgList, lpKeyName); /* Initialize variable arguments. */
	do
	{
		p = va_arg(pArgList, LPCTSTR);
		CString strTempValue1(p);
		if (strTempValue1 == "ListEnd" || strTempValue1 == _T(""))
			break;
		if (!IniFile_WriteString(strSectionName, strKeyName, strTempValue1))
			return FALSE;

		p = va_arg(pArgList, LPCTSTR);
		CString strTempValue2(p);

		if (strTempValue2 == "ListEnd" || strTempValue2 == _T(""))
			return FALSE;
		strSectionName = strTempValue1;
		strKeyName = strTempValue2;
	} while (1);
	va_end(pArgList); /* Reset variable arguments.      */
	if (!IniFile_WriteString(strSectionName, strKeyName, lpValue))
		return FALSE;
	return TRUE;
}

BOOL CIniFile::IniFile_DeleteString(CString strSectionName, CString strKeyName)
{
	return WritePrivateProfileString(strSectionName, strKeyName, NULL, m_strFileFullName);
}

BOOL CIniFile::IniFile_DeleteSection(CString strSectionName)
{
	return WritePrivateProfileString(strSectionName, NULL, NULL, m_strFileFullName);
	// return WritePrivateProfileSection(strSectionName,NULL,m_strFileFullName);
	//这种方法会使 section 的标题一并删除
	// return WritePrivateProfileSection(strSectionName,"",m_strFileFullName);
	//这种方法会保留 section 的标题
}

BOOL CIniFile::IniFile_WriteSection(LPCTSTR lpSectionName, LPCTSTR lpString)
{
	return WritePrivateProfileSection(lpSectionName, lpString, m_strFileFullName);
}

BOOL CIniFile::IniFile_WriteStruct(LPCTSTR lpSectionName, LPCTSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct)
{
	return WritePrivateProfileStruct(lpSectionName, lpszKey, lpStruct, uSizeStruct, m_strFileFullName);
}

BOOL CIniFile::IniFile_DeleteStruct(LPCTSTR lpSectionName, LPCTSTR lpszKey)
{
	return WritePrivateProfileStruct(lpSectionName, lpszKey, NULL, 1, m_strFileFullName);
}

BOOL CIniFile::SetFileName(LPCTSTR lpFileName)
{
	if (!lpFileName)
		return FALSE;

	CString strFileName(lpFileName);
	CString strPathName;

	strFileName.TrimLeft();
	strFileName.TrimRight();

	if (strFileName.Find('\\') == -1)
	{
		TCHAR exeFullPath[MAX_PATH];

		int len2 = GetModuleFileName(NULL, exeFullPath, MAX_PATH);
		CString strDir(exeFullPath);

		strDir.TrimLeft();
		strDir.TrimRight();
		int index = strDir.ReverseFind('\\');
		strPathName = strDir.Left(index + 1) + strFileName;
	}
	else
		strPathName = strFileName;
	CString str;

	if (strPathName.GetLength() > 4)
	{
		str = strPathName.Right(4);
		str.MakeLower();

		if (str == _T(".ini"))
		{
			m_strFileFullName = strPathName;
			return TRUE;
		}
	}
	strPathName += _T(".ini");
	m_strFileFullName = strPathName;
	return TRUE;
}

void CIniFile::IniFile_InitializeForCreate()
{
	IniFile_WriteString(_T("设置"), _T("当前行"), _T("1"));
}
DWORD CIniFile::IniFile_GetSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize)
{
	return GetPrivateProfileSectionNames(lpszReturnBuffer, nSize, m_strFileFullName);
}

DWORD CIniFile::IniFile_GetSectionNames(CStringArray &strArray)
{
	char *sz = new char[nMaxSize_All_SectionNames];
	DWORD dw = IniFile_GetSectionNames(sz, nMaxSize_All_SectionNames);

	strArray.RemoveAll();

	char *index = sz;
	do
	{
		CString str(index);
		if (str.GetLength() < 1)
		{
			delete[] sz;
			return dw;
		}
		strArray.Add(str);
		index = index + str.GetLength() + 1;
	} while (index && (index < sz + nMaxSize_All_SectionNames));
	delete[] sz;
	return dw;
}

BOOL CIniFile::IniFile_GetStruct(LPCTSTR lpSectionName, LPCTSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct)
{
	return GetPrivateProfileStruct(lpSectionName, lpszKey, lpStruct, uSizeStruct, m_strFileFullName);
}

DWORD CIniFile::IniFile_GetSection(LPCTSTR lpSectionName, LPTSTR lpReturnedString, DWORD nSize)
{
	return GetPrivateProfileSection(lpSectionName, lpReturnedString, nSize, m_strFileFullName);
}

DWORD CIniFile::IniFile_GetKeyNamesValues(CString strSectionName, CStringArray &strArray)
{
	char *sz = new char[nMaxSize_A_Section];
	DWORD dw = IniFile_GetSection(strSectionName, sz, nMaxSize_A_Section);
	char *index = sz;
	CString strName, strValue;
	int nPosition = -1;

	while (index && (index < sz + dw))
	{
		CString str(index);
		if (str.GetLength() < 1)
		{
			delete[] sz;
			return dw;
		}
		if ((nPosition = str.Find(_T('='))) == -1)
		{
			IniFile_DeleteString(strSectionName, str);
		}
		else
		{
			strName = str.Left(nPosition);
			strValue = str.Mid(nPosition + 1);
			strArray.Add(strName);
			strArray.Add(strValue);
		}
		index = index + str.GetLength() + 1;
	}
	delete[] sz;
	return dw;
}

void CIniFile::SetMaxSize_A_Section(UINT nSize)
{
	nMaxSize_A_Section = nSize;
}

void CIniFile::SetMaxSize_All_SectionNames(UINT nSize)
{
	nMaxSize_All_SectionNames = nSize;
}

UINT CIniFile::GetMaxSize_All_SectionNames()
{
	return nMaxSize_All_SectionNames;
}

UINT CIniFile::GetMaxSize_A_Section()
{
	return nMaxSize_A_Section;
}

DWORD CIniFile::IniFile_GetKeyNames(CString strSectionName, CStringArray &strArray)
{
	char *sz = new char[nMaxSize_A_Section];
	DWORD dw = IniFile_GetSection(strSectionName, sz, nMaxSize_A_Section);
	char *index = sz;
	CString strName, strValue;
	int nPosition = -1;
	while (index && (index < sz + dw))
	{
		CString str(index);

		if (str.GetLength() < 1)
		{
			delete[] sz;
			return dw;
		}
		if ((nPosition = str.Find(_T('='))) == -1)
			IniFile_DeleteString(strSectionName, str);
		else
			strName = str.Left(nPosition);
		strArray.Add(strName);
		index = index + str.GetLength() + 1;
	}
	delete[] sz;
	return dw;
}

CString CIniFile::GetClassVersion()
{
	return "0.0.1.1";
}
