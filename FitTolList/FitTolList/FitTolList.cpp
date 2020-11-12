// FitTolList.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FitTolList.h"
#include "FitTol.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma region MFC
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

// CFitTolListApp

BEGIN_MESSAGE_MAP(CFitTolListApp, CWinApp)
END_MESSAGE_MAP()

// CFitTolListApp 构造

CFitTolListApp::CFitTolListApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CFitTolListApp 对象

CFitTolListApp theApp;

// CFitTolListApp 初始化

BOOL CFitTolListApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void FitTolListCmd1()
{
	GToInterCal();
}
void FitTolListCmd2()
{
	SelpartInterference();
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
	uiCmdCmdId FitTolList_cmd_id1, FitTolList_cmd_id2;
	status = ProMenubarMenuAdd("FitTolList", "FitTolList", "File", PRO_B_TRUE, L"FitTolList.txt");

	status = ProCmdActionAdd("IMI_FitTolList_Act1", (uiCmdCmdActFn)FitTolListCmd1, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &FitTolList_cmd_id1);
	status = ProMenubarmenuPushbuttonAdd("FitTolList", "Cmd1", "Cmd1", "Tips1", NULL, PRO_B_TRUE, FitTolList_cmd_id1, L"FitTolList.txt");

	status = ProCmdActionAdd("IMI_FitTolList_Act2", (uiCmdCmdActFn)FitTolListCmd2, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &FitTolList_cmd_id2);
	status = ProMenubarmenuPushbuttonAdd("FitTolList", "Cmd2", "Cmd2", "Tips2", NULL, PRO_B_TRUE, FitTolList_cmd_id2, L"FitTolList.txt");

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
