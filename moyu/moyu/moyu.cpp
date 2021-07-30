// moyu.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "moyu.h"

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

// CmoyuApp

BEGIN_MESSAGE_MAP(CmoyuApp, CWinApp)
END_MESSAGE_MAP()

// CmoyuApp 构造

CmoyuApp::CmoyuApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CmoyuApp 对象

CmoyuApp theApp;

// CmoyuApp 初始化

BOOL CmoyuApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int _linenum = 1;
bool _showOrhide = false;
int _maxlinecount = 0;

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void PrevLine()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	CString LineStr;
	char *p;
	if(_linenum > 1)
	{
		_linenum--;
		LineStr.Format(_T("IMI%d"),_linenum);
		p = LineStr.GetBuffer();
		status = ProMessageDisplay(NOVELFILE, p);
		LineStr.ReleaseBuffer();

	}
}

void NextLine()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	CString LineStr;
	char *p;
	if(_showOrhide == true)
	{
		if(_linenum < _maxlinecount+1)
		{
			_linenum++;
			LineStr.Format(_T("IMI%d"),_linenum);
			p = LineStr.GetBuffer();
			status = ProMessageDisplay(NOVELFILE, p);
			LineStr.ReleaseBuffer();
		}
	}
}

void ClearLine()
{
	ProMessageClear();
}

void ShowOrHide()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	CString LineStr;
	char *p;
	if(_showOrhide == true)
	{
		ProMessageClear();
	}
	else
	{
		LineStr.Format(_T("IMI%d"),_linenum);
		p = LineStr.GetBuffer();
		status = ProMessageDisplay(NOVELFILE, p);
		LineStr.ReleaseBuffer();
	}
	_showOrhide = !_showOrhide;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId PrevLineID, NextLineID,ShowOrHideLineID,JumpLineID;

	status = ProMenubarMenuAdd("moyu", "moyu", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("moyu", "moyu", "moyu", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("PrevLine_Act", (uiCmdCmdActFn)PrevLine, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &PrevLineID);
	status = ProMenubarmenuPushbuttonAdd("moyu", "Prevline", "Prevline", "Prevlinetips", NULL, PRO_B_TRUE, PrevLineID, MSGFILE);

	status = ProCmdActionAdd("NextLine_Act", (uiCmdCmdActFn)NextLine, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &NextLineID);
	status = ProMenubarmenuPushbuttonAdd("moyu", "Nextline", "Nextline", "Nextlinetips", NULL, PRO_B_TRUE, NextLineID, MSGFILE);

	status = ProCmdActionAdd("ShowOrHideLineID_Act", (uiCmdCmdActFn)ShowOrHide, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ShowOrHideLineID);
	status = ProMenubarmenuPushbuttonAdd("moyu", "ShowOrHideLine", "ShowOrHideLine", "ShowOrHideLinetips", NULL, PRO_B_TRUE, ShowOrHideLineID, MSGFILE);

	status = ProCmdActionAdd("JumpLineID_Act", (uiCmdCmdActFn)ClearLine, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &JumpLineID);
	status = ProMenubarmenuPushbuttonAdd("moyu", "JumpLine", "JumpLine", "JumpLinetips", NULL, PRO_B_TRUE, JumpLineID, MSGFILE);

	ProLine buffer;
	status = ProMessageToBuffer(buffer, NOVELFILE, (char*)"IMILINELENGTH");
	_maxlinecount = atoi(CString(buffer));

	_showOrhide = false;
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}