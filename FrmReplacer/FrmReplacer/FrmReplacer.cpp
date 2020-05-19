// FrmReplacer.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FrmReplacer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFrmReplacerApp, CWinApp)
END_MESSAGE_MAP()

CFrmReplacerApp::CFrmReplacerApp()
{
}

CFrmReplacerApp theApp;

BOOL CFrmReplacerApp::InitInstance()
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

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_DRAWING)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

void SetSheet(CString Frm, BOOL Deltable)
{
	ProError status;
	ProMdl mdl;
	ProMdl format;
	ProDwgtable *tables = NULL;
	int num, Cur_Sheet;
	ProBoolean from_format;
	status = ProMdlCurrentGet(&mdl);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProDrawingCurrentSheetGet((ProDrawing)mdl, &Cur_Sheet);

	if (Deltable)
	{
		status = ProDrawingTablesCollect((ProDrawing)mdl, &tables);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProArraySizeGet((ProArray)tables, &num);
			for (int i = 0; i < num; i++)
			{
				status = ProDwgtableIsFromFormat(&tables[i], &from_format);
				if (from_format == PRO_B_TRUE)
					status = ProDwgtableDelete(&tables[i], 1);
			}
			status = ProArrayFree((ProArray *)&tables);
		}
	}
	wchar_t *p = Frm.AllocSysString();
	status = ProMdlRetrieve(p, PRO_MDL_DWGFORM, &format);
	SysFreeString(p);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProDrawingFormatAdd((ProDrawing)mdl, Cur_Sheet, NULL, format, 0);
	if (status != PRO_TK_NO_ERROR)
		return;
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

CString ChooseFrmBox()
{
	BOOL isOpen = TRUE;
	CString filter = "图框文件 (*.frm.*)|*.frm.*||"; //文件过虑的类型
	CFileDialog openFileDlg(isOpen, "", "", OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		return openFileDlg.GetPathName();
	}
	else
	{
		return "";
	}
}

void ChangeFrm()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString Frmfile = ChooseFrmBox();
	if (Frmfile != "")
	{
		BOOL Deltab = AfxMessageBox("是否删除内置表格？", MB_YESNO) == IDYES ? TRUE : FALSE;
		SetSheet(Frmfile, Deltab);
	}
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FrmReplacerID;

	status = ProMenubarMenuAdd("FrmReplacer", "FrmReplacer", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FrmReplacer", "FrmReplacer", "FrmReplacer", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FrmReplacer_Act", (uiCmdCmdActFn)ChangeFrm, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &FrmReplacerID);
	status = ProMenubarmenuPushbuttonAdd("FrmReplacer", "FrmReplacermenu", "FrmReplacermenu", "FrmReplacermenutips", NULL, PRO_B_TRUE, FrmReplacerID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}