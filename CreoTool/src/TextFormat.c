#include "./includes/TextFormat.h"

void FormatText()
{
    ProError status, err;
    ProSelection *itemSels = NULL;
    int i, size;
    ProNote srcItem, destItem;
    ProTextStyle textStyle;
    ProMdl drawing;
    ProModelitem item;
    ProSelection *SelBuffer = NULL;

    if (CurrentMdlType() != PRO_DRAWING)
    {
        return;
    }

    status = ProMdlCurrentGet(&drawing);
    status = ProSelbufferSelectionsGet(&SelBuffer);
    err = ProArraySizeGet(SelBuffer, &size);

    if (status == PRO_TK_NO_ERROR && size == 1)
    {
        status = ProSelectionModelitemGet(SelBuffer[0], &srcItem);
    }
    else
    {
        status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectSource");
        status = ProSelect((char *)"dimension,ref_dim,any_note,table_cell", 1, NULL, NULL, NULL, NULL, &itemSels, &size);
        if (status != PRO_TK_NO_ERROR || size < 1)
        {
            status = ProSelectionarrayFree(SelBuffer);
            return;
        }
        status = ProSelectionModelitemGet(itemSels[0], &srcItem);
    }

    status = ProTextStyleAlloc(&textStyle);
    switch (srcItem.type)
    {
    case PRO_DIMENSION:
    case PRO_REF_DIMENSION:
    case PRO_GTOL:
        status = ProAnnotationTextstyleGet(&srcItem, drawing, NULL, NULL, &textStyle);
        break;
    case PRO_NOTE:
        status = ProNoteTextStyleGet(&srcItem, &textStyle);
        break;
    default:
        status = ProTextStyleFree(&textStyle);
        status = ProSelectionarrayFree(SelBuffer);
        return;
    }

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectDest");
    status = ProSelect((char *)"dimension,ref_dim,any_note,table_cell", -1, NULL, NULL, NULL, NULL, &itemSels, &size);
    if (status == PRO_TK_NO_ERROR && size > 0)
    {
        for (i = 0; i < size; i++)
        {
            status = ProSelectionModelitemGet(itemSels[i], &destItem);
            switch (destItem.type)
            {
            case PRO_DIMENSION:
            case PRO_REF_DIMENSION:
            case PRO_GTOL:
                status = ProAnnotationTextstyleSet(&destItem, drawing, NULL, NULL, textStyle);
                break;
            case PRO_NOTE:
                status = ProNoteTextStyleSet(&destItem, textStyle);
                break;
            default:
                break;
            }
        }
    }
    status = ProTextStyleFree(&textStyle);
    status = ProSelectionarrayFree(SelBuffer);
}
