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

void AlignText(ProVerticalJustification VerticalJustification, ProTextHrzJustification HrzJustification)
{
    ProError status;
    ProSelection *tableSels = NULL;
    int size;
    int table_segment, row, column;
    ProDwgtable table;
    ProDtlnote note;
    ProDtlnotedata note_data;

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectTableCell");
    status = ProSelect((char *)"table_cell", 1, NULL, NULL, NULL, NULL, &tableSels, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
    {
        return;
    }
    status = ProArraySizeGet(tableSels, &size);
    if (status != PRO_TK_NO_ERROR || size <= 0)
    {
        return;
    }
    status = ProSelectionDwgtblcellGet(tableSels[0], &table_segment, &row, &column);
    if (status != PRO_TK_NO_ERROR)
    {
        return;
    }
    status = ProSelectionDwgtableGet(tableSels[0], &table);
    if (status != PRO_TK_NO_ERROR)
    {
        return;
    }

    status = ProDwgtableCellNoteGet(&table, column + 1, row + 1, &note);
    status = ProDtlnoteDataGet(&note, NULL, PRODISPMODE_NUMERIC, &note_data);
    status = ProDtlnotedataJustifSet(note_data, HrzJustification, VerticalJustification);
    status = ProDtlnoteModify(&note, NULL, note_data);
    status = ProDtlnotedataFree(note_data);
    status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ;~ Command `ProCmdWinActivate`;");
}

void AlignTopLeft()
{
    AlignText(PRO_VERTJUST_TOP, PRO_HORZJUST_LEFT);
}

void AlignTopCenter()
{
    AlignText(PRO_VERTJUST_TOP, PRO_HORZJUST_CENTER);
}

void AlignTopRight()
{
    AlignText(PRO_VERTJUST_TOP, PRO_HORZJUST_RIGHT);
}

void AlignMiddleLeft()
{
    AlignText(PRO_VERTJUST_MIDDLE, PRO_HORZJUST_LEFT);
}

void AlignMiddleCenter()
{
    AlignText(PRO_VERTJUST_MIDDLE, PRO_HORZJUST_CENTER);
}

void AlignMiddleRight()
{
    AlignText(PRO_VERTJUST_MIDDLE, PRO_HORZJUST_RIGHT);
}

void AlignBottomLeft()
{
    AlignText(PRO_VERTJUST_BOTTOM, PRO_HORZJUST_LEFT);
}

void AlignBottomCenter()
{
    AlignText(PRO_VERTJUST_BOTTOM, PRO_HORZJUST_CENTER);
}

void AlignBottomRight()
{
    AlignText(PRO_VERTJUST_BOTTOM, PRO_HORZJUST_RIGHT);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_NoteAlignTopLeftmenuID, IMI_NoteAlignTopCentermenuID, IMI_NoteAlignTopRightmenuID, IMI_NoteAlignMiddleLeftmenuID, IMI_NoteAlignMiddleCentermenuID, IMI_NoteAlignMiddleRightmenuID, IMI_NoteAlignBottomLeftmenuID, IMI_NoteAlignBottomCentermenuID, IMI_NoteAlignBottomRightmenuID;

    status = ProMenubarMenuAdd("IMI_NoteTextAlignmenu", "IMI_NoteTextAlignmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignTopLeft_Act", (uiCmdCmdActFn)AlignTopLeft, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignTopLeftmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignTopLeftmenu", "IMI_NoteAlignTopLeftmenu", "IMI_NoteAlignTopLeftmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignTopLeftmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignTopCenter_Act", (uiCmdCmdActFn)AlignTopCenter, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignTopCentermenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignTopCentermenu", "IMI_NoteAlignTopCentermenu", "IMI_NoteAlignTopCentermenutips", NULL, PRO_B_TRUE, IMI_NoteAlignTopCentermenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignTopRight_Act", (uiCmdCmdActFn)AlignTopRight, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignTopRightmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignTopRightmenu", "IMI_NoteAlignTopRightmenu", "IMI_NoteAlignTopRightmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignTopRightmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignMiddleLeft_Act", (uiCmdCmdActFn)AlignMiddleLeft, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignMiddleLeftmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignMiddleLeftmenu", "IMI_NoteAlignMiddleLeftmenu", "IMI_NoteAlignMiddleLeftmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignMiddleLeftmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignMiddleCenter_Act", (uiCmdCmdActFn)AlignMiddleCenter, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignMiddleCentermenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignMiddleCentermenu", "IMI_NoteAlignMiddleCentermenu", "IMI_NoteAlignMiddleCentermenutips", NULL, PRO_B_TRUE, IMI_NoteAlignMiddleCentermenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignMiddleRight_Act", (uiCmdCmdActFn)AlignMiddleRight, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignMiddleRightmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignMiddleRightmenu", "IMI_NoteAlignMiddleRightmenu", "IMI_NoteAlignMiddleRightmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignMiddleRightmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignBottomLeft_Act", (uiCmdCmdActFn)AlignBottomLeft, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignBottomLeftmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignBottomLeftmenu", "IMI_NoteAlignBottomLeftmenu", "IMI_NoteAlignBottomLeftmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignBottomLeftmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignBottomCenter_Act", (uiCmdCmdActFn)AlignBottomCenter, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignBottomCentermenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignBottomCentermenu", "IMI_NoteAlignBottomCentermenu", "IMI_NoteAlignBottomCentermenutips", NULL, PRO_B_TRUE, IMI_NoteAlignBottomCentermenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_NoteAlignBottomRight_Act", (uiCmdCmdActFn)AlignBottomRight, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_NoteAlignBottomRightmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_NoteTextAlignmenu", "IMI_NoteAlignBottomRightmenu", "IMI_NoteAlignBottomRightmenu", "IMI_NoteAlignBottomRightmenutips", NULL, PRO_B_TRUE, IMI_NoteAlignBottomRightmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
