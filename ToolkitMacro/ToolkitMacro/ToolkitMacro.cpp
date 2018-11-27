// ToolkitMacro.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ToolkitMacro.h"

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

// CToolkitMacroApp

BEGIN_MESSAGE_MAP(CToolkitMacroApp, CWinApp)
END_MESSAGE_MAP()

// CToolkitMacroApp 构造

CToolkitMacroApp::CToolkitMacroApp()
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CToolkitMacroApp 对象

CToolkitMacroApp theApp;

// CToolkitMacroApp 初始化

BOOL CToolkitMacroApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

typedef enum _hint
{
    Fun = 0,
    About = 1,
} HINT;
HINT hint;

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


static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    //if (CurrentMdlType() == PRO_PART)
	return ACCESS_AVAILABLE;
	//else
	//	return ACCESS_INVISIBLE;
}

ProError ShowDialog(wchar_t *Message)
{
    ProUIMessageButton *buttons;
    ProUIMessageButton user_choice;
    ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
    buttons[0] = PRO_UI_MESSAGE_OK;
    ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, _T("提示"), Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
    ProArrayFree((ProArray *)&buttons);
    return PRO_TK_NO_ERROR;
}

void FunAfterMacro()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    AfxMessageBox(_T("运行完宏后跑mfc的代码！"));
    AfxMessageBox(_T("再来一次！"));
    AfxMessageBox(_T("再来第二次！"));
}
void about()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (hint == Fun)
    {
       FunAfterMacro();
    }
    else
    {
        ShowDialog(_T("如何在运行完宏后再跑mfc代码。\n访问我的博客获得更多信息：\nhttp://www.hudi.site"));
    }
    hint = About;
}

void runMacro()
{
    ProError status;
    CString Macro = _T("~ Close `main_dlg_cur` `appl_casc`;~ Command `ProCmdModelErase` ; ~ Activate `0_std_confirm` `OK`;");
    Macro += _T("~ Command `About_Act`;");
    wchar_t *macro = Macro.AllocSysString();
    hint = Fun;
    status = ProMacroLoad(macro);
    //Note that this function is not supported for the following situations and tasks
    //1.Activating windows or setting the current model
    //2.Erasing the current model
    //3.Completing dialog commands ending with an "OK" button press. It will cancel some dialogs after showing them.
    //4.Executing macros during a trail file replay.
    //status = ProMacroExecute();//这个函数肯定有限制，上面的宏有是不能立即执行的。
    SysFreeString(macro);
}

//错误的示例
void RunmacroW()
{
    ProError status;
    CString Macro = _T("~ Close `main_dlg_cur` `appl_casc`;~ Command `ProCmdModelErase` ; ~ Activate `0_std_confirm` `OK`;");
    wchar_t *macro = Macro.AllocSysString();
    hint = Fun;
    status = ProMacroLoad(macro);
    //Note that this function is not supported for the following situations and tasks
    //1.Activating windows or setting the current model
    //2.Erasing the current model
    //3.Completing dialog commands ending with an "OK" button press. It will cancel some dialogs after showing them.
    //4.Executing macros during a trail file replay.
    status = ProMacroExecute();//这个函数肯定有限制，上面的宏是不能立即执行的。
    SysFreeString(macro);
	FunAfterMacro();
}

extern "C" int user_initialize()
{
    //MFC初始化
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    ProError status;
    uiCmdCmdId RunmacroId, RunmacroWId, AboutId;

    status = ProMenubarMenuAdd("Toolkitmacro", "Toolkitmacro", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("Toolkitmacro", "Toolkitmacro", "OneKey", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("Runmacro_Act", (uiCmdCmdActFn)runMacro, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &RunmacroId);
    status = ProMenubarmenuPushbuttonAdd("Toolkitmacro", "Runmacro", "Runmacro", "Runmacrotips", NULL, PRO_B_TRUE, RunmacroId, MSGFILE);

	status = ProCmdActionAdd("RunmacroW_Act", (uiCmdCmdActFn)RunmacroW, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &RunmacroWId);
    status = ProMenubarmenuPushbuttonAdd("Toolkitmacro", "RunmacroW", "RunmacroW", "RunmacroWtips", NULL, PRO_B_TRUE, RunmacroWId, MSGFILE);


    status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
    status = ProMenubarmenuPushbuttonAdd("Toolkitmacro", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

    hint = About;
    return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
}