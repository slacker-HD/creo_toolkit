// MdlRename.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MdlRename.h"

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

// CMdlRenameApp

BEGIN_MESSAGE_MAP(CMdlRenameApp, CWinApp)
END_MESSAGE_MAP()

// CMdlRenameApp 构造

CMdlRenameApp::CMdlRenameApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMdlRenameApp 对象

CMdlRenameApp theApp;

// CMdlRenameApp 初始化

BOOL CMdlRenameApp::InitInstance()
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

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_PART)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void RenamePrtandDrw()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProMdl mdl, drw;
	ProMdldata mdldata;
	ProName newName;
	CString drwname;
	ProPath currentPath;
	bool findDrw;
	status = ProMdlCurrentGet(&mdl);
	status = ProDirectoryCurrentGet(currentPath);
	status = ProMessageDisplay(MSGFILE, "Enter Prompt");
	status = ProMessageStringRead(PRO_NAME_SIZE, newName);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProMdlDataGet(mdl, &mdldata);
		drwname = CString(mdldata.device) + _T(":") + CString(mdldata.path);
		wchar_t *p = drwname.AllocSysString();
		status = ProDirectoryChange(p);
		findDrw = ProMdlLoad(mdldata.name, PRO_MDL_DRAWING, PRO_B_FALSE, &drw) == PRO_TK_NO_ERROR ? true : false;
		status = ProMdlRename(mdl, newName);
		status = ProMdlSave(mdl);
		if (findDrw)
		{
			status = ProMdlRename(drw, newName);
			status = ProMdlSave(drw);
		}
		SysFreeString(p);
		status = ProDirectoryChange(currentPath);
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId MdlRenameID;

	status = ProMenubarMenuAdd("MdlRename", "MdlRename", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("MdlRename", "MdlRename", "MdlRename", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("MdlRename_Act", (uiCmdCmdActFn)RenamePrtandDrw, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &MdlRenameID);
	status = ProMenubarmenuPushbuttonAdd("MdlRename", "MdlRenamemenu", "MdlRenamemenu", "MdlRenamemenutips", NULL, PRO_B_TRUE, MdlRenameID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}