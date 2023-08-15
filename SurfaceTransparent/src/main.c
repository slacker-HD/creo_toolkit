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

static uiCmdAccessState AccessPrtORAsm(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART || CurrentMdlType() == PRO_ASSEMBLY)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

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

void SetSurfaceTransparent()
{
    ProError status;
    ProModelitem modelitem;
    ProSurfaceAppearanceProps appearanceProperties;
    ProSelection *sel_array;
    int i, n_size;

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSetTransparent");
    status = ProSelect((char *)"sldsurface,surface", -1, NULL, NULL, NULL, NULL, &sel_array, &n_size);
    status = ProArraySizeGet((ProArray *)sel_array, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    for (i = 0; i < n_size; i++)
    {
        status = ProSelectionModelitemGet(sel_array[i], &modelitem);
        status = ProSurfaceAppearancepropsGet(&modelitem, &appearanceProperties);
        appearanceProperties.transparency = 0.7;
        status = ProSurfaceAppearancepropsSet(&modelitem, &appearanceProperties);
    }
    status = ProUserRefreshAll();
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_SetTransparentmenuID;

    status = ProMenubarMenuAdd("IMI_SurfaceTransparentmenu", "IMI_SurfaceTransparentmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_SetTransparent_Act", (uiCmdCmdActFn)SetSurfaceTransparent, uiProeImmediate, AccessPrtORAsm, PRO_B_TRUE, PRO_B_TRUE, &IMI_SetTransparentmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_SurfaceTransparentmenu", "IMI_SetTransparentmenu", "IMI_SetTransparentmenu", "IMI_SetTransparentmenutips", NULL, PRO_B_TRUE, IMI_SetTransparentmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
