// CableTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CableTest.h"

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

// CCableTestApp

BEGIN_MESSAGE_MAP(CCableTestApp, CWinApp)
END_MESSAGE_MAP()

// CCableTestApp 构造

CCableTestApp::CCableTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CCableTestApp 对象

CCableTestApp theApp;

// CCableTestApp 初始化

BOOL CCableTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
#pragma endregion

#pragma region 通用函数

ProMdl CurrentMdl()
{
	ProMdl mdl;
	ProError status;
	status = ProMdlCurrentGet(&mdl);
	return mdl;
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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_ASSEMBLY)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

wchar_t *ProUtilWstrcpy(wchar_t *ws1, wchar_t *ws2)
{
	ProWstringCopy(ws2, ws1, PRO_VALUE_UNUSED);
	return ws2;
}

int ProUtilWstrcmp(wchar_t *ws1, wchar_t *ws2)
{
	int result, status;
	status = ProWstringCompare(ws1, ws2, PRO_VALUE_UNUSED, &result);
	return result;
}

CString GetFileName(CString filePath, BOOL isWithFileSuffix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int pos = filePath.ReverseFind('\\');
	CString fileName = filePath.Right(filePath.GetLength() - pos - 1);
	if (FALSE == isWithFileSuffix)
	{
		pos = fileName.Find('.');
		fileName = fileName.Left(pos);
	}
	return fileName;
}

#pragma endregion

#pragma region 线束相关的定义和函数
typedef struct pro_cbl_harness
{
	ProHarness harness;
	ProName name;
	ProAssembly owner;
} ProCblHarness;

//IN,IN,IN,OUT
ProError InitHarness(ProPart harness_mdl, ProName name, ProAssembly asm_mdl, ProCblHarness *p_harness)
{
	if (p_harness == NULL)
		return PRO_TK_BAD_INPUTS;
	p_harness->harness = harness_mdl;
	ProUtilWstrcpy(p_harness->name, name);
	p_harness->owner = asm_mdl;
	return PRO_TK_NO_ERROR;
}

//IN,IN,OUT
ProError GetHarness(ProAssembly asm_mdl, ProName harnesss_name, ProCblHarness *p_cblharness)
{
	ProError status;
	ProHarness *harness_array;
	int num_harness;
	ProName name;
	if (asm_mdl == NULL)
		return PRO_TK_BAD_INPUTS;

	status = ProAssemblyHarnessesCollect(asm_mdl, &harness_array);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = ProArraySizeGet(harness_array, &num_harness);

	for (int i = 0; i < num_harness; i++)
	{
		status = ProMdlNameGet((ProMdl)harness_array[i], name);
		if (status == PRO_TK_NO_ERROR)
		{
			if (ProUtilWstrcmp(name, harnesss_name) == 0)
			{
				status = InitHarness((ProPart)harness_array[i], name, asm_mdl, p_cblharness);
				break;
			}
		}
	}
	status = ProArrayFree((ProArray *)&harness_array);
	return PRO_TK_E_NOT_FOUND;
}

//IN,IN,OUT
ProError CreateHarness(ProAssembly asm_mdl, ProName w_name, ProCblHarness *p_cblharness)
{
	ProError status;
	ProHarness harness;
	if (asm_mdl == NULL || p_cblharness == NULL)
		return PRO_TK_BAD_INPUTS;
	status = ProHarnessCreate(asm_mdl, w_name, &harness);
	if (status != PRO_TK_NO_ERROR)
		return status;
	status = InitHarness(harness, w_name, asm_mdl, p_cblharness);
	return status;
}
#pragma endregion

