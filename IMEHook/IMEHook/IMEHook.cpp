// IMEHook.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "IMEHook.h"
#include "DialogInput.h"
#include "DialogMessageLoop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region MFC
//
// TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
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

// CIMEHookApp

BEGIN_MESSAGE_MAP(CIMEHookApp, CWinApp)
END_MESSAGE_MAP()

// CIMEHookApp 构造

CIMEHookApp::CIMEHookApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CIMEHookApp 对象

CIMEHookApp theApp;

// CIMEHookApp 初始化

BOOL CIMEHookApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

HHOOK hHook = NULL;
CDialogInput *dlg = NULL;

LRESULT CALLBACK __stdcall GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LPMSG lpMsg = (LPMSG)lParam;

	if (dlg != NULL)
	{
		if (lpMsg->message == WM_IME_ENDCOMPOSITION)
		{
			CDialogMessageLoop dialog;
			dialog.DoModal();
		}
	}

	return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void ShowWin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxEnableControlContainer();
	if (dlg != NULL)
		delete (dlg);
	dlg = new CDialogInput();
	dlg->Create(IDD_DIALOGINPUT);
	dlg->ShowWindow(SW_SHOW);
}

extern "C" int user_initialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxInitRichEdit2();
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	AfxEnableControlContainer();
	ProError status;
	uiCmdCmdId showModelessDialog;
	status = ProMenubarMenuAdd("unicodechineseinput", "unicodechineseinput", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("unicodechineseinput", "unicodechineseinput", "showdialog", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdActionAdd("ShowDIALOG_Act", (uiCmdCmdActFn)ShowWin, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &showModelessDialog);
	status = ProMenubarmenuPushbuttonAdd("unicodechineseinput", "showdialog", "showdialog", "showdialogtips", NULL, PRO_B_TRUE, showModelessDialog, MSGFILE);

	hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, GetCurrentThreadId());
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (dlg != NULL)
		delete (dlg);
	UnhookWindowsHookEx(hHook);
}
