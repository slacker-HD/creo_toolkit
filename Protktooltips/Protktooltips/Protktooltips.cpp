// Protktooltips.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Protktooltips.h"
#include "TestDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK hHook = NULL;
CTestDialog *dlg = NULL;

LRESULT CALLBACK __stdcall GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LPMSG lpMsg = (LPMSG)lParam;

	if (lpMsg->message == WM_MOUSEMOVE || lpMsg->message == WM_LBUTTONDOWN || lpMsg->message == WM_LBUTTONUP)
	{
		if (dlg != NULL)
			dlg->m_ToolTipCtrl.RelayEvent(lpMsg);
	}

	return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}

#pragma region MFC
BEGIN_MESSAGE_MAP(CProtktooltipsApp, CWinApp)
END_MESSAGE_MAP()
CProtktooltipsApp::CProtktooltipsApp()
{
}
CProtktooltipsApp theApp;
BOOL CProtktooltipsApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
#pragma endregion

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}

void ShowWin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxEnableControlContainer();
	if (dlg != NULL)
		delete (dlg);
	dlg = new CTestDialog();
	dlg->Create(IDD_DIALOGTESTTOOLTIPS);
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
	dlg = NULL;
	hHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, NULL, GetCurrentThreadId());

	ProError status;
	uiCmdCmdId cmd_id;
	status = ProMenubarMenuAdd("tooltips", "tooltips", "Help", PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("Act1", (uiCmdCmdActFn)ShowWin, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &cmd_id);
	status = ProMenubarmenuPushbuttonAdd("tooltips", "Cmd", "Cmd", "Tips", NULL, PRO_B_TRUE, cmd_id, MSGFILE);

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (dlg != NULL)
		delete (dlg);
	UnhookWindowsHookEx(hHook);
}