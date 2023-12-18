#include "./includes/TableCellWidth.h"

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
