// FitTolList.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FitTolList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma region MFC
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

// CFitTolListApp

BEGIN_MESSAGE_MAP(CFitTolListApp, CWinApp)
END_MESSAGE_MAP()

// CFitTolListApp 构造

CFitTolListApp::CFitTolListApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CFitTolListApp 对象

CFitTolListApp theApp;

// CFitTolListApp 初始化

BOOL CFitTolListApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

ProError GToInterCal()
{
	ProError status;
	int n_parts, i;
	double volume;
	ProMdl mdl;
	ProModelitem part1, part2;
	ProName name1, name2;
	ProInterferenceInfo *interf_info_arr;
	status = ProMdlCurrentGet(&mdl);
	status = ProFitGlobalinterferenceCompute((ProAssembly)mdl, PRO_FIT_SUB_ASSEMBLY, PRO_B_FALSE, PRO_B_FALSE, PRO_B_FALSE, &interf_info_arr);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_GENERAL_ERROR;

	status = ProArraySizeGet(interf_info_arr, &n_parts);
	for (i = 0; i < n_parts; i++)
	{
		status = ProSelectionModelitemGet(interf_info_arr[i].part_1, &part1);
		status = ProMdlNameGet(part1.owner, name1);
		status = ProSelectionModelitemGet(interf_info_arr[i].part_2, &part2);
		status = ProMdlNameGet(part2.owner, name2);
		status = ProFitInterferencevolumeCompute(interf_info_arr[i].interf_data, &volume);
		CString a = CString(name1);
		CString b = CString(name2);
		CString c;
		c.Format(_T("%lf"), volume);
		AfxMessageBox(a + _T("和") + b + _T("发生干涉，干涉量为") + c);
	}
	status = ProInterferenceInfoProarrayFree(interf_info_arr);
	return PRO_TK_NO_ERROR;
}

ProError SelpartInterference()
{
	ProError status;
	int nSels = 0;
	ProSelection *sel_array;
	ProModelitem part1, part2;
	ProName name1, name2;
	ProInterferenceData interf_data;
	double volume;
	CString inter;

	status = ProSelect("prt_or_asm", 2, NULL, NULL, NULL, NULL, &sel_array, &nSels);
	if (status != PRO_TK_NO_ERROR || nSels <= 0)
	{
		return PRO_TK_GENERAL_ERROR;
	}
	status = ProFitInterferenceCompute(sel_array[0], sel_array[1], PRO_B_FALSE, PRO_B_FALSE, &interf_data);
	if (interf_data == NULL)
	{
		AfxMessageBox(_T("未发生干涉。"));
		return PRO_TK_NO_ERROR;
	}

	status = ProSelectionModelitemGet(sel_array[0], &part1);
	status = ProMdlNameGet(part1.owner, name1);
	status = ProSelectionModelitemGet(sel_array[1], &part2);
	status = ProMdlNameGet(part2.owner, name2);
	status = ProFitInterferencevolumeCompute(interf_data, &volume);
	CString a = CString(name1);
	CString b = CString(name2);
	CString c;
	c.Format(_T("%lf"), volume);
	AfxMessageBox(a + _T("和") + b + _T("发生干涉，干涉量为") + c);
	status = ProFitInterferencevolumeDisplay(interf_data, PRO_COLOR_HIGHLITE);
	status = ProInterferenceDataFree(interf_data);
	return PRO_TK_NO_ERROR;
}

void FitTolListCmd1()
{
	GToInterCal();
}
void FitTolListCmd2()
{
	SelpartInterference();
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
	uiCmdCmdId FitTolList_cmd_id1, FitTolList_cmd_id2;
	status = ProMenubarMenuAdd("FitTolList", "FitTolList", "File", PRO_B_TRUE, L"FitTolList.txt");

	status = ProCmdActionAdd("IMI_FitTolList_Act1", (uiCmdCmdActFn)FitTolListCmd1, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &FitTolList_cmd_id1);
	status = ProMenubarmenuPushbuttonAdd("FitTolList", "Cmd1", "Cmd1", "Tips1", NULL, PRO_B_TRUE, FitTolList_cmd_id1, L"FitTolList.txt");

	status = ProCmdActionAdd("IMI_FitTolList_Act2", (uiCmdCmdActFn)FitTolListCmd2, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &FitTolList_cmd_id2);
	status = ProMenubarmenuPushbuttonAdd("FitTolList", "Cmd2", "Cmd2", "Tips2", NULL, PRO_B_TRUE, FitTolList_cmd_id2, L"FitTolList.txt");

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
