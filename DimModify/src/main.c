#include "./includes/main.h"

ProError ProUserRefreshAll()
{
    ProError status;
    ProMdl mdl;
    ProSolid solid;
    status = ProMdlCurrentGet(&mdl);
    solid = ProMdlToSolid(mdl);
    status = ProSolidRegenerate(solid, PRO_REGEN_NO_FLAGS);
    status = ProWindowRepaint(PRO_VALUE_UNUSED);
    return PRO_TK_NO_ERROR;
}

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

ProError ProAddDimPrefix(ProLine prefix)
{
    ProError status;
    ProSelection *SelBuffer = NULL;
    int size;
    ProModelitem Modelitem;

    status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_SelectDimension");
    status = ProSelect((char *)"dimension,ref_dim", 1, NULL, NULL, NULL, NULL, &SelBuffer, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
        return status;

    status = ProSelectionModelitemGet(SelBuffer[0], &Modelitem);
    if (status != PRO_TK_NO_ERROR)
        return status;

    if (Modelitem.type == PRO_DIMENSION)
    {
        status = ProDimensionPrefixSet(&Modelitem, prefix);
        status = ProUserRefreshAll();
    }
    return status;
}

ProError ProAddDimSuffix(ProLine prefix)
{
    ProError status;
    ProSelection *SelBuffer = NULL;
    int size;
    ProModelitem Modelitem;

    status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_SelectDimension");
    status = ProSelect((char *)"dimension,ref_dim", 1, NULL, NULL, NULL, NULL, &SelBuffer, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
        return status;

    status = ProSelectionModelitemGet(SelBuffer[0], &Modelitem);
    if (status != PRO_TK_NO_ERROR)
        return status;

    if (Modelitem.type == PRO_DIMENSION)
    {
        status = ProDimensionSuffixSet(&Modelitem, prefix);
        status = ProUserRefreshAll();
    }
    return status;
}

ProError ProSetDimText(wchar_t *text, int line)
{
    ProError status;
    ProSelection *SelBuffer = NULL;
    int i, size, textarraysize;
    ProModelitem Modelitem;
    wchar_t **p_text;
    wchar_t *nulText = L"";
    status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_SelectDimension");
    status = ProSelect((char *)"dimension,ref_dim", 1, NULL, NULL, NULL, NULL, &SelBuffer, &size);
    if (status != PRO_TK_NO_ERROR || size < 1)
        return status;

    status = ProSelectionModelitemGet(SelBuffer[0], &Modelitem);
    if (status != PRO_TK_NO_ERROR)
        return status;

    if (Modelitem.type == PRO_DIMENSION)
    {
        status = ProDimensionTextWstringsGet(&Modelitem, &p_text);
        status = ProArraySizeGet(p_text, &textarraysize);

        if (textarraysize < line)
        {
            for (i = 0; i < line - textarraysize; i++)
            {
                status = ProArrayObjectAdd((ProArray *)&p_text, 1, 1, &nulText);
            }
        }
        status = ProArrayObjectAdd((ProArray *)&p_text, line - 1, 1, &text);
        status = ProArrayObjectRemove((ProArray *)&p_text, line, 1);

        status = ProDimensionTextWstringsSet(&Modelitem, p_text);
        status = ProArrayFree((ProArray *)&p_text);
    }
    return status;
}

void AddDimPrefix()
{
    ProError status;
    status = ProAddDimPrefix(L"前缀");
}

void AddDimSuffix()
{
    ProError status;
    status = ProAddDimSuffix(L"后缀");
}

void SetDimText()
{
    ProError status;
    status = ProSetDimText(L"测试第二行文字", 2);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_AddDimPrefixmenuID, IMI_AddDimSuffixmenuID, IMI_SetDimTextmenuID;

    status = ProMenubarMenuAdd("IMI_DimModifymenu", "IMI_DimModifymenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_AddD
    imPrefix_Act", (uiCmdCmdActFn)AddDimPrefix, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_AddDimPrefixmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DimModifymenu", "IMI_AddDimPrefixMenu", "IMI_AddDimPrefixMenu", "IMI_AddDimPrefixMenutips", NULL, PRO_B_TRUE, IMI_AddDimPrefixmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_AddDimSuffix_Act", (uiCmdCmdActFn)AddDimSuffix, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_AddDimSuffixmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DimModifymenu", "IMI_AddDimSuffixMenu", "IMI_AddDimSuffixMenu", "IMI_AddDimSuffixMenutips", NULL, PRO_B_TRUE, IMI_AddDimSuffixmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_SetDimText_Act", (uiCmdCmdActFn)SetDimText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_SetDimTextmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DimModifymenu", "IMI_SetDimTextMenu", "IMI_SetDimTextMenu", "IMI_SetDimTextMenutips", NULL, PRO_B_TRUE, IMI_SetDimTextmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
