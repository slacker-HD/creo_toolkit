#include "./includes/main.h"

#define DIALOGNAME (char *)"ParamsMod"
#define TABLENAME (char *)"TableParams"
#define INPUTPANENAME (char *)"InputPanel1"

static char *ip_row;
static char *ip_col;

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void _commitOK()
{
    ProError status;
    status = ProUIDialogExit(DIALOGNAME, 1);
}

void _commitCancel()
{
    ProError status;
    status = ProUIDialogExit(DIALOGNAME, 0);
}

void _uiTableEditLoseIPfocus(char *dialog, char *component, ProAppData data)
{
    ProError status;
    wchar_t *label;
    char input_panel[20];

    ProTKSprintf(input_panel, "%s_%s", ip_row, ip_col);

    status = ProUIInputpanelValueGet(DIALOGNAME, input_panel, &label);

    if ((status == PRO_TK_NO_ERROR) && (label != NULL))
    {
        status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, ip_row, ip_col, label);
    }

    status = ProUITableCellComponentDelete(DIALOGNAME, TABLENAME, ip_row, ip_col);

    status = ProWstringFree(label);
    status = ProStringFree(ip_row);
    status = ProStringFree(ip_col);
}

void _uiTableEditActivateCell(char *dialog, char *component, ProAppData data)
{
    ProError status;
    int count, i, width;
    char **values;
    char *row;
    wchar_t *label;
    char input_panel[20];

    status = ProUITableFocusCellGet(DIALOGNAME, TABLENAME, &ip_row, &ip_col);

    if (status == PRO_TK_NO_ERROR)
    {
        status = ProUITableCellLabelGet(DIALOGNAME, TABLENAME, ip_row, ip_col, &label);

        status = ProTKSprintf(input_panel, "%s_%s", ip_row, ip_col);

        status = ProUITableCellComponentCopy(DIALOGNAME, TABLENAME, ip_row, ip_col, DIALOGNAME, INPUTPANENAME, input_panel);

        if (status == PRO_TK_NO_ERROR)
        {

            status = ProUITableColumnWidthGet(DIALOGNAME, TABLENAME, ip_col, &width);
            status = ProUIInputpanelColumnsSet(DIALOGNAME, input_panel, width);
            status = ProUIInputpanelEnable(DIALOGNAME, input_panel);
            status = ProUIInputpanelShow(DIALOGNAME, input_panel);
            status = ProUIInputpanelValueSet(DIALOGNAME, input_panel, label);

            status = ProUIInputpanelFocusoutActionSet(DIALOGNAME, input_panel, (ProUIAction)_uiTableEditLoseIPfocus, NULL);
            status = ProUIInputpanelActivateActionSet(DIALOGNAME, input_panel, (ProUIAction)_uiTableEditLoseIPfocus, NULL);

            status = ProUIDialogFocusSet(DIALOGNAME, input_panel);
        }
        else
        {
            status = ProStringFree(ip_row);
            status = ProStringFree(ip_col);
        }
        status = ProWstringFree(label);
    }
}

void ShowParamsModDialog()
{
    ProError status;
    int i, n_files;
    ProPath *file_list, *dir_list, r_path;
    ProName r_file_name, r_extension, mdl_name;
    ProFileName f_file_name;
    int r_version;
    ProMdl mdl;
    ProMaterial material;
    ProUnitsystem unitSystem;

    int column_size = 3;
    char *column_names[] = {"name", "material", "unit"};
    wchar_t *column_labels[] = {L"模型名称", L"材料", L"单位"};

    char **row_names;
    wchar_t **row_labels;

    status = ProUIDialogCreate(DIALOGNAME, DIALOGNAME);

    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitOK", (ProUIAction)_commitOK, NULL);
    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitCancel", (ProUIAction)_commitCancel, NULL);

    status = ProUITableColumnnamesSet(DIALOGNAME, TABLENAME, column_size, column_names);
    status = ProUITableColumnlabelsSet(DIALOGNAME, TABLENAME, column_size, column_labels);

    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
    status = ProFilesList(NULL, L"*.prt", PRO_FILE_LIST_LATEST, &file_list, &dir_list);

    if (status == PRO_TK_NO_ERROR)
    {
        status = ProArraySizeGet((ProArray)file_list, &n_files);
        row_names = (char **)calloc(n_files, sizeof(char *));
        row_labels = (wchar_t **)calloc(n_files, sizeof(wchar_t *));

        if (n_files > 0)
        {
            for (i = 0; i < n_files; i++)
            {
                row_names[i] = (char *)calloc(10, sizeof(char));
                row_labels[i] = (wchar_t *)calloc(10, sizeof(wchar_t));
                sprintf(row_names[i], "%d", i + 1);
                wsprintfW(row_labels[i], L"%d", i + 1);
            }
            status = ProUITableRownamesSet(DIALOGNAME, TABLENAME, n_files, row_names);
            status = ProUITableRowlabelsSet(DIALOGNAME, TABLENAME, n_files, row_labels);

            for (i = 0; i < n_files; i++)
            {
                memset(f_file_name, '\0', sizeof(ProFileName));
                status = ProFilenameParse(file_list[i], r_path, r_file_name, r_extension, &r_version);
                status = ProWstringConcatenate(r_file_name, f_file_name, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(L".", f_file_name, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(r_extension, f_file_name, PRO_VALUE_UNUSED);
                status = ProMdlLoad(f_file_name, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
                status = ProMdlNameGet(mdl, mdl_name);
                status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], "name", mdl_name);

                status = ProMaterialCurrentGet(ProMdlToSolid(mdl), &material);
                if (status == PRO_TK_NO_ERROR)
                    status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], "material", material.matl_name);
                else
                    status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], "material", L"未设置");

                status = ProMdlPrincipalunitsystemGet(mdl, &unitSystem);
                if (status == PRO_TK_NO_ERROR)
                    status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], "unit", unitSystem.name);
                else
                    status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], "unit", L"未知");

                free(row_names[i]);
                free(row_labels[i]);
            }
            free(row_names);
            free(row_labels);
        }
    }

    status = ProUITableSelectActionSet(DIALOGNAME, TABLENAME, (ProUIAction)_uiTableEditActivateCell, NULL);

    status = ProArrayFree((ProArray *)&file_list);
    status = ProArrayFree((ProArray *)&dir_list);

    status = ProUIDialogActivate(DIALOGNAME, NULL);
    status = ProUIDialogDestroy(DIALOGNAME);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ParamModmenuID;

    status = ProMenubarMenuAdd("IMI_Paramsmenu", "IMI_Paramsmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitInterPret_Act", (uiCmdCmdActFn)ShowParamsModDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ParamModmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Paramsmenu", "IMI_ParamModmenu", "IMI_ParamModmenu", "IMI_ParamModmenutips", NULL, PRO_B_TRUE, IMI_ParamModmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
