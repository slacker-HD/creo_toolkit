// MaterialSet.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MaterialSet.h"

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

// CMaterialSetApp

BEGIN_MESSAGE_MAP(CMaterialSetApp, CWinApp)
END_MESSAGE_MAP()

// CMaterialSetApp 构造

CMaterialSetApp::CMaterialSetApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMaterialSetApp 对象

CMaterialSetApp theApp;

// CMaterialSetApp 初始化

BOOL CMaterialSetApp::InitInstance()
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

void ChangeMaterial()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProPath currentPath;
	ProError status;
	ProMdl mdl;
	ProMaterial material;

	CString filter = "材料文件 (*.mtl)|*.mtl||";
	CFileDialog openFileDlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	if (openFileDlg.DoModal() == IDOK)
	{
		status = ProDirectoryCurrentGet(currentPath);
		status = ProMdlCurrentGet(&mdl);

		wchar_t *p = openFileDlg.GetFolderPath().AllocSysString();
		status = ProDirectoryChange(p);
		SysFreeString(p);

		p = openFileDlg.GetFileTitle().AllocSysString();
		status = ProMaterialfileRead(ProMdlToSolid(mdl), p);
		status = ProWstringCopy(p, material.matl_name, PRO_VALUE_UNUSED);
		material.part = ProMdlToSolid(mdl);
		SysFreeString(p);

		status = ProMaterialCurrentSet(&material);

		status = ProDirectoryChange(currentPath);
		status = ProMacroLoad(L"~ Command `ProCmdModelSave` ;~ Activate `file_saveas` `OK`;");
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId MaterialSetID;

	status = ProMenubarMenuAdd("MaterialSet", "MaterialSet", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("MaterialSet", "MaterialSet", "MaterialSet", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("MaterialSet1_Act", (uiCmdCmdActFn)ChangeMaterial, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &MaterialSetID);
	status = ProMenubarmenuPushbuttonAdd("MaterialSet", "MaterialSetmenu", "MaterialSetmenu", "MaterialSetmenutips", NULL, PRO_B_TRUE, MaterialSetID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}