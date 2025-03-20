#include "./includes/main.h"
#define EPSILON 1e-6

typedef enum
{
    ALIGN_HORIZONTAL_FROM_TOP,
    ALIGN_HORIZONTAL_FROM_MID,
    ALIGN_HORIZONTAL_FROM_BOTTOM,
    ALIGN_VERTICAL_FROM_LEFT,
    ALIGN_VERTICAL_FROM_MID,
    ALIGN_VERTICAL_FROM_RIGHT
} AlignType;

// 计算线段中点
void midpoint(ProPoint3d start, ProPoint3d end, ProPoint3d mid)
{
    mid[0] = (start[0] + end[0]) / 2;
    mid[1] = (start[1] + end[1]) / 2;
    mid[2] = 0.0; // 保留 z 坐标
}

// 计算高点和低点
void calculate_top_bottom(ProPoint3d *start, ProPoint3d *end, ProPoint3d **top, ProPoint3d **bottom)
{
    if ((*start)[1] > (*end)[1])
    {
        *top = start;
        *bottom = end;
    }
    else
    {
        *top = end;
        *bottom = start;
    }
}

// 计算左侧点和右侧点
void calculate_left_right(ProPoint3d *start, ProPoint3d *end, ProPoint3d **left, ProPoint3d **right)
{
    if ((*start)[0] < (*end)[0])
    {
        *left = start;
        *right = end;
    }
    else
    {
        *left = end;
        *right = start;
    }
}

// 将线段在X轴沿中点水平对齐
void align_horizontal_from_mid(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d mid;

    if (fabs((*start)[1] - (*end)[1]) < EPSILON)
        return; // 如果线段是水平的，则不进行对齐

    midpoint(*start, *end, mid);
    (*start)[1] = mid[1];
    (*end)[1] = mid[1];
}

// 将线段在X轴沿高点水平对齐
void align_horizontal_from_top(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d *top, *bottom;
    calculate_top_bottom(start, end, &top, &bottom);

    if (fabs((*top)[1] - (*bottom)[1]) < EPSILON)
        return; // 如果线段是水平的，则不进行对齐

    (*bottom)[1] = (*top)[1];
}

// 将线段在X轴沿低点水平对齐
void align_horizontal_from_bottom(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d *top, *bottom;
    calculate_top_bottom(start, end, &top, &bottom);

    if (fabs((*top)[1] - (*bottom)[1]) < EPSILON)
        return; // 如果线段是水平的，则不进行对齐

    (*top)[1] = (*bottom)[1];
}

// 将线段在Y轴沿中点垂直对齐
void align_vertical_from_mid(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d mid;

    if (fabs((*start)[0] - (*end)[0]) < EPSILON)
        return; // 如果线段是垂直的，则不进行对齐

    midpoint(*start, *end, mid);
    (*start)[0] = mid[0];
    (*end)[0] = mid[0];
}

// 将线段在Y轴沿左侧点垂直对齐
void align_vertical_from_left(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d *left, *right;
    calculate_left_right(start, end, &left, &right);

    if (fabs((*left)[0] - (*right)[0]) < EPSILON)
        return; // 如果线段是垂直的，则不进行对齐

    (*right)[0] = (*left)[0];
}

// 将线段在Y轴沿右侧点垂直对齐
void align_vertical_from_right(ProPoint3d *start, ProPoint3d *end)
{
    ProPoint3d *left, *right;
    calculate_left_right(start, end, &left, &right);

    if (fabs((*left)[0] - (*right)[0]) < EPSILON)
        return; // 如果线段是垂直的，则不进行对齐

    (*left)[0] = (*right)[0];
}

int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdltype;
    ProError status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;

    status = ProMdlTypeGet(mdl, &mdltype);
    return (status == PRO_TK_NO_ERROR) ? mdltype : -1;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
    return (CurrentMdlType() == PRO_DRAWING) ? ACCESS_AVAILABLE : ACCESS_INVISIBLE;
}

