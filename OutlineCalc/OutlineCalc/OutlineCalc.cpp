// OutlineCalc.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OutlineCalc.h"

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

// COutlineCalcApp

BEGIN_MESSAGE_MAP(COutlineCalcApp, CWinApp)
END_MESSAGE_MAP()

// COutlineCalcApp 构造

COutlineCalcApp::COutlineCalcApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 COutlineCalcApp 对象

COutlineCalcApp theApp;

// COutlineCalcApp 初始化

BOOL COutlineCalcApp::InitInstance()
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

static double identity_matrix[4][4] = {{1.0, 0.0, 0.0, 0.0},
									   {0.0, 1.0, 0.0, 0.0},
									   {0.0, 0.0, 1.0, 0.0},
									   {0.0, 0.0, 0.0, 1.0}};

/*====================================================================*\
    FUNCTION :	ProUtilMatrixCopy()
    PURPOSE  :	Copy one matrix to another, or initialize a matrix to
		be the unitary matrix
\*====================================================================*/
void ProUtilMatrixCopy(
	double input[4][4],
	double output[4][4])
{
	int i, j;

	if (input == NULL)
	{
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				output[i][j] = identity_matrix[i][j];
	}
	else
	{
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				output[i][j] = input[i][j];
	}
}

/*====================================================================*\
    FUNCTION :	ProUtilMatrixInvert()
    PURPOSE  :	Find the inverse of a transformation matrix
\*====================================================================*/
int ProUtilMatrixInvert(
	double m[4][4],
	double output[4][4])
{
	double vec[3], scale_sq, inv_sq_scale;
	int i, j;

	/*--------------------------------------------------------------------*\
    If the matrix is null, return the identity matrix
\*--------------------------------------------------------------------*/
	if (m == NULL)
	{
		ProUtilMatrixCopy(NULL, output);
		return (1);
	}

	/*--------------------------------------------------------------------*\
    Obtain the matrix scale
\*--------------------------------------------------------------------*/
	vec[0] = m[0][0];
	vec[1] = m[0][1];
	vec[2] = m[0][2];
	scale_sq = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];

	/*--------------------------------------------------------------------*\
    Check whether there is an inverse, and if not, return 0
\*--------------------------------------------------------------------*/
	if (scale_sq < (.000000001 * .000000001))
		return (0);

	/*--------------------------------------------------------------------*\
    Need the inverse scale squared 
\*--------------------------------------------------------------------*/
	inv_sq_scale = 1.0 / scale_sq;

	/*--------------------------------------------------------------------*\
    The orientation vectors
\*--------------------------------------------------------------------*/
	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 3; i++)
			output[j][i] = m[i][j] * inv_sq_scale;
		output[j][3] = 0.0;
	}

	/*--------------------------------------------------------------------*\
    The shift vectors
\*--------------------------------------------------------------------*/
	for (i = 0; i < 3; i++)
	{
		output[3][i] = 0.0;
		for (j = 0; j < 3; j++)
			output[3][i] -= m[i][j] * m[3][j] * inv_sq_scale;
	}
	output[3][3] = 1.0;
	return (1);
}

/*====================================================================*\
Function :  ProUtilVectorsToTransf()
Purpose  :  Creates transformation from vectors defining it
\*====================================================================*/
ProError ProUtilVectorsToTransf(
	double x_vector[3],		/* IN - X directional vector   */
	double y_vector[3],		/* IN - Y directional vector   */
	double z_vector[3],		/* IN - Z directional vector   */
	double o_vector[3],		/* IN - Origin                 */
	double transform[4][4]) /* OUT - transformation matrix */
{
	int n;
	if (x_vector != NULL)
	{
		transform[0][0] = x_vector[0];
		transform[0][1] = x_vector[1];
		transform[0][2] = x_vector[2];
	}
	if (y_vector != NULL)
	{
		transform[1][0] = y_vector[0];
		transform[1][1] = y_vector[1];
		transform[1][2] = y_vector[2];
	}
	if (z_vector != NULL)
	{
		transform[2][0] = z_vector[0];
		transform[2][1] = z_vector[1];
		transform[2][2] = z_vector[2];
	}
	if (o_vector != NULL)
	{
		transform[3][0] = o_vector[0];
		transform[3][1] = o_vector[1];
		transform[3][2] = o_vector[2];
	}
	for (n = 0; n < 4; n++)
		transform[n][3] = (n == 3) ? 1.0 : 0.0;
	return (PRO_TK_NO_ERROR);
}

void OutlineCalc()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	int sel_count;
	ProSelection *psels = NULL;
	ProModelitem csys_feat;
	ProGeomitemdata *geom_data = NULL;
	ProCsysdata *p_csys = NULL;
	ProMdl solid;
	Pro3dPnt outline[2];
	ProMatrix transf, itranf;
	ProSolidOutlExclTypes excludes[] = {PRO_OUTL_EXC_DATUM_PLANE, PRO_OUTL_EXC_DATUM_POINT, PRO_OUTL_EXC_DATUM_CSYS};
	status = ProMessageDisplay(MSGFILE, "entermsg");
	if ((ProSelect("csys", 1, NULL, NULL, NULL, NULL, &psels, &sel_count) != PRO_TK_NO_ERROR) || (sel_count < 1))
	{
		return;
	}
	status = ProSelectionModelitemGet(psels[0], &csys_feat);
	status = ProGeomitemdataGet(&csys_feat, &geom_data);
	if (geom_data->obj_type != PRO_CSYS)
	{
		return;
	}
	p_csys = geom_data->data.p_csys_data;
	ProUtilVectorsToTransf(p_csys->x_vector, p_csys->y_vector, p_csys->z_vector, p_csys->origin, transf);
	ProUtilMatrixInvert(transf, itranf);
	status = ProMdlCurrentGet(&solid);
	status = ProSolidOutlineCompute((ProSolid)solid, transf, excludes, 3, outline);
	CString Msg;
	Msg.Format("长:%f\n宽：%f\n高:%f", abs(outline[1][0] - outline[0][0]), abs(outline[1][1] - outline[0][1]), abs(outline[1][2] - outline[0][2]));
	AfxMessageBox(Msg);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId OutlineCalcID;

	status = ProMenubarMenuAdd("OutlineCalc", "OutlineCalc", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("OutlineCalc", "OutlineCalc", "OutlineCalc", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("OutlineCalc_Act", (uiCmdCmdActFn)OutlineCalc, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &OutlineCalcID);
	status = ProMenubarmenuPushbuttonAdd("OutlineCalc", "OutlineCalcmenu", "OutlineCalcmenu", "OutlineCalcmenutips", NULL, PRO_B_TRUE, OutlineCalcID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}