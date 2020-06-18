// JpgSnapShot.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "JpgSnapShot.h"

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

// CJpgSnapShotApp

BEGIN_MESSAGE_MAP(CJpgSnapShotApp, CWinApp)
END_MESSAGE_MAP()

// CJpgSnapShotApp 构造

CJpgSnapShotApp::CJpgSnapShotApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CJpgSnapShotApp 对象

CJpgSnapShotApp theApp;

// CJpgSnapShotApp 初始化

BOOL CJpgSnapShotApp::InitInstance()
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

static uiCmdAccessState AccessPRTORASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_PART || CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void Snapshot()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	int wid;
	ProPath currentPath;
	CString filter = "图片文件 (*.jpg)|*.jpg";

	CFileDialog saveFileDlg(FALSE, "", "", OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	if (saveFileDlg.DoModal() == IDOK)
	{
		status = ProDirectoryCurrentGet(currentPath);
		wchar_t *p = saveFileDlg.GetFolderPath().AllocSysString();
		status = ProDirectoryChange(p);
		SysFreeString(p);
		status = ProWindowCurrentGet(&wid);
		status = ProViewReset(NULL, NULL);
		status = ProMacroLoad(L"~ Command `ProCmdEnvDtmDisp` 0; ~ Command `ProCmdEnvAxisDisp` 0; ~ Command `ProCmdViewSpinCntr` 0; ~ Command `ProCmdEnvPntsDisp`  0;~ Command `ProCmdEnvCsysDisp`  0;");//不显示线框和坐标系，后面的刷新等其实也可以用宏来做的
		status = ProMacroExecute();
		status = ProWindowClear(wid);
		status = ProWindowRefresh(PRO_VALUE_UNUSED);

		p = saveFileDlg.GetFileName().AllocSysString();
		status = ProRasterFileWrite(wid, PRORASTERDEPTH_24, 10, 10, PRORASTERDPI_100, PRORASTERTYPE_JPEG, p);//修改参数以适应需要的图片的dpi以及尺寸
		SysFreeString(p);
		status = ProDirectoryChange(currentPath);
	}
}
extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId JpgSnapShotID;

	status = ProMenubarMenuAdd("JpgSnapShot", "JpgSnapShot", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("JpgSnapShot", "JpgSnapShot", "JpgSnapShot", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("JpgSnapShot1_Act", (uiCmdCmdActFn)Snapshot, uiProeImmediate, AccessPRTORASM, PRO_B_TRUE, PRO_B_TRUE, &JpgSnapShotID);
	status = ProMenubarmenuPushbuttonAdd("JpgSnapShot", "JpgSnapShotmenu", "JpgSnapShotmenu", "JpgSnapShotmenutips", NULL, PRO_B_TRUE, JpgSnapShotID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}