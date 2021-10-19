// CascadingMenuTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CascadingMenuTest.h"

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

// CCascadingMenuTestApp

BEGIN_MESSAGE_MAP(CCascadingMenuTestApp, CWinApp)
END_MESSAGE_MAP()


// CCascadingMenuTestApp 构造

CCascadingMenuTestApp::CCascadingMenuTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCascadingMenuTestApp 对象

CCascadingMenuTestApp theApp;


// CCascadingMenuTestApp 初始化

BOOL CCascadingMenuTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


int _dialogStyle = 1;
wchar_t *_dialogcontent = L"测试内容1";

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

ProError _showDialog(int style, wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProError status;
	if (style == 1)
	{
		status = ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
		buttons[0] = PRO_UI_MESSAGE_YES;
	}
	else
	{
		ProArrayAlloc(2, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
		buttons[0] = PRO_UI_MESSAGE_YES;
		buttons[1] = PRO_UI_MESSAGE_NO;
	}

	status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"提示", Message, buttons, buttons[0], &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

int SetDialogContent(ProAppData app_data, int dialogStyle)
{
	_dialogcontent = (wchar_t *)app_data;
	return 0;
}

int ShowContent(ProAppData app_data, int app_int)
{
	ProError status;
	int ContentMenuID;
	int action;
	wchar_t *Content1 = L"测试内容1";
	wchar_t *Content2 = L"测试内容2";
	status = ProMenuFileRegister("Content", "Content.mnu", &ContentMenuID);
	status = ProMenubuttonActionSet("Content", "Custom String 1", (ProMenubuttonAction)SetDialogContent, ProAppData(Content1), 0);
	status = ProMenubuttonActionSet("Content", "Custom String 2", (ProMenubuttonAction)SetDialogContent, ProAppData(Content2), 0);
	status = ProMenubuttonActionSet("Content", "Done/Return", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	status = ProMenubuttonActionSet("Content", "Content", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	//status = ProMenuPush();
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Content", &ContentMenuID);
	status = ProMenubuttonHighlight("Content", "Custom String 1");
	status = ProMenuProcess("", &action);
	return 0;
}

int SetDialogStyle(ProAppData app_data, int dialogStyle)
{
	_dialogStyle = dialogStyle;
	return 0;
}

int DeleteMenuadnPop(ProAppData app_data, int dialogStyle)
{
	ProError status;
	status = ProMenuDelete();
	status = ProMenuPop();
	return 0;
}

int ShowDialogStyle(ProAppData app_data, int app_int)
{
	ProError status;
	int DialogStyleMenuID;
	int action;
	status = ProMenuFileRegister("Dialog Style", "DialogStyle.mnu", &DialogStyleMenuID);
	status = ProMenubuttonActionSet("Dialog Style", "Single YES Button", (ProMenubuttonAction)SetDialogStyle, NULL, SINGLEYESDIALOG);
	status = ProMenubuttonActionSet("Dialog Style", "YES/NO Button", (ProMenubuttonAction)SetDialogStyle, NULL, YESNODIALOG);
	status = ProMenubuttonActionSet("Dialog Style", "Done/Return", (ProMenubuttonAction)DeleteMenuadnPop, NULL, 0);
	status = ProMenubuttonActionSet("Dialog Style", "Dialog Style", (ProMenubuttonAction)DeleteMenuadnPop, NULL, 0);
	status = ProMenuPush();
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Dialog Style", &DialogStyleMenuID);
	status = ProMenubuttonHighlight("Dialog Style", "Single YES Button");
	status = ProMenuProcess("", &action);
	return 0;
}

int QuitAction(ProAppData app_data, int app_int)
{
	ProError status;
	status = ProMenuDeleteWithStatus(app_int);
	return 0;
}

void ShowCascadingMenu()
{
	ProError status;
	int TestMenuId;
	int action;
	status = ProMenuFileRegister("Show Custom Dialog", "ShowCustomDialog.mnu", &TestMenuId);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Show Custom Dialog", (ProMenubuttonAction)ProMenuDelete, NULL, 0);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Dialog Style", (ProMenubuttonAction)ShowDialogStyle, NULL, 0);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Content", (ProMenubuttonAction)ShowContent, NULL, 1);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Show Dialog", (ProMenubuttonAction)QuitAction, NULL, POPUPMENU_DONE);
	status = ProMenubuttonActionSet("Show Custom Dialog", "Done/Return", (ProMenubuttonAction)QuitAction, NULL, POPUPMENU_QUIT);
	status = ProMenuCreate(PROMENUTYPE_MAIN, "Show Custom Dialog", &TestMenuId);
	status = ProMenuProcess("", &action);
	if (action == POPUPMENU_DONE)
	{
		_showDialog(_dialogStyle, _dialogcontent);
	}
	else
	{
		_showDialog(SINGLEYESDIALOG, L"取消了功能");
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId showPopupMenuID;

	status = ProMenubarMenuAdd("CascadingMenuTest", "CascadingMenuTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CascadingMenuTest", "CascadingMenuTest", "CascadingMenuTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ShowCascadingMenu_Act", (uiCmdCmdActFn)ShowCascadingMenu, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &showPopupMenuID);
	status = ProMenubarmenuPushbuttonAdd("CascadingMenuTest", "ShowCascadingMenu", "ShowCascadingMenu", "ShowCascadingMenuTips", NULL, PRO_B_TRUE, showPopupMenuID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
