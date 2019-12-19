// ToolkitCsharp.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ToolkitCsharp.h"

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

// CToolkitCsharpApp

BEGIN_MESSAGE_MAP(CToolkitCsharpApp, CWinApp)
END_MESSAGE_MAP()

// CToolkitCsharpApp 构造

CToolkitCsharpApp::CToolkitCsharpApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CToolkitCsharpApp 对象

CToolkitCsharpApp theApp;

// CToolkitCsharpApp 初始化

BOOL CToolkitCsharpApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

extern "C"  void SaveFile()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	status = ProMacroLoad(L"~ Command `ProCmdModelSave` ;~ Activate `file_saveas` `OK`;");
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}

DWORD CallCSharpFun(LPCWSTR DotNetVer, LPCWSTR DLLfile, LPCWSTR NameSpace, LPCWSTR FunctionName, LPCWSTR Param)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT hr;
	ICLRMetaHost *pMetaHost = NULL;
	ICLRRuntimeInfo *pRuntimeInfo = NULL;
	ICLRRuntimeHost *pClrRuntimeHost = NULL;
	DWORD pReturnValue;

	hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
	hr = pMetaHost->GetRuntime(DotNetVer, IID_PPV_ARGS(&pRuntimeInfo));
	hr = pRuntimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&pClrRuntimeHost));
	hr = pClrRuntimeHost->Start();
	hr = pClrRuntimeHost->ExecuteInDefaultAppDomain(DLLfile, NameSpace, FunctionName, Param, &pReturnValue);
	pMetaHost->Release();
	pRuntimeInfo->Release();
	pClrRuntimeHost->Release();
	return pReturnValue;
}

void ShowCSharpDlg()
{
	LPCWSTR DotNetVer, DLLfile, NameSpace, FunctionName, Param;
	DotNetVer = L"v4.0.30319";
	DLLfile = L"D:\\mydoc\\creo_toolkit\\ToolkitCsharp\\x64\\Debug\\CsharpDll.dll";
	NameSpace = L"CsharpDll.MyDialog";
	FunctionName = L"ShowWindow";
	Param = L"只能传入1个字符串，如果复杂可以考虑用txt文件路径传入";
	CallCSharpFun(DotNetVer, DLLfile, NameSpace, FunctionName, Param);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ToolkitCsharpID;

	status = ProMenubarMenuAdd("ToolkitCsharp", "ToolkitCsharp", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("ToolkitCsharp", "ToolkitCsharp", "ToolkitCsharp", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ToolkitCsharp_Act", (uiCmdCmdActFn)ShowCSharpDlg, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ToolkitCsharpID);
	status = ProMenubarmenuPushbuttonAdd("ToolkitCsharp", "ToolkitCsharpmenu", "ToolkitCsharpmenu", "ToolkitCsharpmenutips", NULL, PRO_B_TRUE, ToolkitCsharpID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