#pragma region 线轴相关的定义和函数
//IN,IN,OUT
ProError ReadSpoolFile(ProMdl asm_mdl, ProName file_fullname, ProSpool *p_spool)
{
	ProError status;
	CString CSfilename;

	if (asm_mdl == NULL || p_spool == NULL)
		return PRO_TK_BAD_INPUTS;
	status = ProInputFileRead(asm_mdl, file_fullname, PRO_SPOOL_FILE, NULL, NULL, NULL, NULL);
	if (status != PRO_TK_NO_ERROR)
		return status;
	CSfilename = CString(file_fullname);
	CSfilename = GetFileName(CSfilename, false);
	wchar_t *p = CSfilename.AllocSysString();
	ProUtilWstrcpy(p_spool->name, p);
	p_spool->owner = (ProAssembly)asm_mdl;
	SysFreeString(p);
	return status;
}

//IN,IN,IN,IN,OUT
ProError ProCblSpoolCreate(ProAssembly asm_mdl, ProName harness_name, ProCableType cable_type, ProBundleType sheath_type, ProSpool *p_spool)
{
	ProError status;
	if (asm_mdl == NULL || p_spool == NULL)
		return PRO_TK_BAD_INPUTS;
	status = ProSpoolCreate(asm_mdl, harness_name, cable_type, sheath_type, (ProSpool *)p_spool);
	if (status != PRO_TK_NO_ERROR)
		return status;
	return PRO_TK_NO_ERROR;
}
#pragma endregion

#pragma region 元件相关的定义和函数
//IN,IN,IN,IN,OUT
ProError ProCblCableCreate(ProCblHarness *p_cblharness, ProSpool *p_spool, ProName cable_name, ProCable *p_cable)
{
	ProError status;
	if (p_cblharness == NULL || p_cable == NULL)
		return PRO_TK_BAD_INPUTS;
	status = ProCableCreate(p_cblharness->harness, p_spool, cable_name, p_cable);

	if (status != PRO_TK_NO_ERROR)
		return status;
	return status;
}

#pragma endregion

