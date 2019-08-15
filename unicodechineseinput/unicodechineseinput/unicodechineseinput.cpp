// unicodechineseinput.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "unicodechineseinput.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region MFC初始化
// CunicodechineseinputApp

BEGIN_MESSAGE_MAP(CunicodechineseinputApp, CWinApp)
END_MESSAGE_MAP()

// CunicodechineseinputApp 构造

CunicodechineseinputApp::CunicodechineseinputApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CunicodechineseinputApp 对象

CunicodechineseinputApp theApp;

// CunicodechineseinputApp 初始化

BOOL CunicodechineseinputApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

CMyDialog *Dialog;

void showDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Dialog = new CMyDialog();
	Dialog->Create(IDD_DIALOG);
	Dialog->ShowWindow(SW_SHOWNOACTIVATE);
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}


extern "C" int user_initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	uiCmdCmdId showModelessDialog;
	status = ProMenubarMenuAdd("unicodechineseinput", "unicodechineseinput", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("unicodechineseinput", "unicodechineseinput", "showdialog", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdActionAdd("Runmacro_Act", (uiCmdCmdActFn)showDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &showModelessDialog);
	status = ProMenubarmenuPushbuttonAdd("unicodechineseinput", "showdialog", "showdialog", "showdialogtips", NULL, PRO_B_TRUE, showModelessDialog, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
