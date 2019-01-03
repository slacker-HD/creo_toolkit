// ProTKDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ProTKDll.h"
/******************************************
在dll工程里面复制,注意include的目录关系，
IDD_IMIDIALOGTEST = 16000，见ImiProTKDLl工程的res文件,这个工程没有定义，故重定义下
******************************************/
#define IDD_IMIDIALOGTEST 16000
#include "..\ImiProTKDLl\ImiDialogTest.h"

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

// CProTKDllApp

BEGIN_MESSAGE_MAP(CProTKDllApp, CWinApp)
END_MESSAGE_MAP()

// CProTKDllApp 构造

CProTKDllApp::CProTKDllApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CProTKDllApp 对象

CProTKDllApp theApp;

// CProTKDllApp 初始化

BOOL CProTKDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}
void Test()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProPath exec_path;
	ProToolkitApplExecPathGet(exec_path);

	HINSTANCE m_hDll = LoadLibrary(CString(exec_path).Left((CString(exec_path)).GetLength() - 12) + _T("ImiProTKDLl.dll"));
	if (NULL == m_hDll)
		AfxMessageBox(_T("加载 ImiProTKDLl.dll 失败"));
	typedef ImiDialogTest *(*hpDllFun)();
	hpDllFun pShowDlg = (hpDllFun)GetProcAddress(m_hDll, "ShowDialog");
	if (NULL == pShowDlg)
	{
		AfxMessageBox(_T("DLL中函数寻找失败"));
		return;
	}
	//指针，内存泄漏,做例子不回收了
	ImiDialogTest *imiDialogTest = pShowDlg();
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
	uiCmdCmdId Test_cmd_id1;
	status = ProMenubarMenuAdd("Test", "Test", "File", PRO_B_TRUE, L"Test.txt");

	status = ProCmdActionAdd("Test_Act1", (uiCmdCmdActFn)Test, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &Test_cmd_id1);
	status = ProMenubarmenuPushbuttonAdd("Test", "Cmd1", "Cmd1", "Tips1", NULL, PRO_B_TRUE, Test_cmd_id1, L"Test.txt");
	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
