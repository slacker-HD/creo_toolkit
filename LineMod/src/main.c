#include "./includes/main.h"


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

ProError ProDrawResizeBar(ProCurvedata CurveData)
{
    ProError status;
    ProGraphicsPenPosition(CurveData.line.end1);
    ProGraphicsCircleDraw(CurveData.line.end1, 5);
    ProGraphicsPenPosition(CurveData.line.end2);
    ProGraphicsCircleDraw(CurveData.line.end2, 5);
    return PRO_TK_NO_ERROR;
}

void ExtendLine(int bidirectional)
{
    ProError status;
    ProMouseButton button_pressed;
    ProMdl mdl;
    ProDrawing drawing;
    int size, options = 0;
    int wid = 0;
    ProSelection *sel = NULL;
    ProModelitem modelitem;
    ProDtlentitydata entdata;
    ProCurvedata curvedata;
    ProPoint3d positionmouse;

    status = ProMdlCurrentGet(&mdl);
    drawing = (ProDrawing)mdl;
    status = ProSelect((char *)"draft_ent", 1, NULL, NULL, NULL, NULL, &sel, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
        return;

    status = ProSelectionModelitemGet(sel[0], &modelitem);
    if (status != PRO_TK_NO_ERROR)
        return;

    if (modelitem.type != PRO_DRAFT_ENTITY)
        return;

    status = ProDtlentityDataGet(&modelitem, NULL, &entdata);
    status = ProDtlentitydataCurveGet(entdata, &curvedata);

    while (1)
    {
        status = ProDrawResizeBar(curvedata);
        status = ProMouseTrack(options, &button_pressed, positionmouse);
        // 按任意鼠标键退出，注意PRO_ANY_BUTTON是个坑
        if (button_pressed == PRO_LEFT_BUTTON || button_pressed == PRO_RIGHT_BUTTON || button_pressed == PRO_MIDDLE_BUTTON)
        {
            status = ProWindowRepaint(PRO_VALUE_UNUSED);
            break;
        }

        // 这里要计算了
        calculate_and_update(curvedata.line.end1, curvedata.line.end2, positionmouse, bidirectional);
        // 实时更改线段
        status = ProDtlentitydataCurveSet(entdata, &curvedata);
        status = ProDtlentityModify(&modelitem, NULL, entdata);
        status = ProWindowCurrentGet(&wid);
        status = ProWindowRefresh(wid);
    }
    status = ProDtlentitydataFree(entdata);
    status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

void UnidirectionalExtend()
{
    ExtendLine(0);
}

void BidirectionalExtend()
{
    ExtendLine(1);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_BidirectionalExtendLineMenuID, IMI_UnidirectionalExtendLineMenuID;

    status = ProMenubarMenuAdd("IMI_LineModmenu", "IMI_LineModmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BidirectionalExtendLine_Act", (uiCmdCmdActFn)BidirectionalExtend, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BidirectionalExtendLineMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_BidirectionalExtendLineMenu", "IMI_BidirectionalExtendLineMenu", "IMI_BidirectionalExtendLineMenutips", NULL, PRO_B_TRUE, IMI_BidirectionalExtendLineMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_UnidirectionalExtendLine_Act", (uiCmdCmdActFn)UnidirectionalExtend, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_UnidirectionalExtendLineMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_UniirectionalExtendLineMenu", "IMI_UnidirectionalExtendLineMenu", "IMI_UnidirectionalExtendLineMenutips", NULL, PRO_B_TRUE, IMI_UnidirectionalExtendLineMenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}