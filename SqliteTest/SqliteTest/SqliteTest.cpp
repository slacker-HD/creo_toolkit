// SqliteTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SqliteTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CSqliteTestApp

BEGIN_MESSAGE_MAP(CSqliteTestApp, CWinApp)
END_MESSAGE_MAP()

// CSqliteTestApp 构造

CSqliteTestApp::CSqliteTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CSqliteTestApp 对象

CSqliteTestApp theApp;

// CSqliteTestApp 初始化

BOOL CSqliteTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

int Utf82Unicode(const char *utf, wchar_t *unicode, int nBuffSize)
{
	if (!utf || !strlen(utf))
	{
		return 0;
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, utf, -1, NULL, 0);
	size_t num = dwUnicodeLen * sizeof(wchar_t);
	if (num > nBuffSize)
	{
		return 0;
	}
	MultiByteToWideChar(CP_UTF8, 0, utf, -1, unicode, dwUnicodeLen);
	return dwUnicodeLen;
}

int callback(void *, int nCount, char **pValue, char **pName)
{
	CString str = _T("");
	for (int i = 0; i < nCount; i++)
	{
		wchar_t t[100];
		int a = Utf82Unicode(pValue[i], t, 100);
		str += CString(pName[i]) + _T("   ") + CString(t);
		str += _T("\n");
	}
	wchar_t *msg = str.AllocSysString();
	ShowDialog(msg);
	SysFreeString(msg);
	return 0;
}

extern "C" int user_initialize()
{
	ProError status;
	sqlite3 *db;
	char *sql = "SELECT * FROM TEST";
	char *zErrMsg = 0;

	status = ShowDialog(L"程序已启动,准备打开数据库。");

	int res = sqlite3_open("D:\\mydoc\\creo_toolkit\\SqliteTest\\test.db", &db);
	if (res != SQLITE_OK)
	{
		status = ShowDialog(L"数据库打开失败。");
		return PRO_TK_NO_ERROR;
	}

	res = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
	sqlite3_free(zErrMsg);
	sqlite3_close(db);
	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	ProError status;
	status = ShowDialog(L"程序已退出");
}