#pragma region 布线
//本函数写死了Offset、ProCablelocationType等属性，需要修改
//IN,IN,IN,IN,IN
void RouteCable(ProSelection *sel_array, ProName harnessname, ProPath spoolpath, ProName cablename)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProAssembly asm_mdl;
	ProMdl mdl;
	int nSels = 0, sel_array_count;
	ProCblHarness cblharness;						  //新建的线束结构体对象，自定义结构便于程序扩展查找多个
	ProSpool spool;									  //新建的线轴结构体对象
	ProCable cable;									  //新建的元件对象
	ProCable *array_cable;							  //布线需要的参数
	ProAsmcomppath comp_path;						  //起始到终点Connector
	ProRouting cable_route_data;					  //联接线用的数据
	ProCablelocationref cablelocationref;			  //联接线用的数据
	ProCablelocation p_location, second_location;	  //联接线用的数据
	ProCablelocationType locType = PRO_LOC_CONNECTOR; //联接线对应坐标系，The coordinate system datum for the entry port;与选择的selection对应，axis就必须选axis
	ProVector offset = {0, 0, 0};					  //ProCablelocation对应的偏移量，ProCablelocationrefAlloc函数使用
	ProSelection *sel_array_route;					  //selection数组，ProCablelocationrefAlloc函数使用
	status = ProMdlCurrentGet(&mdl);
	asm_mdl = ProMdlToAssembly(mdl);

	//是否有同名线束(Harness)，有则直接初始化
	status = GetHarness(asm_mdl, harnessname, &cblharness);
	//没有则新建线束(Harness)
	if (status != PRO_TK_NO_ERROR)
	{
		status = CreateHarness(asm_mdl, harnessname, &cblharness);
		if (status != PRO_TK_NO_ERROR)
		{
			AfxMessageBox(_T("无法新建名为“") + CString(harnessname) + _T("”的线束对象。\n这是一个测试程序，名称已写死，请在当前打开的装配体中删除和拭除同名零件。"));
			return;
		}
	}

	//从文件读取线轴(spool)
	status = ReadSpoolFile(mdl, spoolpath, &spool);
	if (status != PRO_TK_NO_ERROR)
	{
		AfxMessageBox(_T("无法读取") + CString(spoolpath) + _T("线轴文件。\n这是一个测试程序，名称已写死，请提前准备好线轴文件。"));
		return;
	}
	//新建元件(Cable)
	status = ProCblCableCreate(&cblharness, &spool, cablename, &cable);
	if (status != PRO_TK_NO_ERROR)
	{
		AfxMessageBox(_T("无法新建名为“") + CString(cablename) + _T("”的元件对象。\n这是一个测试程序，名称已写死，请在当前打开的装配体中删除和拭除同名元件。"));
		return;
	}

	//开始布线(Route)，官方介绍的流程如下：
	//To Route a Group of Cables Through a Sequence of Locations:
	//1.Call ProCableRoutingStart() to identify the cables to be routed.
	//2.Call ProCablelocationrefAlloc() to create a routing reference location structure.
	//3.Call ProCableThruLocationRoute() for each location through which to route the cables.
	//4.Call ProCablelocationrefFree() to free the location reference.
	//5.Call ProCableRoutingEnd() to complete the routing.
	//6.Call ProSolidRegenerate() to make Creo Parametric calculate the resulting cable geometry and create the necessary cable features.
	//Note You must also call the function ProWindowRepaint() to see the new cables.

	//1.Call ProCableRoutingStart() to identify the cables to be routed.
	status = ProArrayAlloc(0, sizeof(ProCable), 1, (ProArray *)&array_cable);
	status = ProArrayObjectAdd((ProArray *)&array_cable, PRO_VALUE_UNUSED, 1, &cable);
	status = ProCableRoutingStart(asm_mdl, array_cable, &cable_route_data);
	if (status != PRO_TK_NO_ERROR)
	{
		status = ProArrayFree((ProArray *)&array_cable);
		return;
	}

	status = ProArraySizeGet(sel_array, &sel_array_count);
	for (int i = 0; i < sel_array_count; i++)
	{
		//对应起点
		status = ProSelectionAsmcomppathGet(sel_array[i], &comp_path);
		status = ProConnectorDesignate(&comp_path, NULL);
		//2.Call ProCablelocationrefAlloc() to create a routing reference location structure.
		//初始化ProCablelocationrefAlloc第二个参数，选择的第一个坐标系作为起点,sel_array只添加了一个
		status = ProArrayAlloc(0, sizeof(ProSelection), 1, (ProArray *)&sel_array_route);
		status = ProArrayObjectAdd((ProArray *)&sel_array_route, -1, 1, &sel_array[i]);
		status = ProCablelocationrefAlloc(locType, sel_array_route, PRO_B_TRUE, offset, &cablelocationref);

		//3.Call ProCableThruLocationRoute() for each location through which to route the cables.
		status = ProCableThruLocationRoute(cable_route_data, cablelocationref, &p_location, &second_location);

		//4.Call ProCablelocationrefFree() to free the location reference.
		status = ProCablelocationrefFree(cablelocationref);
		status = ProArrayFree((ProArray *)&sel_array_route);
	}

	//5.Call ProCableRoutingEnd() to complete the routing.
	status = ProCableRoutingEnd(cable_route_data);

	status = ProArrayFree((ProArray *)&array_cable);
	//6.Call ProSolidRegenerate() to make Creo Parametric calculate the resulting cable geometry and create the necessary cable features.
	//Note You must also call the function ProWindowRepaint() to see the new cables.
	status = ProSolidRegenerate((ProSolid)mdl, PRO_REGEN_NO_FLAGS);
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}
#pragma endregion

#pragma region 查找坐标系
typedef struct
{
	ProMdl model;
	ProCsys p_csys;
	ProName csys_name;
	ProModelitem modelitem;
	int id;
} UserCsysAppData;

