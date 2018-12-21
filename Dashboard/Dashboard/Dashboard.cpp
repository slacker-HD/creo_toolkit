// Dashboard.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Dashboard.h"
#include "TestDashBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDashboardApp, CWinApp)
END_MESSAGE_MAP()

CDashboardApp::CDashboardApp()
{
}

CDashboardApp theApp;

BOOL CDashboardApp::InitInstance()
{
    CWinApp::InitInstance();
    return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

extern "C" int user_initialize()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ProError status;
    uiCmdCmdId RunmacroId;

    status = ProMenubarMenuAdd("Dashboard", "Dashboard", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("Dashboard", "Dashboard", "OneKey", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("ShowDashboard_Act", (uiCmdCmdActFn)ShowDashBoard, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &RunmacroId);
    status = ProMenubarmenuPushbuttonAdd("Dashboard", "ShowDashboard", "ShowDashboard", "ShowDashboardtips", NULL, PRO_B_TRUE, RunmacroId, MSGFILE);

    return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
}