void ModLine(AlignType align_type)
{
    ProError status;
    ProMdl mdl;
    ProDrawing drawing;
    int size, options = 0, wid = 0;
    ProSelection *sel = NULL;
    ProModelitem modelitem;
    ProDtlentitydata entdata;
    ProCurvedata curvedata;

    status = ProMdlCurrentGet(&mdl);
    drawing = (ProDrawing)mdl;
    status = ProSelect((char *)"draft_ent", 1, NULL, NULL, NULL, NULL, &sel, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
        return;

    status = ProSelectionModelitemGet(sel[0], &modelitem);
    if (status != PRO_TK_NO_ERROR || modelitem.type != PRO_DRAFT_ENTITY)
        return;

    status = ProDtlentityDataGet(&modelitem, NULL, &entdata);
    status = ProDtlentitydataCurveGet(entdata, &curvedata);

    if (status != PRO_TK_NO_ERROR || curvedata.line.type != 2)
        return;

    switch (align_type)
    {
    case ALIGN_HORIZONTAL_FROM_TOP:
        align_horizontal_from_top(&curvedata.line.end1, &curvedata.line.end2);
        break;
    case ALIGN_HORIZONTAL_FROM_MID:
        align_horizontal_from_mid(&curvedata.line.end1, &curvedata.line.end2);
        break;
    case ALIGN_HORIZONTAL_FROM_BOTTOM:
        align_horizontal_from_bottom(&curvedata.line.end1, &curvedata.line.end2);
        break;
    case ALIGN_VERTICAL_FROM_LEFT:
        align_vertical_from_left(&curvedata.line.end1, &curvedata.line.end2);
        break;
    case ALIGN_VERTICAL_FROM_MID:
        align_vertical_from_mid(&curvedata.line.end1, &curvedata.line.end2);
        break;
    case ALIGN_VERTICAL_FROM_RIGHT:
        align_vertical_from_right(&curvedata.line.end1, &curvedata.line.end2);
        break;
    }

    status = ProDtlentitydataCurveSet(entdata, &curvedata);
    status = ProDtlentityModify(&modelitem, NULL, entdata);
    status = ProWindowCurrentGet(&wid);
    status = ProWindowRefresh(wid);

    status = ProDtlentitydataFree(entdata);
    status = ProWindowRepaint(PRO_VALUE_UNUSED);
}

void HorizontalLineFromTop()
{
    ModLine(ALIGN_HORIZONTAL_FROM_TOP);
}

void HorizontalLineFromMid()
{
    ModLine(ALIGN_HORIZONTAL_FROM_MID);
}

void HorizontalLineFromBottom()
{
    ModLine(ALIGN_HORIZONTAL_FROM_BOTTOM);
}

void VerticalLineFromLeft()
{
    ModLine(ALIGN_VERTICAL_FROM_LEFT);
}

void VerticalLineFromMid()
{
    ModLine(ALIGN_VERTICAL_FROM_MID);
}

void VerticalLineFromRight()
{
    ModLine(ALIGN_VERTICAL_FROM_RIGHT);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_HorizontalLineFromTopMenuID, IMI_HorizontalLineFromMidMenuID, IMI_HorizontalLineFromBottomMenuID;
    uiCmdCmdId IMI_VerticalLineFromLeftMenuID, IMI_VerticalLineFromMidMenuID, IMI_VerticalLineFromRightMenuID;

    status = ProMenubarMenuAdd("IMI_LineModmenu", "IMI_LineModmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_HorizontalLineFromTop_Act", (uiCmdCmdActFn)HorizontalLineFromTop, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_HorizontalLineFromTopMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_HorizontalLineFromTopMenu", "IMI_HorizontalLineFromTopMenu", "IMI_HorizontalLineFromTopMenutips", NULL, PRO_B_TRUE, IMI_HorizontalLineFromTopMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_HorizontalLineFromMid_Act", (uiCmdCmdActFn)HorizontalLineFromMid, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_HorizontalLineFromMidMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_HorizontalLineFromMidMenu", "IMI_HorizontalLineFromMidMenu", "IMI_HorizontalLineFromMidMenutips", NULL, PRO_B_TRUE, IMI_HorizontalLineFromMidMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_HorizontalLineFromBottom_Act", (uiCmdCmdActFn)HorizontalLineFromBottom, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_HorizontalLineFromBottomMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_HorizontalLineFromBottomMenu", "IMI_HorizontalLineFromBottomMenu", "IMI_HorizontalLineFromBottomMenutips", NULL, PRO_B_TRUE, IMI_HorizontalLineFromBottomMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_VerticalLineFromLeft_Act", (uiCmdCmdActFn)VerticalLineFromLeft, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_VerticalLineFromLeftMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_VerticalLineFromLeftMenu", "IMI_VerticalLineFromLeftMenu", "IMI_VerticalLineFromLeftMenutips", NULL, PRO_B_TRUE, IMI_VerticalLineFromLeftMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_VerticalLineFromMid_Act", (uiCmdCmdActFn)VerticalLineFromMid, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_VerticalLineFromMidMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_VerticalLineFromMidMenu", "IMI_VerticalLineFromMidMenu", "IMI_VerticalLineFromMidMenutips", NULL, PRO_B_TRUE, IMI_VerticalLineFromMidMenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_VerticalLineFromRight_Act", (uiCmdCmdActFn)VerticalLineFromRight, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_VerticalLineFromRightMenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_LineModmenu", "IMI_VerticalLineFromRightMenu", "IMI_VerticalLineFromRightMenu", "IMI_VerticalLineFromRightMenutips", NULL, PRO_B_TRUE, IMI_VerticalLineFromRightMenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}