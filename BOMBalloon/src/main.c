#include "./includes/main.h"

int CurrentMdlType()
{
    ProMdl drawing;
    ProMdlType drawingtype;
    ProError status;
    status = ProMdlCurrentGet(&drawing);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(drawing, &drawingtype);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return drawingtype;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_DRAWING)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

ProError UserTableVisitAct(ProDwgtable *p_table, ProError status, ProAppData tables)
{
    status = ProArrayObjectAdd((ProArray *)tables, PRO_VALUE_UNUSED, 1, p_table);
    return PRO_TK_NO_ERROR;
}

void CreateAllBallon()
{
    ProError status;
    ProMdl drawing;
    ProArray tables;
    int i, n_size;

    status = ProMdlCurrentGet(&drawing);
    status = ProArrayAlloc(0, sizeof(ProDwgtable), 1, &tables);
    status = ProDrawingTableVisit((ProDrawing)drawing, (ProDwgtableVisitAction)UserTableVisitAct, NULL, (ProAppData)&tables);
    ProArraySizeGet(tables, &n_size);
    if (n_size > 0)
    {
        for (i = 0; i < n_size; i++)
        {
            ProDwgtable table;
            table = ((ProDwgtable *)tables)[i];
            status = ProBomballoonAllCreate(drawing, &table, -1);
            if (status == PRO_TK_NO_ERROR)
                break;
        }
    }
    status = ProArrayFree(&tables);
}

void CreateBallon()
{
    ProError status;
    ProMdl drawing;
    ProArray tables;
    int i, n_size;
    ProView view;
    ProSelection *SelBuffer = NULL;

    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &SelBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(SelBuffer[0], &view);

    status = ProArrayAlloc(0, sizeof(ProDwgtable), 1, &tables);
    status = ProDrawingTableVisit((ProDrawing)drawing, (ProDwgtableVisitAction)UserTableVisitAct, NULL, (ProAppData)&tables);
    ProArraySizeGet(tables, &n_size);
    if (n_size > 0)
    {
        for (i = 0; i < n_size; i++)
        {
            ProDwgtable table;
            table = ((ProDwgtable *)tables)[i];
            status = ProBomballoonCreate(drawing, &table, -1, view);
            if (status == PRO_TK_NO_ERROR)
                break;
        }
    }
    status = ProArrayFree(&tables);
}

void CleanupBallon()
{
    ProError status;
    ProMdl drawing;
    ProView view;
    ProSelection *SelBuffer = NULL;
    int n_size;
    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &SelBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(SelBuffer[0], &view);
    status = ProBomballoonClean(drawing, view, PRO_B_TRUE, PRO_B_TRUE, 0, PRO_B_TRUE, 0, PRO_B_TRUE, 0, PRO_B_TRUE);
}
int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_BOMBalloonCreateAllmenuID, IMI_BOMBalloonCreatemenuID, IMI_BOMBalloonCleanupmenuID;

    status = ProMenubarMenuAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateAll_Act", (uiCmdCmdActFn)CreateAllBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateAllmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateAllmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreate_Act", (uiCmdCmdActFn)CreateBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreatemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreatemenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCleanup_Act", (uiCmdCmdActFn)CleanupBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCleanupmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCleanupmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
