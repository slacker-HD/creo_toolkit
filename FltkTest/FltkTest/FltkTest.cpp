// FltkTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FltkTest.h"

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

// CFltkTestApp

BEGIN_MESSAGE_MAP(CFltkTestApp, CWinApp)
END_MESSAGE_MAP()

// CFltkTestApp 构造

CFltkTestApp::CFltkTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CFltkTestApp 对象

CFltkTestApp theApp;

// CFltkTestApp 初始化

BOOL CFltkTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void ShowDialog()
{
	// 这个例子中main.c采用gb2312编码，所以无法使用中文
	// 实际将界面代码单独提出来用UTF-8编码保存就可以显示中文
	Fl_Double_Window w(100, 200, 460, 320, "Fltk layout");
	w.size_range(w.w(), w.h(), 0, 0);

	Fl_Group group1(10, 10, w.w() - 20, 30);
	Fl_Input input1(80, 10, w.w() - 205, 30, "Name:");
	Fl_Button *b1 = new Fl_Button(w.w() - 110, 10, 100, 30, "Check!");
	group1.end();
	group1.resizable(input1);

	Fl_Group group2(10, 50, w.w() - 20, 30);
	Fl_Input input2(80, 50, w.w() - 205, 30, "Mail:");
	Fl_Button b2(w.w() - 110, 50, 100, 30, "Check!");
	group2.end();
	group2.resizable(input2);

	Fl_Multiline_Input comments(80, 100, w.w() - 90, w.h() - 150, "Comments:");
	Fl_Group group3(10, w.h() - 10 - 30, w.w() - 20, 30);
	Fl_Box b(10, w.h() - 10 - 30, group3.w() - 100, 30); // Fl_Box是默认不可见的
	Fl_Button b3(w.w() - 10 - 100, w.h() - 10 - 30, 100, 30, "OK");
	group3.end();
	group3.resizable(b);

	w.resizable(comments);
	w.show();
	Fl::run();
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FltkTestID;

	Fl::scheme("plastic");

	status = ProMenubarMenuAdd("FltkTest", "FltkTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FltkTest", "FltkTest", "FltkTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FltkTest_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &FltkTestID);
	status = ProMenubarmenuPushbuttonAdd("FltkTest", "FltkTestmenu", "FltkTestmenu", "FltkTestmenutips", NULL, PRO_B_TRUE, FltkTestID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
