// AuxiliaryLine.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AuxiliaryLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAuxiliaryLineApp, CWinApp)
END_MESSAGE_MAP()

CAuxiliaryLineApp::CAuxiliaryLineApp()
{
}

CAuxiliaryLineApp theApp;

BOOL CAuxiliaryLineApp::InitInstance()
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

void DrawLine(ProDrawing Drawing, ProView View, ProPoint3d Start, ProPoint3d End, ProColortype Color, ProName Font)
{
	ProError status;
	ProDtlentitydata edata;
	ProCurvedata *curve;
	ProDtlentity entity;
	ProColor entity_color;

	status = ProDtlentitydataAlloc(Drawing, &edata);
	status = ProCurvedataAlloc(&curve);
	status = ProLinedataInit(Start, End, curve);
	status = ProDtlentitydataCurveSet(edata, curve);

	status = ProDtlentitydataViewSet(edata, View);

	entity_color.value.type = PRO_COLOR_LETTER;
	entity_color.method = PRO_COLOR_METHOD_TYPE;
	status = ProDtlentitydataColorSet(edata, &entity_color);
	status = ProDtlentitydataWidthSet(edata, 0);
	status = ProDtlentitydataFontSet(edata, Font);

	status = ProDtlentityCreate(Drawing, NULL, edata, &entity);

	status = ProDtlentitydataFree(edata);
}

void InsertCrossLine()
{
	ProError status;
	ProSelection *SelBuffer = NULL;
	ProSelection Sel = NULL;
	ProMdl mdl;
	ProDrawing drawing;
	ProView view;
	int size, options = 0;

	ProModelitem Modelitem;
	ProGeomitemdata *geomitem_data = NULL;

	ProVector vector1, vector2;
	double p_start_angle, p_end_angle, p_radius;

	ProPoint3d centerinCsys, centerinDrawing;
	ProMatrix transform;

	ProMouseButton expected_button = (ProMouseButton)(PRO_LEFT_BUTTON | PRO_MIDDLE_BUTTON);
	ProMouseButton button_pressed;
	ProColor Gray, OrigColor;
	ProPoint3d positionmouse, positionto[4];

	status = ProMdlCurrentGet(&mdl);
	drawing = (ProDrawing)mdl;

	status = ProSelect((char *)"edge", 1, NULL, NULL, NULL, NULL, &SelBuffer, &size);

	if (status != PRO_TK_NO_ERROR || size < 1)
		return;

	status = ProSelectionModelitemGet(SelBuffer[0], &Modelitem);
	if (status != PRO_TK_NO_ERROR)
		return;

	status = ProGeomitemdataGet(&Modelitem, &geomitem_data);
	if (status != PRO_TK_NO_ERROR)
		return;

	status = ProArcdataGet(geomitem_data->data.p_curve_data, vector1, vector2, centerinCsys, &p_start_angle, &p_end_angle, &p_radius);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProSelectionViewGet(SelBuffer[0], &view);
		status = ProDrawingViewTransformGet(drawing, view, PRO_B_TRUE, transform);
		status = ProPntTrfEval(centerinCsys, transform, centerinDrawing);

		Gray.method = PRO_COLOR_METHOD_RGB;
		Gray.value.map.red = 0.5;
		Gray.value.map.green = 0.5;
		Gray.value.map.blue = 0.5;

		status = ProGraphicsColorModify(&Gray, &OrigColor);
		while (1)
		{
			status = ProMouseTrack(options, &button_pressed, positionmouse);

			int wid = 0;
			status = ProWindowCurrentGet(&wid);
			status = ProWindowRefresh(wid);

			if (button_pressed == PRO_LEFT_BUTTON)
			{
				break;
			}

			if (button_pressed == PRO_MIDDLE_BUTTON)
			{
				status = ProGeomitemdataFree(&geomitem_data);
				status = ProWindowRepaint(PRO_VALUE_UNUSED);
				return;
			}

			positionto[0][0] = positionmouse[0];
			positionto[0][1] = centerinDrawing[1];
			positionto[0][2] = positionmouse[2];

			positionto[1][0] = centerinDrawing[0] - (positionmouse[0] - centerinDrawing[0]);
			positionto[1][1] = centerinDrawing[1];
			positionto[1][2] = positionmouse[2];

			ProGraphicsPenPosition(positionto[0]);
			ProGraphicsLineDraw(positionto[1]);

			positionto[2][0] = centerinDrawing[0];
			positionto[2][1] = positionmouse[1];
			positionto[2][2] = positionmouse[2];

			positionto[3][0] = centerinDrawing[0];
			positionto[3][1] = centerinDrawing[1] - (positionmouse[1] - centerinDrawing[1]);
			positionto[3][2] = positionmouse[2];

			ProGraphicsPenPosition(positionto[2]);
			ProGraphicsLineDraw(positionto[3]);
		}
		status = ProGraphicsColorModify(&OrigColor, &Gray);
		DrawLine(drawing, view, positionto[0], positionto[1], PRO_COLOR_LETTER, L"CTRLFONT");
		DrawLine(drawing, view, positionto[2], positionto[3], PRO_COLOR_LETTER, L"CTRLFONT");
	}

	status = ProGeomitemdataFree(&geomitem_data);
	status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId AuxiliaryLineID;

	status = ProMenubarMenuAdd("AuxiliaryLine", "AuxiliaryLine", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("AuxiliaryLine", "AuxiliaryLine", "AuxiliaryLine", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("AuxiliaryLine1_Act", (uiCmdCmdActFn)InsertCrossLine, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &AuxiliaryLineID);
	status = ProMenubarmenuPushbuttonAdd("AuxiliaryLine", "AuxiliaryLinemenu", "AuxiliaryLinemenu", "AuxiliaryLinemenutips", NULL, PRO_B_TRUE, AuxiliaryLineID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}