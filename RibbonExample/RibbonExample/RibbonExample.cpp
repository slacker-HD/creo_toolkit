// RibbonExample.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RibbonExample.h"

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

// CRibbonExampleApp

BEGIN_MESSAGE_MAP(CRibbonExampleApp, CWinApp)
END_MESSAGE_MAP()


// CRibbonExampleApp 构造

CRibbonExampleApp::CRibbonExampleApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRibbonExampleApp 对象

CRibbonExampleApp theApp;


// CRibbonExampleApp 初始化

BOOL CRibbonExampleApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CString _lastRibbonTab;
ProMdl CurrentMdl()
{
	ProMdl mdl;
	ProError status;
	status = ProMdlCurrentGet (&mdl);
	return mdl;
}

int CurrentMdlType()
{
	ProMdl mdl;
	ProMdlType mdltype;
	ProError status;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	status = ProMdlTypeGet(mdl, &mdltype);
	if (status != PRO_TK_NO_ERROR)
		return -1;
	else
		return mdltype;
}

static uiCmdAccessState AccessDrawing(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_DRAWING)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
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

void WrongAct()
{
	ProError status;
	status = ShowDialog(L"注意Ribbon界面要切换到视图栏了");
	status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ; ~Command `ProCmdWinActivate`;");
}

void RightAct()
{
	ProError status;
	CString macro;
	status = ShowDialog(L"同样的功能禁止了Ribbon界面的切换");
	macro = "~ Command `ProCmdDwgRegenModel` ; ~Command `ProCmdWinActivate`;";
	if(_lastRibbonTab != "")
	{
		macro += _T("~ Activate `main_dlg_cur` `" + _lastRibbonTab + "_control_btn` 1;");
	}
	wchar_t *p = macro.AllocSysString();
	status = ProMacroLoad(p);
	SysFreeString(p);
}

void LoadRibbonAct()
{
	ProError status;
	status = ProRibbonDefinitionfileLoad(L"RibbonExample.rbn");
}

ProError ProRibbonTabSwitchNotification(char* from_tab, char* to_tab)
{
	_lastRibbonTab = CString(to_tab);
	return PRO_TK_NO_ERROR;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId WrongActID, RightActID, LoadRibbonItemID;

	status = ProMenubarMenuAdd("RibbonExample", "RibbonExample", "Help", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("RibbonExample", "MainMenu", "MainMenu", NULL, PRO_B_TRUE, MSGFILE);
	
	status = ProCmdActionAdd("WrongMainMenu_Act", (uiCmdCmdActFn)WrongAct, uiProeImmediate, AccessDrawing, PRO_B_TRUE, PRO_B_TRUE, &WrongActID);
	status = ProMenubarmenuPushbuttonAdd("MainMenu", "WrongMenuItem", "WrongMenuItem", "WrongMenuItemtips", NULL, PRO_B_TRUE, WrongActID, MSGFILE);
	
	status = ProCmdActionAdd("RightMainMenu_Act", (uiCmdCmdActFn)RightAct, uiProeImmediate, AccessDrawing, PRO_B_TRUE, PRO_B_TRUE, &RightActID);
	status = ProMenubarmenuPushbuttonAdd("MainMenu", "RightMenuItem", "RightMenuItem", "RightMenuItemtips", NULL, PRO_B_TRUE, RightActID, MSGFILE);
	
	status = ProCmdActionAdd("LoadRibbonItem_Act", (uiCmdCmdActFn)LoadRibbonAct, uiProeImmediate, AccessDrawing, PRO_B_TRUE, PRO_B_TRUE, &LoadRibbonItemID);
	status = ProMenubarmenuPushbuttonAdd("MainMenu", "LoadRibbonItem", "LoadRibbonItem", "LoadRibbonItemtips", NULL, PRO_B_TRUE, LoadRibbonItemID, MSGFILE);
	
	status = ProNotificationSet(PRO_RIBBON_TAB_SWITCH,  (ProFunction)ProRibbonTabSwitchNotification);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	ProError status;
	status = ProNotificationUnset(PRO_RIBBON_TAB_SWITCH);
}