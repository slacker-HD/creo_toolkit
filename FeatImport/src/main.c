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

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

void ImportFeat()
{
    ProError status;
    ProMdl mdl;
    ProSolid solid;
    ProPath stpFile;
    ProImportfeatAttr importfeatAttr = {1, 1, 1};
    ProIntfType intfType = PRO_INTF_STEP;
    ProIntfData *intfData;
    ProCsys csys;
    ProModelitem modelItem;
    ProFeature feature;
    ProIntfDataSource intfDataSource;
    int n_size;
    ProSelection *sel;

    status = ProMdlCurrentGet(&mdl);

    solid = ProMdlToSolid(mdl);

    status = ProFileOpen(L"选择要导入的step文件", L"*.stp", NULL, NULL, NULL, NULL, stpFile);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectCsys");
        status = ProSelect((char *)"csys", -1, NULL, NULL, NULL, NULL, &sel, &n_size);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProSelectionModelitemGet(sel[0], &modelItem);
            status = ProGeomitemToCsys(&modelItem,&csys);
            status = ProIntfDataAlloc(&intfData);
            status = ProIntfDataSourceInit(intfType, stpFile, &intfDataSource);
            status = ProImportfeatCreate(solid, &intfDataSource, csys, &importfeatAttr, &feature);
            status = ProIntfDataFree(intfData);
        }
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ImportFeatmenuID;

    status = ProMenubarMenuAdd("IMI_FeatImportmenu", "IMI_FeatImportmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_ImportFeat_Act", (uiCmdCmdActFn)ImportFeat, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_ImportFeatmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_FeatImportmenu", "IMI_ImportFeatmenu", "IMI_ImportFeatmenu", "IMI_ImportFeatmenutips", NULL, PRO_B_TRUE, IMI_ImportFeatmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
