#include "./includes/TableCellFormat.h"

void WrapText()
{
    ProError status;
    ProSelection *tableSels = NULL;
    ProSelection *SelBuffer = NULL;
    int size;
    int tableSegment, row, column;
    ProDwgtable table;

    status = ProSelbufferSelectionsGet(&SelBuffer);
    status = ProArraySizeGet(SelBuffer, &size);
    if (status == PRO_TK_NO_ERROR && size == 1)
    {
        status = ProSelectionDwgtblcellGet(SelBuffer[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
        status = ProSelectionDwgtableGet(SelBuffer[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
    }
    else
    {
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
        status = ProSelectionDwgtblcellGet(tableSels[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
        status = ProSelectionDwgtableGet(tableSels[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
    }
    status = ProDwgtableCelltextWrap(&table, row + 1, column + 1);
    status = ProSelectionarrayFree(SelBuffer);
    status = ProUserRefreshCurrentSheet();
}

void AlignText(ProVerticalJustification VerticalJustification, ProTextHrzJustification HrzJustification)
{
    ProError status;
    ProSelection *SelBuffer = NULL;
    ProSelection *tableSels = NULL;
    int size;
    int tableSegment, row, column;
    ProDwgtable table;
    ProDtlnote note;
    ProDtlnotedata noteData;

    status = ProSelbufferSelectionsGet(&SelBuffer);
    status = ProArraySizeGet(SelBuffer, &size);
    if (status == PRO_TK_NO_ERROR && size == 1)
    {
        status = ProSelectionDwgtblcellGet(SelBuffer[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
        status = ProSelectionDwgtableGet(SelBuffer[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
    }
    else
    {
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
        status = ProSelectionDwgtblcellGet(tableSels[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
        status = ProSelectionDwgtableGet(tableSels[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
    }
    status = ProDwgtableCellNoteGet(&table, column + 1, row + 1, &note);
    status = ProDtlnoteDataGet(&note, NULL, PRODISPMODE_NUMERIC, &noteData);
    status = ProDtlnotedataJustifSet(noteData, HrzJustification, VerticalJustification);
    status = ProDtlnoteModify(&note, NULL, noteData);
    status = ProDtlnotedataFree(noteData);
    status = ProSelectionarrayFree(SelBuffer);
    status = ProUserRefreshCurrentSheet();
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

void SetAutoTextWidth()
{
    ProError status;
    ProSelection *tableSels = NULL;
    ProSelection *SelBuffer = NULL;
    int i, size;
    int tableSegment, row, column;
    ProDwgtable table;
    ProDtlnote note;
    ProDtlnotedata notedata;
    ProDtlnoteline *lines;
    int lineSize;
    ProTextStyle textStyle;
    double width = 0.85;
    ProVector envel[4];
    double cellSize;
    double sizeFactor = 1;

    status = ProSelbufferSelectionsGet(&SelBuffer);
    status = ProArraySizeGet(SelBuffer, &size);
    if (status == PRO_TK_NO_ERROR && size == 1)
    {
        status = ProSelectionDwgtblcellGet(SelBuffer[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
        status = ProSelectionDwgtableGet(SelBuffer[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
    }
    else
    {
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
        status = ProSelectionDwgtblcellGet(tableSels[0], &tableSegment, &row, &column);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
        status = ProSelectionDwgtableGet(tableSels[0], &table);
        if (status != PRO_TK_NO_ERROR)
        {
            return;
        }
    }
    status = ProDwgtableCellNoteGet(&table, column + 1, row + 1, &note);
    status = ProNoteTextStyleGet(&note, &textStyle);
    status = ProTextStyleWidthGet(textStyle, &width);
    if (status != PRO_TK_NO_ERROR)
    {
        width = 0.85;
    }
    status = ProDwgtableColumnSizeGet(&table, tableSegment, column, &cellSize);

    status = ProDtlnoteDataGet(&note, NULL, PRODISPMODE_NUMERIC, &notedata);
    status = ProDtlnotedataLinesCollect(notedata, &lines);
    status = ProArraySizeGet(lines, &lineSize);

    // 遍历每个line找到最长的
    for (i = 0; i < lineSize; i++)
    {
        status = ProDtlnoteLineEnvelopeGet(&note, i, envel);
        if ((envel[1][0] - envel[0][0]) > cellSize && cellSize / (envel[1][0] - envel[0][0]) * 0.92 < sizeFactor)
        {
            sizeFactor = cellSize / (envel[1][0] - envel[0][0]) * 0.92; // 给个系数0.92再缩短点
        }
    }
    status = ProTextStyleWidthSet(textStyle, width * sizeFactor);
    status = ProNoteTextStyleSet(&note, textStyle);
    status = ProTextStyleFree(&textStyle);
    status = ProDtlnotedataFree(notedata);
    status = ProArrayFree(lines);
    status = ProSelectionarrayFree(SelBuffer);
    status = ProUserRefreshCurrentSheet();
}