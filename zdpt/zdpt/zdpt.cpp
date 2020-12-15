// zdpt.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "zdpt.h"

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

// CzdptApp

BEGIN_MESSAGE_MAP(CzdptApp, CWinApp)
END_MESSAGE_MAP()


// CzdptApp 构造

CzdptApp::CzdptApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CzdptApp 对象

CzdptApp theApp;


// CzdptApp 初始化

BOOL CzdptApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

typedef enum _hint
{
	Fun = 0,   
	About = 1,
} HINT;
HINT hint;

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}



static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void zdpt()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProPath *file_list,*dir_list;
	int n_files;
	ProPath currentpath;
	CString filename;
	CString macro;
	status = ProDirectoryCurrentGet(currentpath);

	CString Cfilter = _T("*.drw");
	wchar_t *filter = NULL;
	filter = Cfilter.AllocSysString();
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray*)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray*)&dir_list);
	status = ProFilesList(currentpath, filter, PRO_FILE_LIST_LATEST,  &file_list, &dir_list);

	if(filter!=NULL)
		SysFreeString(filter);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if(n_files>1)
		{
			ProPath savepath;
			status = ProFileSave(NULL,filter,NULL,NULL,NULL,NULL,savepath);
			if(status != PRO_TK_NO_ERROR)
				return;
			status = ProMdlfileCopy(PRO_MDL_DRAWING,file_list[0],savepath);
			if(status != PRO_TK_NO_ERROR)
			{
				AfxMessageBox(_T("无法创建或覆盖文件!"));
				status = ProArrayFree((ProArray*)&file_list);
				status = ProArrayFree((ProArray*)&dir_list);
				return;
			}
			filename = CString(savepath);
			filename.Replace(_T("\\"),_T("\\\\"));
			macro = _T("~ Command `ProCmdModelOpen` ;~ Trail `UI Desktop` `UI Desktop` `DLG_PREVIEW_POST` `file_open`;~ Update `file_open` `Inputname` `"+filename+"`;~ Trail `UI Desktop` `UI Desktop` `PREVIEW_POPUP_TIMER` `file_open:Ph_list.Filelist:<NULL>`;~ Command `ProFileSelPushOpen@context_dlg_open_cmd`;");
		}
		else
		{
			AfxMessageBox(_T("当前目录包含的绘图文件数量小于1,无需排图!"));
		}
		for (int i=1; i<n_files; i++)
		{
			CString tmp = CString(file_list[i]);
			tmp.Replace(_T("\\"),_T("\\\\"));
			if(tmp == filename)
				continue;
			macro+=_T("~ Command `ProCmdDwgImpAppend` ;~ Trail `UI Desktop` `UI Desktop` `DLG_PREVIEW_POST` `file_open`;~ Update `file_open` `Inputname` `")+tmp+_T("`;~ Command `ProFileSelPushOpen@context_dlg_open_cmd`;");
		}
	}
	status = ProArrayFree((ProArray*)&file_list);
	status = ProArrayFree((ProArray*)&dir_list);
	macro += _T("~ Command `About_Act`;");
	hint = Fun;
	wchar_t *p = macro.AllocSysString();
	ProMacroLoad(p);
	SysFreeString(p);
}

void about()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (hint == Fun)
	{
		ShowDialog(L"自动排图结束。");
	}
	else
	{
		ShowDialog(L"用于自动排图。\n访问我的博客获取更多信息：\nhttp://www.hudi.site");
	}
	hint = About;
}


extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId zdptID, AboutId;

	status = ProMenubarMenuAdd("zdpt", "zdpt", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("zdpt", "zdpt", "zdpt", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("zdpt_Act", (uiCmdCmdActFn)zdpt, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &zdptID);
	status = ProMenubarmenuPushbuttonAdd("zdpt", "zdptmenu", "zdptmenu", "zdptmenutips", NULL, PRO_B_TRUE, zdptID, MSGFILE);

	status = ProCmdActionAdd("About_Act", (uiCmdCmdActFn)about, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &AboutId);
	status = ProMenubarmenuPushbuttonAdd("zdpt", "About", "About", "Abouttips", NULL, PRO_B_TRUE, AboutId, MSGFILE);

	hint = About;

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
