// CurrentFolder.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CurrentFolder.h"

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

// CCurrentFolderApp

BEGIN_MESSAGE_MAP(CCurrentFolderApp, CWinApp)
END_MESSAGE_MAP()

// CCurrentFolderApp 构造

CCurrentFolderApp::CCurrentFolderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CCurrentFolderApp 对象

CCurrentFolderApp theApp;

// CCurrentFolderApp 初始化

BOOL CCurrentFolderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void OpenFolder()
{
	ProError status;
	ProMdl mdl;
	ProPath path;
	ProMdldata mdldata;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProDirectoryCurrentGet(path);
		ShellExecute(NULL, NULL, _T("explorer"), CString(path), NULL, SW_SHOW);
		return;
	}
	else
	{
		status = ProMdlDataGet(mdl, &mdldata);
		ShellExecute(NULL, NULL, _T("explorer"), CString(mdldata.device) + _T(":") + CString(mdldata.path), NULL, SW_SHOW);
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CurrentFolderID;

	status = ProMenubarMenuAdd("CurrentFolder", "CurrentFolder", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("CurrentFolder", "CurrentFolder", "CurrentFolder", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("CurrentFolder_Act", (uiCmdCmdActFn)OpenFolder, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &CurrentFolderID);
	status = ProMenubarmenuPushbuttonAdd("CurrentFolder", "CurrentFoldermenu", "CurrentFoldermenu", "CurrentFoldermenutips", NULL, PRO_B_TRUE, CurrentFolderID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}