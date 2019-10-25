// ModelPurge.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ModelPurge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CModelPurgeApp, CWinApp)
END_MESSAGE_MAP()

CModelPurgeApp::CModelPurgeApp()
{
}

CModelPurgeApp theApp;

BOOL CModelPurgeApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void Recycle(CString pszPath, BOOL bDelete)
{
	SHFILEOPSTRUCT shDelFile;
	memset(&shDelFile, 0, sizeof(SHFILEOPSTRUCT));
	shDelFile.fFlags |= FOF_SILENT;
	shDelFile.fFlags |= FOF_NOERRORUI;
	shDelFile.fFlags |= FOF_NOCONFIRMATION;

	TCHAR buf[_MAX_PATH + 1];
	strcpy_s(buf, pszPath);
	buf[_tcslen(buf) + 1] = 0;
	shDelFile.wFunc = FO_DELETE;
	shDelFile.pFrom = buf;
	shDelFile.pTo = NULL;
	if (bDelete)
	{
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;
	}
	else
	{
		shDelFile.fFlags |= FOF_ALLOWUNDO;
	}
	SHFileOperation(&shDelFile);
}

CString Rfilename(ProPath path)
{
	ProError status;
	ProName r_file_name, r_extension;
	status = ProFilenameParse(path, NULL, r_file_name, r_extension, NULL);
	CString name = CString(r_file_name) + _T(".") + CString(r_extension);
	return name;
}

int ProCompareVersion(ProPath p1, ProPath p2)
{
	int ver1 = 0, ver2 = 0;
	ProError status;
	status = ProFilenameParse(p1, NULL, NULL, NULL, &ver1);
	status = ProFilenameParse(p2, NULL, NULL, NULL, &ver2);
	return (ver1 - ver2);
}

void ProMdlPurge(ProName w_name)
{
	ProError status;
	ProPath *file_list, *dir_list;
	wchar_t *p = NULL;
	int i, n_files;
	CString Filename;
	Filename = Rfilename(w_name);
	p = Filename.AllocSysString();
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, p, PRO_FILE_LIST_ALL, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 1)
		{
			qsort(file_list, n_files, sizeof(file_list[0]), (int (*)(PRO_CONST_ARG void *, PRO_CONST_ARG void *))ProCompareVersion);
		}
		for (i = 0; i < n_files - 1; i++)
		{
			CString tmp = CString(file_list[i]);
			Recycle(tmp, false);
		}
	}
	SysFreeString(p);
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}

void ProMdlPurgeFileType(wchar_t *Type)
{
	ProError status;
	ProPath *file_list, *dir_list;
	int n_files;
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, Type, PRO_FILE_LIST_LATEST, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 0)
		{
			for (int i = 0; i < n_files; i++)
			{
				ProMdlPurge(file_list[i]);
			}
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}

void ProMdlPurgeAll()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProMdlPurgeFileType(L"*.drw");
	ProMdlPurgeFileType(L"*.prt");
	ProMdlPurgeFileType(L"*.asm");
	AfxMessageBox(_T("当前工作目录已清理。"));
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ModelPurgeID;

	status = ProMenubarMenuAdd("ModelPurge", "ModelPurge", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("ModelPurge", "ModelPurge", "ModelPurge", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ModelPurge_Act", (uiCmdCmdActFn)ProMdlPurgeAll, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ModelPurgeID);
	status = ProMenubarmenuPushbuttonAdd("ModelPurge", "ModelPurgemenu", "ModelPurgemenu", "ModelPurgemenutips", NULL, PRO_B_TRUE, ModelPurgeID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}