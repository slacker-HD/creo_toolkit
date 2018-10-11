// RelFunctions.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RelFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#
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

// CRelFunctionsApp

BEGIN_MESSAGE_MAP(CRelFunctionsApp, CWinApp)
END_MESSAGE_MAP()

// CRelFunctionsApp 构造

CRelFunctionsApp::CRelFunctionsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CRelFunctionsApp 对象

CRelFunctionsApp theApp;

// CRelFunctionsApp 初始化

BOOL CRelFunctionsApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion
ProRelfuncArg* Args_StrToDouble = NULL;
ProRelfuncArg* Args_DoubleToStr = NULL;
ProError StrToDouble (ProRelset* relset, ProMdl mdl,char* ext_func_name,ProParamvalue* args,ProAppData data, ProParamvalue* result)
{
	CString str = CString(args [0].value.s_val);
	result->type = PRO_PARAM_DOUBLE;
	result->value.d_val = atof(str);
	return (PRO_TK_NO_ERROR);
}
ProError DoubleToStr (ProRelset* relset, ProMdl mdl,char* ext_func_name,ProParamvalue* args,ProAppData data, ProParamvalue* result)
{
	CString str;
	int i;
	str.Format("%g",args [0].value.d_val);
	wchar_t *p = str.AllocSysString();
	result->type = PRO_PARAM_STRING;
	for(i = 0; i<PRO_LINE_SIZE; i++)
	{
		if(i == str.GetLength())
			break;
		result->value.s_val[i] = p[i];
	}
	if(i < PRO_LINE_SIZE)
		result->value.s_val[i] = '\0';
	SysFreeString(p);
	p = NULL;
	return (PRO_TK_NO_ERROR);
}
extern "C" int user_initialize()
{
	ProError status;
	status = ProArrayAlloc (1, sizeof (ProRelfuncArg), 1, (ProArray*)&Args_StrToDouble);
	Args_StrToDouble [0].type = PRO_PARAM_STRING;
	Args_StrToDouble [0].attributes = PRO_RELF_ATTR_NONE;

	status = ProArrayAlloc (1, sizeof (ProRelfuncArg), 1, (ProArray*)&Args_DoubleToStr);
	Args_DoubleToStr [0].type = PRO_PARAM_DOUBLE;
	Args_DoubleToStr [0].attributes = PRO_RELF_ATTR_NONE;

	status = ProRelationFunctionRegister ("IMI_StrToDouble", Args_StrToDouble,  StrToDouble, NULL, NULL,PRO_B_FALSE, NULL);
	status = ProRelationFunctionRegister ("IMI_DoubleToStr", Args_DoubleToStr,  DoubleToStr, NULL, NULL,PRO_B_FALSE, NULL);

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	status = ProArrayFree((ProArray*)&Args_StrToDouble);
	Args_StrToDouble = NULL;
	status = ProArrayFree((ProArray*)&Args_DoubleToStr);
	Args_DoubleToStr = NULL;
}
