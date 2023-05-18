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

void FormatText()
{
    ProError status;
    ProSelection *itemSels = NULL;
    int size;
    ProNote srcItem, destItem;
    ProTextStyle textStyle;
    ProMdl drawing;
    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectSource");
    status = ProSelect((char *)"dimension,ref_dim,any_note,table_cell", 1, NULL, NULL, NULL, NULL, &itemSels, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
    {
        return;
    }
    status = ProSelectionModelitemGet(itemSels[0], &srcItem);

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
        return;
    }

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectDest");
    status = ProSelect((char *)"dimension,ref_dim,any_note,table_cell", 1, NULL, NULL, NULL, NULL, &itemSels, &size);
    if (status == PRO_TK_NO_ERROR && size > 0)
    {
        status = ProSelectionModelitemGet(itemSels[0], &destItem);
        switch (srcItem.type)
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
    status = ProTextStyleFree(&textStyle);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_FormatTextmenuID;

    status = ProMenubarMenuAdd("IMI_TextFormatmenu", "IMI_TextFormatmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitInterPret_Act", (uiCmdCmdActFn)FormatText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_FormatTextmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_TextFormatmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenutips", NULL, PRO_B_TRUE, IMI_FormatTextmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