typedef struct
{
	ProMdl mdl;
	ProCsys csys;
	ProModelitem modelitem;
	ProName mdl_name;
	ProName csys_name;
	ProAsmcomppath asmcomppath;
} UserComponentAppData;

ProError CsysFindFilterAction(ProCsys this_csys, ProAppData app_data)
{
	ProError status;
	ProName this_csys_name;
	ProModelitem mdlitem;
	int id;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	status = ProCsysIdGet(this_csys, &id);
	status = ProModelitemInit(((UserCsysAppData *)app_data)->model, id, PRO_CSYS, &mdlitem);
	status = ProModelitemNameGet(&mdlitem, this_csys_name);
	if (status != PRO_TK_NO_ERROR)
		return status;
	if (ProUtilWstrcmp(((UserCsysAppData *)app_data)->csys_name, this_csys_name) == 0)
		return PRO_TK_NO_ERROR;
	return PRO_TK_CONTINUE;
}

ProError CsysFindVisitAction(ProCsys this_csys, ProError filter_return, ProAppData app_data)
{
	int status, id;
	if (filter_return != PRO_TK_NO_ERROR)
		return filter_return;
	((UserCsysAppData *)app_data)->p_csys = this_csys;
	status = ProCsysIdGet(this_csys, &id);
	return PRO_TK_NO_ERROR;
}

//IN,OUT
ProError FindCsys(ProName csys_name, UserComponentAppData *componentAppData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProMdlType mdl_type;
	UserCsysAppData app_data;

	status = ProMdlTypeGet(componentAppData->mdl, &mdl_type);
	if (status != PRO_TK_NO_ERROR)
		return status;

	if ((mdl_type != PRO_MDL_PART) && (mdl_type != PRO_MDL_ASSEMBLY))
		return PRO_TK_INVALID_TYPE;

	ProUtilWstrcpy(app_data.csys_name, csys_name);
	app_data.model = componentAppData->mdl;

	status = ProAsmcomppathDispCsysVisit(&(componentAppData->asmcomppath), (ProSolid)componentAppData->mdl, (ProCsysFilterAction)CsysFindFilterAction, (ProCsysVisitAction)CsysFindVisitAction, (ProAppData)&app_data);
	if (status != PRO_TK_NO_ERROR)
		return PRO_TK_E_NOT_FOUND;
	componentAppData->csys = app_data.p_csys;
	status = ProCsysToGeomitem((ProSolid)(componentAppData->mdl), componentAppData->csys, &(componentAppData->modelitem));

	ProUtilWstrcpy(componentAppData->csys_name, app_data.csys_name);
	return PRO_TK_NO_ERROR;
}
#pragma endregion

#pragma region 遍历组件
ProError ComponentSysVisitAction(ProAsmcomppath *path, ProSolid solid, ProError status, ProAppData app_data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProMdl mdl;
	ProName mdlname;
	status = ProAsmcomppathMdlGet(path, &mdl);
	status = ProMdlNameGet(mdl, mdlname);
	if (ProUtilWstrcmp(((UserComponentAppData *)app_data)->mdl_name, mdlname) == 0)
	{
		((UserComponentAppData *)app_data)->mdl = mdl;
		ProUtilWstrcpy(((UserComponentAppData *)app_data)->mdl_name, mdlname);
		((UserComponentAppData *)app_data)->asmcomppath.table_num = path->table_num;
		((UserComponentAppData *)app_data)->asmcomppath.owner = path->owner;
		for (int i = 0; i < PRO_MAX_ASSEM_LEVEL; i++)
		{
			((UserComponentAppData *)app_data)->asmcomppath.comp_id_table[i] = path->comp_id_table[i];
		}
	}
	return status;
}

