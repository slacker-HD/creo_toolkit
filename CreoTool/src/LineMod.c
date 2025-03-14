#include "./includes/LineMod.h"

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

// 计算两点间的距离
double distance(ProPoint3d p1, ProPoint3d p2)
{
    return sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]));
}
// 计算线段中点
void midpoint(ProPoint3d start, ProPoint3d end, ProPoint3d mid)
{
    mid[0] = (start[0] + end[0]) / 2;
    mid[1] = (start[1] + end[1]) / 2;
    mid[2] = 0.0; // 保留 z 坐标
}
// 计算一个点关于另一个点的镜像点
void mirror_point(ProPoint3d point, ProPoint3d center, ProPoint3d result)
{
    result[0] = 2 * center[0] - point[0];
    result[1] = 2 * center[1] - point[1];
    result[2] = 0.0; // 保留 z 坐标
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


// 计算沿 x 轴方向的投影点
void calculate_projection_x(ProPoint3d start, ProPoint3d end, ProPoint3d third, ProPoint3d point_on_x)
{
    if (start[0] == end[0])
    {
        point_on_x[0] = start[0];
        point_on_x[1] = third[1];
        point_on_x[2] = 0.0; // 保留 z 坐标
    }
    else
    {
        double slope = (end[1] - start[1]) / (end[0] - start[0]);
        point_on_x[0] = third[0];
        point_on_x[1] = start[1] + slope * (third[0] - start[0]);
        point_on_x[2] = 0.0; // 保留 z 坐标
    }
}

// 计算沿 y 轴方向的投影点
void calculate_projection_y(ProPoint3d start, ProPoint3d end, ProPoint3d third, ProPoint3d point_on_y)
{
    if (start[1] == end[1])
    {
        point_on_y[0] = third[0];
        point_on_y[1] = start[1];
        point_on_y[2] = 0.0; // 保留 z 坐标
    }
    else
    {
        double slope = (end[0] - start[0]) / (end[1] - start[1]);
        point_on_y[1] = third[1];
        point_on_y[0] = start[0] + slope * (third[1] - start[1]);
        point_on_y[2] = 0.0; // 保留 z 坐标
    }
}

// 计算并更新线段起点和终点
void calculate_and_update(ProPoint3d start, ProPoint3d end, ProPoint3d third, int bidirectional)
{
    ProPoint3d point_on_x;
    ProPoint3d point_on_y;
    double dist_x;
    double dist_y;
    ProPoint3d closest_point;
    double dist_to_start;
    double dist_to_end;
    ProPoint3d mid;
    ProPoint3d mirror;
    int moved_point = 0;

    calculate_projection_x(start, end, third, point_on_x);
    calculate_projection_y(start, end, third, point_on_y);

    // 计算距离
    dist_x = distance(third, point_on_x);
    dist_y = distance(third, point_on_y);

    if (dist_x < dist_y)
    {
        closest_point[0] = point_on_x[0];
        closest_point[1] = point_on_x[1];
        closest_point[2] = 0.0; // 保留 z 坐标
    }
    else
    {
        closest_point[0] = point_on_y[0];
        closest_point[1] = point_on_y[1];
        closest_point[2] = 0.0; // 保留 z 坐标
    }

    // 判断更短的点距离起点还是终点更近
    dist_to_start = distance(closest_point, start);
    dist_to_end = distance(closest_point, end);
    midpoint(start, end, mid);

    if (dist_to_start < dist_to_end)
    {
        moved_point = 1;
        start[0] = closest_point[0];
        start[1] = closest_point[1];
        start[2] = 0.0; // 保留 z 坐标
        if (bidirectional)
        {
            mirror_point(closest_point, mid, mirror);
            end[0] = mirror[0];
            end[1] = mirror[1];
            end[2] = 0.0; // 保留 z 坐标
        }
    }
    else
    {
        moved_point = 2;
        end[0] = closest_point[0];
        end[1] = closest_point[1];
        end[2] = 0.0; // 保留 z 坐标
        if (bidirectional)
        {
            mirror_point(closest_point, mid, mirror);
            start[0] = mirror[0];
            start[1] = mirror[1];
            start[2] = 0.0; // 保留 z 坐标
        }
    }
}

void ModLine(AlignType align_type)
{
    ProError status;
    ProMouseButton button_pressed;
    ProMdl mdl;
    ProDrawing drawing;
    int size, options = 0, wid = 0;
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
    if (status != PRO_TK_NO_ERROR || modelitem.type != PRO_DRAFT_ENTITY)
        return;

    status = ProDtlentityDataGet(&modelitem, NULL, &entdata);
    status = ProDtlentitydataCurveGet(entdata, &curvedata);

    while (1)
    {
        status = ProMouseTrack(options, &button_pressed, positionmouse);
        if (button_pressed == PRO_LEFT_BUTTON || button_pressed == PRO_RIGHT_BUTTON || button_pressed == PRO_MIDDLE_BUTTON)
        {
            status = ProWindowRepaint(PRO_VALUE_UNUSED);
            break;
        }

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
    }
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
