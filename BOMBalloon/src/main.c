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
            // 如果知道工程图的表格实际情况，利用ProDwgtableCellRegionGet根据行、列查找表格，通用的只能强制遍历使用默认值-1尝试了
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
    ProSelection *selBuffer = NULL;

    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &selBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(selBuffer[0], &view);

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

void CreateBallonByComponent()
{
    ProError status;
    ProMdl drawing;
    ProArray tables;
    int i, j, k, n_size, tab_size;
    ProView view;
    ProSelection *selBuffer = NULL;
    ProAsmcomppath asmCompPath;
    int row = 0, column = 0;
    int *compIDTabs;
    status = ProMdlCurrentGet(&drawing);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &selBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(selBuffer[0], &view);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectComponents");
    //! 直接component无法获取asmCompPath，相反用part可以
    status = ProSelect((char *)"component", -1, NULL, NULL, NULL, NULL, &selBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }

    status = ProArrayAlloc(0, sizeof(ProDwgtable), 1, &tables);
    status = ProDrawingTableVisit((ProDrawing)drawing, (ProDwgtableVisitAction)UserTableVisitAct, NULL, (ProAppData)&tables);
    ProArraySizeGet(tables, &tab_size);
    if (tab_size > 0)
    {
        for (i = 0; i < tab_size; i++)
        {
            ProDwgtable table;
            table = ((ProDwgtable *)tables)[i];
            for (j = 0; j < n_size; j++)
            {
                //! 未完成，手动和程序设定运行均无错误，但无法生成
                status = ProSelectionAsmcomppathGet(selBuffer[j], &asmCompPath);
                status = ProArrayAlloc(0, sizeof(int), asmCompPath.table_num, &compIDTabs);
                // compIDTabs = (int *)malloc(sizeof(int) * asmCompPath.table_num);
                for (k = 0; k < asmCompPath.table_num; k++)
                {
                    compIDTabs[k] = asmCompPath.comp_id_table[k];
                }
                status = ProBomballoonByComponentCreate(drawing, &table, -1, view, compIDTabs);
                // free(compIDTabs);
                // status = ProArrayFree((ProArray *)compIDTabs);
                if (status != PRO_TK_NO_ERROR)
                    break;
            }
        }
    }
    status = ProArrayFree(&tables);
}

void CleanupBallon()
{
    ProError status;
    ProMdl drawing;
    ProView view;
    ProSelection *selBuffer = NULL;
    int n_size;
    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &selBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(selBuffer[0], &view);
    status = ProBomballoonClean(drawing, view, PRO_B_TRUE, PRO_B_TRUE, 0, PRO_B_TRUE, 0, PRO_B_TRUE, 0, PRO_B_TRUE);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_BOMBalloonCreateAllmenuID, IMI_BOMBalloonCreatemenuID, IMI_BOMBalloonCreateByComponentmenuID, IMI_BOMBalloonCleanupmenuID;

    status = ProMenubarMenuAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateAll_Act", (uiCmdCmdActFn)CreateAllBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateAllmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateAllmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreate_Act", (uiCmdCmdActFn)CreateBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreatemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreatemenuID, MSGFILE);
    //! 未完成，函数运行无错误，但无法生成
    // status = ProCmdActionAdd("IMI_BOMBalloonCreateByComponent_Act", (uiCmdCmdActFn)CreateBallonByComponent, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateByComponentmenuID);
    // status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateByComponentmenu", "IMI_BOMBalloonCreateByComponentmenu", "IMI_BOMBalloonCreateByComponentmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateByComponentmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCleanup_Act", (uiCmdCmdActFn)CleanupBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCleanupmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCleanupmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