//ProSolidDispCompVisit访问子装配体的part和asm
//所以不需要遍历，但是带来的问题是多个同名零件的区分用ProAsmcomppath
//本例靠循环读取所有的，所以如果用同名的零件会读取到装配树中最后的零件
//IN,IN,IN,OUT
ProError FindCsysinComponent(ProName mdl_name, ProName csys_name, UserComponentAppData *app_data)
{
	ProError status;
	ProMdl asm_mdl;

	asm_mdl = CurrentMdl();
	UserComponentAppData componentAppData;
	componentAppData.mdl = NULL;
	componentAppData.csys = NULL;
	ProUtilWstrcpy(componentAppData.mdl_name, mdl_name);

	status = ProSolidDispCompVisit(ProMdlToSolid(asm_mdl), (ProSolidDispCompVisitAction)ComponentSysVisitAction, NULL, (ProAppData)&componentAppData);
	if (status != PRO_TK_NO_ERROR)
		return status;
	if (componentAppData.mdl == NULL)
		return PRO_TK_E_NOT_FOUND;
	else
	{
		app_data->mdl = componentAppData.mdl;
		ProUtilWstrcpy(app_data->mdl_name, componentAppData.mdl_name);
		app_data->asmcomppath = componentAppData.asmcomppath;
		status = FindCsys(csys_name, app_data);
		return status;
	}
}
#pragma endregion

void InsertCable()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	ProSelection ss1, ss2;
	ProSelection *sel_array;
	UserComponentAppData componentAppData_start, componentAppData_end;
	ProName mdl_name;
	ProName csys_name;
	/**************************************/
	/*********手动选择两个坐标系的代码********/
	/**************************************/
	int nSels = 0;
	////选择两个坐标系用于布线，	//自动化的时候遍历装配体可以使用ProSelectionAlloc程序自动添加selection
	AfxMessageBox(_T("同时选择两个坐标系用于布线"));
	status = ProSelect("csys", 2, NULL, NULL, NULL, NULL, &sel_array, &nSels); //filter与ProCablelocationref类型一致
	if (status != PRO_TK_NO_ERROR || nSels <= 0)
	{
		AfxMessageBox(_T("需要选择两个坐标系才能进行布线。\n这是一个测试程序，仅为演示使用。"));
		return;
	}

	/********************************************/
	/*********指定坐标系、零件的名称自动操作********/
	/********************************************/
	//ProUtilWstrcpy(mdl_name, L"PRT0001");
	//ProUtilWstrcpy(csys_name, L"CS01");
	//status = FindCsysinComponent(mdl_name, csys_name, &componentAppData_start);

	//ProUtilWstrcpy(mdl_name, L"PRT0002");
	//ProUtilWstrcpy(csys_name, L"CS02");
	//status = FindCsysinComponent(mdl_name, csys_name, &componentAppData_end);

	//status = ProSelectionAlloc(&(componentAppData_start.asmcomppath), &(componentAppData_start.modelitem), &ss1);
	//status = ProSelectionAlloc(&(componentAppData_end.asmcomppath), &(componentAppData_end.modelitem), &ss2);

	//status = ProArrayAlloc(0, sizeof(ProSelection), 1, (ProArray *)&sel_array);
	//status = ProArrayObjectAdd((ProArray *)&sel_array, -1, 1, &ss1);
	//status = ProArrayObjectAdd((ProArray *)&sel_array, -1, 1, &ss2);

	//注意Creo会自动把Harness和Cable的名称改为大写，调用下面函数时两个参数自行转为大写
	RouteCable(sel_array, L"NEWHARNESS", L"D:\\ProeRes\\cabletest\\awg_20_red.spl", L"NEWCABLE");
	status = ProArrayFree((ProArray *)&sel_array);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId CableTest;

	status = ProMenubarMenuAdd("CableTest", "CableTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProCmdActionAdd("CableTest_Act", (uiCmdCmdActFn)InsertCable, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &CableTest);
	status = ProMenubarmenuPushbuttonAdd("CableTest", "CableTestmenu", "CableTestmenu", "CableTestmenutips", NULL, PRO_B_TRUE, CableTest, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}