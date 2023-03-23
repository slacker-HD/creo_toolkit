// FltkTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FltkTest.h"

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
	Fl_Window  *window;
	Fl_Box  *box;
	//本文件的编码是GB2312,所以无法显示中文
	//应该将Fltk的代码单独提出来用UTF-8编码保存，这样就支持中文了
	window = new  Fl_Window(450, 330, "First App!");
	box = new Fl_Box(20, 40, 360, 200, "Hello World");
	box->box(FL_FLAT_BOX);
	box->labelsize(36);                   //设置字体大小
	box->labelfont(FL_BOLD + FL_ITALIC);  //设置字体
	box->labeltype(FL_SHADOW_LABEL);      //设置label的类型
	window->end();
	window->show();
}



extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FltkTestID;

	status = ProMenubarMenuAdd("FltkTest", "FltkTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FltkTest", "FltkTest", "FltkTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FltkTest_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &FltkTestID);
	status = ProMenubarmenuPushbuttonAdd("FltkTest", "FltkTestmenu", "FltkTestmenu", "FltkTestmenutips", NULL, PRO_B_TRUE, FltkTestID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
