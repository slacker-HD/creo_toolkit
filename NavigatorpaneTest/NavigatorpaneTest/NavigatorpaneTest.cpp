// NavigatorpaneTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "NavigatorpaneTest.h"

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

// CNavigatorpaneTestApp

BEGIN_MESSAGE_MAP(CNavigatorpaneTestApp, CWinApp)
END_MESSAGE_MAP()

// CNavigatorpaneTestApp 构造

CNavigatorpaneTestApp::CNavigatorpaneTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CNavigatorpaneTestApp 对象

CNavigatorpaneTestApp theApp;

// CNavigatorpaneTestApp 初始化

BOOL CNavigatorpaneTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CString CurrentMdlName()
{
	ProError status;
	ProMdl mdl;
	ProName MdlName;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return _T("获取当前窗口打开的模型名称出错。");
	else
	{
		status = ProMdlNameGet(mdl, MdlName);
		return CString(MdlName);
	}
}

CString CurrentMdlext()
{
	ProError status;
	ProMdl mdl;
	ProName MdlName;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return _T("获取当前窗口打开的模型名称出错。");
	else
	{
		status = ProMdlNameGet(mdl, MdlName);
		return CString(MdlName);
	}
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void AddUrlPane()
{
	ProError status;
	status = ProNavigatorpaneBrowserAdd("Weblink Apps", NULL, L"www.hudi.site");
	status = ProNavigatorpaneBrowserURLSet("Weblink Apps", L"weblink.hudi.site");
	// 替换自己的目录
	status = ProNavigatorpaneBrowsericonSet("Weblink Apps", "D:\\mydoc\\creo_toolkit\\NavigatorpaneTest\\text\\favicon.PNG");
}

void GetName()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxMessageBox(CurrentMdlName());
}

void ShowDialog()
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", L"自定义对话框", buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
}

ProError PaneActionInit()
{
	ProError status;
	int WinID;
	char *ComponentName, *DeviceName;
	status = ProWindowCurrentGet(&WinID);
	status = ProNavigatorpanePHolderDevicenameGet(WinID, &DeviceName);
	status = ProNavigatorpanePHolderComponentnameGet("MyApps1", "BtnName", &ComponentName);
	status = ProUIPushbuttonActivateActionSet(DeviceName, ComponentName, (ProUIAction)(GetName), NULL);
	status = ProStringFree(ComponentName);

	status = ProNavigatorpanePHolderComponentnameGet("MyApps1", "BtnDialog", &ComponentName);
	status = ProUIPushbuttonActivateActionSet(DeviceName, ComponentName, (ProUIAction)(ShowDialog), NULL);
	status = ProStringFree(ComponentName);
	status = ProStringFree(DeviceName);

	return PRO_TK_NO_ERROR;
}

void AddDialog()
{
	ProError status;
	// 替换自己的目录
	status = ProNavigatorpanePHolderAdd("MyApps1", "NavigatorpaneTest.res", "D:\\mydoc\\creo_toolkit\\NavigatorpaneTest\\text\\favicon.PNG");
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProNotificationSet(PRO_WINDOW_OCCUPY_POST, (ProFunction)PaneActionInit);
		PaneActionInit();
	}
}
extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId AddUrlID, AddDialogID;

	status = ProMenubarMenuAdd("NavigatorpaneTest", "NavigatorpaneTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("NavigatorpaneTest", "NavigatorpaneTest", "NavigatorpaneTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("AddUrl_Act", (uiCmdCmdActFn)AddUrlPane, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddUrlID);
	status = ProMenubarmenuPushbuttonAdd("NavigatorpaneTest", "AddUrl", "AddUrl", "AddUrlTips", NULL, PRO_B_TRUE, AddUrlID, MSGFILE);

	status = ProCmdActionAdd("AddDialog_Act", (uiCmdCmdActFn)AddDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddDialogID);
	status = ProMenubarmenuPushbuttonAdd("NavigatorpaneTest", "AddDialog", "AddDialog", "AddDialogTips", NULL, PRO_B_TRUE, AddDialogID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	ProError status;
	status = ProNotificationUnset(PRO_WINDOW_OCCUPY_POST);
}
