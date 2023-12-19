#include "./includes/TableCellFormat.h"

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