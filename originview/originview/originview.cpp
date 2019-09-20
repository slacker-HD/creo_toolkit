// originview.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "originview.h"
#include "MyEbdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CoriginviewApp

BEGIN_MESSAGE_MAP(CoriginviewApp, CWinApp)
END_MESSAGE_MAP()

// CoriginviewApp 构造

CoriginviewApp::CoriginviewApp()
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CoriginviewApp 对象

CoriginviewApp theApp;

// CoriginviewApp 初始化

BOOL CoriginviewApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

CMyEbdDialog *dlg = NULL;
HWND hwndChild = NULL;
HWND hwndParent = NULL;
static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void ShowMFCChild()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString str = _T("this is a test dialog");
    hwndParent = ::FindWindow(NULL, str);
    dlg = new CMyEbdDialog();
    dlg->Create(IDD_EBDDIALOG);
    dlg->ShowWindow(SW_SHOW);

    str = _T("IMImfcchilddialog");
    hwndChild = ::FindWindow(NULL, str);
    LONG style = GetWindowLong(hwndChild, GWL_STYLE);
    style &= ~WS_CAPTION;
    SetWindowLong(hwndChild, GWL_STYLE, style);
    ::SetParent(hwndChild, hwndParent);
    ::SetWindowPos(hwndChild, HWND_TOP, 14, 39, 400, 400, SWP_SHOWWINDOW | SWP_HIDEWINDOW);
    ::ShowWindow(hwndChild, SW_SHOW);
}

void CommitExit()
{
    ProError status;
    status = ProUIDialogExit("originview", 0);
    if (dlg != NULL)
    {
        dlg->EndDialog(0);
        delete dlg;
        dlg = NULL;
    }
}

void CommitSet()
{
    ProError status;
	wchar_t *value;
	status = ProUIInputpanelValueGet("originview", "InputString", &value);
	if(dlg != NULL)
	{
		dlg->SetEdit(CString(value));
	}
	status = ProWstringFree(value);
}

void CalendarSet(CString Value)
{
    ProError status;
	wchar_t *value = Value.AllocSysString();
	status = ProUIInputpanelValueSet("originview", "InputCalendar", value);
	SysFreeString(value);
}

void ShowOrigdlg()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    ProError status;
    status = ProUIDialogCreate("originview", "originview");
    status = ProUIDialogPostmanagenotifyActionSet("originview", (ProUIAction)ShowMFCChild, NULL);
    status = ProUIPushbuttonActivateActionSet("originview", "Exit", ProUIAction(CommitExit), NULL);
    status = ProUIPushbuttonActivateActionSet("originview", "Set", ProUIAction(CommitSet), NULL);
    status = ProUIDialogActivate("originview", NULL);
    status = ProUIDialogDestroy("originview");
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
    uiCmdCmdId originviewID;

    status = ProMenubarMenuAdd("originview", "originview", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("originview", "originview", "originview", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("originview_Act", (uiCmdCmdActFn)ShowOrigdlg, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &originviewID);
    status = ProMenubarmenuPushbuttonAdd("originview", "originviewdlg", "originviewdlg", "originviewdlgtips", NULL, PRO_B_TRUE, originviewID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
    if (dlg != NULL)
        delete (dlg);
}