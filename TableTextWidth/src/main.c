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

void FormatText()
{
    ProError status;
    ProSelection *tableSels = NULL;
    int size;
    int table_segment, row, column;
    double columnwidth;
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

    status = ProDwgtableColumnSizeGet(&table, table_segment, column, &columnwidth);
    status = ProDwgtableCellNoteGet(&table, column + 1, row + 1, &note);
    status = ProNoteTextStyleGet(&note, &textStyle);
    // 根据字体计算宽度因子

    status = ProTextStyleWidthSet(textStyle, 0.6);
    status = ProNoteTextStyleSet(&note, textStyle);
    status = ProTextStyleFree(&textStyle);

    status = ProDwgtableCelltextWrap(&table, row + 1, column + 1);
    status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel` ;~ Command `ProCmdWinActivate`;");
}

// void FormatText()
// {
//     ProError status;
//     ProSelection *tableSels = NULL;
//     int size;
//     int table_segment, row, column;
//     double columnwidth;
//     ProDwgtable table;
//     ProDtlnote note;
//     ProDtlnotedata notedata;
//     ProDtlnoteline *p_lines;
//     ProDtlnotetext *p_texts;
//     int n_lines, n_texts;
//     double textwidth;

//     status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectTableCell");
//     status = ProSelect((char *)"table_cell", 1, NULL, NULL, NULL, NULL, &tableSels, &size);
//     if (status != PRO_TK_NO_ERROR || size < 1)
//     {
//         return;
//     }

//     status = ProArraySizeGet(tableSels, &size);
//     if (status != PRO_TK_NO_ERROR || size <= 0)
//     {
//         return;
//     }
//     status = ProSelectionDwgtblcellGet(tableSels[0], &table_segment, &row, &column);
//     if (status != PRO_TK_NO_ERROR)
//     {
//         return;
//     }
//     status = ProSelectionDwgtableGet(tableSels[0], &table);
//     if (status != PRO_TK_NO_ERROR)
//     {
//         return;
//     }

//     status = ProDwgtableColumnSizeGet(&table, table_segment, column, &columnwidth);
//     status = ProDwgtableCellNoteGet(&table, column + 1, row + 1, &note);
//     status = ProDtlnoteDataGet(&note, NULL, PRODISPMODE_NUMERIC, &notedata);
//     status = ProDtlnotedataLinesCollect(notedata, &p_lines);

//     if (status == PRO_TK_NO_ERROR)
//     {
//         status = ProArraySizeGet((ProArray)p_lines, &n_lines);
//         status = ProDtlnotelineTextsCollect(p_lines[0], &p_texts);

//         status = ProArraySizeGet((ProArray)p_lines, &n_texts);
//         if (status == PRO_TK_NO_ERROR)
//         {
//             status = ProDtlnotetextWidthGet(p_texts[0], &textwidth);
//             // if (columnwidth < CellText.GetLength() * 2.753)
//             // {
//             status = ProDtlnotetextWidthSet(p_texts[0], -1.0); // columnwidth/CellText.GetLength()*0.85/2.753);
//             status = ProDtlnotetextHeightSet(p_texts[0], 5.5); // columnwidth/CellText.GetLength()*0.85/2.753);

//             // }
//             // 3.5的字符，100宽度为3.24每个字符
//             //
//             status = ProDtlnotelineTextsSet(p_lines[0], p_texts);

//             // status = ProArrayFree((ProArray *)&p_texts);
//         }
//         status = ProDtlnoteldataLinesSet(notedata, p_lines);

//         ProTextStyleWidthSet
//         // status = ProDtlnotedataFree(notedata);
//     }
//     // status = ProArrayFree((ProArray *)&p_lines);

//     ProTestDtlNoteTextModify(notedata);
// }

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_FormatTextmenuID;

    status = ProMenubarMenuAdd("IMI_TableTextWidthmenu", "IMI_TableTextWidthmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitInterPret_Act", (uiCmdCmdActFn)FormatText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_FormatTextmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_TableTextWidthmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenutips", NULL, PRO_B_TRUE, IMI_FormatTextmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
