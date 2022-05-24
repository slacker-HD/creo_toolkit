// TrailTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TrailTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// CTrailTestApp

BEGIN_MESSAGE_MAP(CTrailTestApp, CWinApp)
END_MESSAGE_MAP()

// CTrailTestApp 构造

CTrailTestApp::CTrailTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTrailTestApp 对象

CTrailTestApp theApp;

// CTrailTestApp 初始化

BOOL CTrailTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static int _count = 1;

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void AddTrail()
{
	ProError status;
	CString c;
	c.Format("点击了测试菜单%d次\n", _count);
	wchar_t *str = c.AllocSysString();
	status = ProTrailfileCommentWrite(str);
	_count++;
	SysFreeString(str);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId AddTrailID;

	status = ProMenubarMenuAdd("TrailTest", "TrailTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("TrailTest", "TrailTest", "TrailTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("AddTrailMenu_Act", (uiCmdCmdActFn)AddTrail, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AddTrailID);
	status = ProMenubarmenuPushbuttonAdd("TrailTest", "AddTrail", "AddTrail", "AddTrailTips", NULL, PRO_B_TRUE, AddTrailID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}