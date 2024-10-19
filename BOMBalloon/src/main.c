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
    status = ProArraySizeGet(tables, &n_size);
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
    status = ProArraySizeGet(tables, &n_size);
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
    status = ProSelect((char *)"prt_or_asm", -1, NULL, NULL, NULL, NULL, &selBuffer, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }

    status = ProArrayAlloc(0, sizeof(ProDwgtable), 1, &tables);
    status = ProDrawingTableVisit((ProDrawing)drawing, (ProDwgtableVisitAction)UserTableVisitAct, NULL, (ProAppData)&tables);
    status = ProArraySizeGet(tables, &tab_size);
    if (tab_size > 0)
    {
        for (i = 0; i < tab_size; i++)
        {
            for (j = 0; j < n_size; j++)
            {
                status = ProSelectionAsmcomppathGet(selBuffer[j], &asmCompPath);
                status = ProArrayAlloc(0, sizeof(int), asmCompPath.table_num, &compIDTabs);
                for (k = 0; k < asmCompPath.table_num; k++)
                {
                    status = ProArrayObjectAdd((ProArray *)&compIDTabs, PRO_VALUE_UNUSED, 1, &asmCompPath.comp_id_table[k]);
                }
                status = ProBomballoonByComponentCreate(drawing, &(((ProDwgtable *)tables)[i]), 0, view, compIDTabs);
                status = ProArrayFree(&compIDTabs);
                if (status != PRO_TK_NO_ERROR)
                    break;
            }
        }
    }
    status = ProArrayFree(&tables);
}

void CreateBallonByRecordWithoutLeader()
{
    ProError status;
    ProMdl drawing;
    ProDwgtable table;
    int i, n_size, cell_size;
    int table_segment, row, column, cell_region_id;
    ProView view;
    ProSelection *selView = NULL, *selCells = NULL;
    ProMouseButton btn;
    Pro3dPnt sel_pnt;

    status = ProMdlCurrentGet(&drawing);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectView");
    status = ProSelect((char *)"dwg_view", 1, NULL, NULL, NULL, NULL, &selView, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(selView[0], &view);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectCells");
    // table_row是未公开的选择项，这样选择表的行更好，但是没有找到对应的对象
    status = ProSelect((char *)"table_cell", 1, NULL, NULL, NULL, NULL, &selCells, &cell_size);
    if (status != PRO_TK_NO_ERROR || cell_size < 1)
    {
        return;
    }

    status = ProMessageDisplay(MSGFILE, "IMI_MousePick");
    status = ProMousePickGet(PRO_ANY_BUTTON, &btn, sel_pnt);

    if (status == PRO_TK_NO_ERROR)
    {
        for (i = 0; i < cell_size; i++)
        {
            status = ProSelectionDwgtableGet(selCells[i], &table);
            status = ProSelectionDwgtblcellGet(selCells[i], &table_segment, &row, &column);
            status = ProDwgtableCellRegionGet(drawing, &table, column, row, &cell_region_id);
            status = ProBomballoonByRecordCreate(drawing, &table, cell_region_id, view, row - 1, NULL, PRO_VALUE_UNUSED, PRO_EDGE, sel_pnt);
        }
    }
}

void CreateBallonByRecordWithLeader()
{

    ProError status;
    ProMdl drawing;
    ProDwgtable table;
    int i, n_size;
    int table_segment, row, column;
    int cell_region_id;
    ProView view;
    ProModelitem modelItem;
    ProAsmcomppath asmCompPath;
    int *compIDTabs;
    ProSelection *selView = NULL, *selCells = NULL, *selEdge = NULL, *selComp = NULL;
    ProMouseButton btn;
    Pro3dPnt sel_pnt;

    status = ProMdlCurrentGet(&drawing);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectCells");
    // table_row是未公开的选择项，这样选择表的行更好，但是没有找到对应的对象
    status = ProSelect((char *)"table_cell", 1, NULL, NULL, NULL, NULL, &selCells, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionDwgtableGet(selCells[0], &table);
    status = ProSelectionDwgtblcellGet(selCells[0], &table_segment, &row, &column);
    status = ProDwgtableCellRegionGet(drawing, &table, column, row, &cell_region_id);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectComponents");
    status = ProSelect((char *)"prt_or_asm", 1, NULL, NULL, NULL, NULL, &selComp, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionAsmcomppathGet(selComp[0], &asmCompPath);

    status = ProMessageDisplay(MSGFILE, "IMI_PrompSelectEntity");
    status = ProSelect((char *)"edge,silhedge", 1, NULL, NULL, NULL, NULL, &selEdge, &n_size);
    if (status != PRO_TK_NO_ERROR || n_size < 1)
    {
        return;
    }
    status = ProSelectionViewGet(selEdge[0], &view);
    status = ProSelectionModelitemGet(selEdge[0], &modelItem);

    status = ProMessageDisplay(MSGFILE, "IMI_MousePick");
    status = ProMousePickGet(PRO_ANY_BUTTON, &btn, sel_pnt);

    if (status == PRO_TK_NO_ERROR)
    {
        status = ProArrayAlloc(0, sizeof(int), asmCompPath.table_num, &compIDTabs);
        for (i = 0; i < asmCompPath.table_num; i++)
        {
            status = ProArrayObjectAdd((ProArray *)&compIDTabs, PRO_VALUE_UNUSED, 1, &asmCompPath.comp_id_table[i]);
        }

        status = ProBomballoonByRecordCreate(drawing, &table, cell_region_id, view, row - 1, compIDTabs, modelItem.id, PRO_EDGE, sel_pnt);
        status = ProArrayFree(&compIDTabs);
    }
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
    uiCmdCmdId IMI_BOMBalloonCreateAllmenuID, IMI_BOMBalloonCreatemenuID, IMI_BOMBalloonCreateByComponentmenuID, IMI_BOMBalloonCleanupmenuID, IMI_BOMBalloonCreateByRecordmenu1ID, IMI_BOMBalloonCreateByRecordmenu2ID;

    status = ProMenubarMenuAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateAll_Act", (uiCmdCmdActFn)CreateAllBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateAllmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenu", "IMI_BOMBalloonCreateAllmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateAllmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreate_Act", (uiCmdCmdActFn)CreateBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreatemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenu", "IMI_BOMBalloonCreatemenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreatemenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateByComponent_Act", (uiCmdCmdActFn)CreateBallonByComponent, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateByComponentmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateByComponentmenu", "IMI_BOMBalloonCreateByComponentmenu", "IMI_BOMBalloonCreateByComponentmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateByComponentmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateByRecord1_Act", (uiCmdCmdActFn)CreateBallonByRecordWithoutLeader, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateByRecordmenu1ID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateByRecordmenu1", "IMI_BOMBalloonCreateByRecordmenu1", "IMI_BOMBalloonCreateByRecordmenu1tips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateByRecordmenu1ID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCreateByRecord2_Act", (uiCmdCmdActFn)CreateBallonByRecordWithLeader, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCreateByRecordmenu2ID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCreateByRecordmenu2", "IMI_BOMBalloonCreateByRecordmenu2", "IMI_BOMBalloonCreateByRecordmenu2tips", NULL, PRO_B_TRUE, IMI_BOMBalloonCreateByRecordmenu2ID, MSGFILE);

    status = ProCmdActionAdd("IMI_BOMBalloonCleanup_Act", (uiCmdCmdActFn)CleanupBallon, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_BOMBalloonCleanupmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BOMBalloonmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenu", "IMI_BOMBalloonCleanupmenutips", NULL, PRO_B_TRUE, IMI_BOMBalloonCleanupmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
