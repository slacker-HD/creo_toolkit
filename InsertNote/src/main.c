#include "./includes/main.h"
#define QRCODE_MAX_LENGTH 128

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

/// @brief 获取默认字高
/// @param mdl 当前的绘图，模型(in)
/// @param height 字高（out）
/// @return 函数是否成功
ProError ProUsrGetTextDefaultHeight(ProMdl mdl, double *height)
{
    ProError status;

    int cur_sheet;
    ProName w_size;
    ProMatrix matrix;

    status = ProSolidDefaulttextheightGet((ProSolid)mdl, height);

    if (status != PRO_TK_NO_ERROR)
        return status;

    status = ProDrawingCurrentSheetGet(mdl, &cur_sheet);
    status = ProDrawingSheetTrfGet(mdl, cur_sheet, w_size, matrix);
    if (status != PRO_TK_NO_ERROR)
        return status;

    *height /= matrix[0][0];
    return PRO_TK_NO_ERROR;
}

/// @brief 设定dtlnote的字体样式
/// @param mdl 当前绘图
/// @param text 需要设定的dtlnote
/// @param height 字高
/// @param width 字宽
/// @param slant 倾斜角度
/// @param thickness 厚度
/// @param wtext 内容
/// @param
/// @return 函数是否成功
ProError ProUsrSetDtlText(ProMdl mdl, ProDtlnotetext *text, double height, double width, double slant, double thickness, wchar_t *wtext)
{
    ProError status;
    // 设定一行的字体等
    status = ProUsrGetTextDefaultHeight(mdl, &height);
    status = ProDtlnotetextWidthSet(*text, width);
    status = ProDtlnotetextSlantSet(*text, slant);
    status = ProDtlnotetextThicknessSet(*text, thickness);
    status = ProDtlnotetextStringSet(*text, wtext);
    return status;
}

/// @brief 设定自由设定放置的note
/// @param notedata dtlnote
/// @param attachment 放置的方式,申请了内存记得释放
/// @return 函数是否成功
ProError ProUsrSetFreeAttatchment(ProDtlnotedata *notedata, ProDtlattach *attachment)
{
    ProError status;
    ProMouseButton mouse_button;
    ProPoint3d pos;
    status = ProMousePickGet(PRO_LEFT_BUTTON, &mouse_button, pos);
    if (status != PRO_TK_NO_ERROR)
        return status;
    // 设定摆放方式。这里是FREE，定位在之前的鼠标点击位置
    status = ProDtlattachAlloc(PRO_DTLATTACHTYPE_FREE, NULL, pos, NULL, attachment);
    status = ProDtlnotedataAttachmentSet(*notedata, *attachment);

    return status;
}

/// @brief 设定notedata的颜色
/// @param notedata notedata
/// @return 函数是否成功
ProError ProUsrSetDefaultColor(ProDtlnotedata *notedata)
{
    ProError status;
    ProColor color;
    color.method = PRO_COLOR_METHOD_TYPE;
    color.value.type = PRO_COLOR_LETTER;
    status = ProDtlnotedataColorSet(*notedata, &color);
    return status;
}

ProError ProUsrNoteCreate()
{
    ProError status;
    ProMdl mdl;
    ProDtlnote note;
    ProDtlnotedata notedata;
    ProDtlnoteline line;
    ProDtlnotetext text;
    ProLine wtext;
    double height;
    ProColor color;
    ProDtlattach attachment;

    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return status;

    status = ProMessageStringRead(PRO_LINE_SIZE, wtext);
    if (status != PRO_TK_NO_ERROR)
        return status;

    // 一个notedata有多个line，每个line对应多个text（行）,每个都要先申请内存，函数结束后释放
    status = ProDtlnotedataAlloc(mdl, &notedata);
    status = ProDtlnotelineAlloc(&line);
    status = ProDtlnotetextAlloc(&text);

    status = ProUsrSetFreeAttatchment(&notedata, &attachment);
    if (status == PRO_TK_NO_ERROR)
    {
        // 设定字体等
        status = ProUsrGetTextDefaultHeight(mdl, &height);
        status = ProUsrSetDtlText(mdl, &text, height, 0.8, 0, 0, wtext);
        // line加一行，可以加多次代表多行
        status = ProDtlnotelineTextAdd(line, text);
        // line加入notedata
        status = ProDtlnotedataLineAdd(notedata, line);
        // 设定默认颜色
        status = ProUsrSetDefaultColor(&notedata);
        // 创建并显示
        status = ProDtlnoteCreate(mdl, NULL, notedata, &note);
        status = ProDtlnoteShow(&note);
    }

    // 申请的内存必须得释放
    status = ProDtlnotedataFree(notedata);
    status = ProDtlnotetextFree(text);
    status = ProDtlnotelineFree(line);
    status = ProDtlattachFree(attachment);

    return status;
}

void SetInsertNote()
{
    ProError status;
    status = ProUsrNoteCreate();
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_InsertNotemenuID;

    status = ProMenubarMenuAdd("IMI_NoteInsertmenu", "IMI_NoteInsertmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_InsertNote_Act", (uiCmdCmdActFn)SetInsertNote, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_InsertNotemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteInsertmenu", "IMI_InsertNotemenu", "IMI_InsertNotemenu", "IMI_InsertNotemenutips", NULL, PRO_B_TRUE, IMI_InsertNotemenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
