// RotateView.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RotateView.h"

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

// CRotateViewApp

BEGIN_MESSAGE_MAP(CRotateViewApp, CWinApp)
END_MESSAGE_MAP()

// CRotateViewApp 构造

CRotateViewApp::CRotateViewApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CRotateViewApp 对象

CRotateViewApp theApp;

// CRotateViewApp 初始化

BOOL CRotateViewApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
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

static uiCmdAccessState AccessPRTorASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_PART || CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void _setView(ProRotate rotation_axis, double angle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	status = ProViewRotate(NULL, NULL, rotation_axis, angle);
	status = ProWindowClear(PRO_VALUE_UNUSED);
	status = ProWindowRefresh(PRO_VALUE_UNUSED);
}

void RotateX()
{
	_setView(PRO_X_ROTATION, 90);
}

void RotateY()
{
	_setView(PRO_Y_ROTATION, 90);
}

void RotateZ()
{
	_setView(PRO_Z_ROTATION, 90);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId RotateViewXID, RotateViewYID, RotateViewZID;

	status = ProMenubarMenuAdd("RotateView", "RotateView", "About", PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("RotateViewX_Act", (uiCmdCmdActFn)RotateX, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &RotateViewXID);
	status = ProMenubarmenuPushbuttonAdd("RotateView", "RotateViewX", "RotateViewXmenu", "RotateViewXmenutips", NULL, PRO_B_TRUE, RotateViewXID, MSGFILE);

	status = ProCmdActionAdd("RotateViewY_Act", (uiCmdCmdActFn)RotateY, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &RotateViewYID);
	status = ProMenubarmenuPushbuttonAdd("RotateView", "RotateViewY", "RotateViewYmenu", "RotateViewYmenutips", NULL, PRO_B_TRUE, RotateViewYID, MSGFILE);

	status = ProCmdActionAdd("RotateViewZ_Act", (uiCmdCmdActFn)RotateZ, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &RotateViewZID);
	status = ProMenubarmenuPushbuttonAdd("RotateView", "RotateViewZ", "RotateViewZmenu", "RotateViewZmenutips", NULL, PRO_B_TRUE, RotateViewZID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}