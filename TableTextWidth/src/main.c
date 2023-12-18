#include "./includes/main.h"

#define NOTE_FONT 0
#define NOTE_ULINE 1
#define NOTE_THICKNESS 2
#define NOTE_SLANT 3
#define NOTE_WIDTH 4
#define NOTE_HEIGHT 5
#define NOTE_STRING 6

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

void WrapText()
{
    ProError status;
    ProSelection *tableSels = NULL;
    int size;
    int table_segment, row, column;
    ProDwgtable table;

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
    status = ProDwgtableCelltextWrap(&table, row + 1, column + 1);
    status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ;~ Command `ProCmdWinActivate`;");
}

void setTextWidth(double scale)
{
    ProError status;
    ProSelection *tableSels = NULL;
    int size;
    int table_segment, row, column;
    double width;
    ProDwgtable table;
    ProDtlnote note;
    ProTextStyle textStyle;
    double textwidth;

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
    status = ProNoteTextStyleGet(&note, &textStyle);

    status = ProTextStyleWidthGet(textStyle, &width);
    status = ProTextStyleWidthSet(textStyle, width + scale);
    status = ProNoteTextStyleSet(&note, textStyle);
    status = ProTextStyleFree(&textStyle);
    status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ;~ Command `ProCmdWinActivate`;");
}

void IncreaseWidth()
{
    setTextWidth(0.05);
}
void DecreaseWidth()
{
    setTextWidth(-0.05);
}
int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_TextWrapmenuID, IMI_TextWidthIncmenuID, IMI_TextWidthDecmenuID;

    status = ProMenubarMenuAdd("IMI_TableTextWidthmenu", "IMI_TableTextWidthmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_TextWrap_Act", (uiCmdCmdActFn)WrapText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_TextWrapmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_TableTextWidthmenu", "IMI_TextWrapmenu", "IMI_TextWrapmenu", "IMI_TextWrapmenutips", NULL, PRO_B_TRUE, IMI_TextWrapmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_TextWidthInc_Act", (uiCmdCmdActFn)IncreaseWidth, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_TextWidthIncmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_TableTextWidthmenu", "IMI_TextWidthIncmenu", "IMI_TextWidthIncmenu", "IMI_TextWidthIncmenutips", NULL, PRO_B_TRUE, IMI_TextWidthIncmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_TextWidthDec_Act", (uiCmdCmdActFn)DecreaseWidth, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_TextWidthDecmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_TableTextWidthmenu", "IMI_TextWidthDecmenu", "IMI_TextWidthDecmenu", "IMI_TextWidthDecmenutips", NULL, PRO_B_TRUE, IMI_TextWidthDecmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
