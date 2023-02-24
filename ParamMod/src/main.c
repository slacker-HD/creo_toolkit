#include "./includes/main.h"

#define DIALOGNAME (char *)"ParamsMod"
#define TABLENAME (char *)"TableParams"
#define INPUTPANENAME (char *)"InputPanel1"
#define OPTIONMENUNAME (char *)"OptionMenu1"
#define CHECKBUTTONNAME (char *)"CheckButton1"

static char *_ipRow;
static char *_ipCol;
int column_size = 3;
char *column_names[] = {"name", "OpMenu", "unit"};
wchar_t *column_labels[] = {L"工作目录下模型", L"测试OptionMenu", L"测试CheckButton"};

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

void _uiTableInputPanelEditLoseIPfocus(char *dialog, char *component, ProAppData data)
{
    ProError status;
    wchar_t *label;
    char inputPanel[20];

    ProTKSprintf(inputPanel, "%s_%s", _ipRow, _ipCol);

    status = ProUIInputpanelValueGet(DIALOGNAME, inputPanel, &label);

    if ((status == PRO_TK_NO_ERROR) && (label != NULL))
    {
        status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, _ipRow, _ipCol, label);
    }

    status = ProUITableCellComponentDelete(DIALOGNAME, TABLENAME, _ipRow, _ipCol);

    status = ProWstringFree(label);
    status = ProStringFree(_ipRow);
    status = ProStringFree(_ipCol);
}

void _addUIInputPanel(char *InputPanelName, int Width, wchar_t *Label)
{
    ProError status;

    status = ProUIInputpanelColumnsSet(DIALOGNAME, InputPanelName, Width);
    status = ProUIInputpanelEnable(DIALOGNAME, InputPanelName);
    status = ProUIInputpanelShow(DIALOGNAME, InputPanelName);
    status = ProUIInputpanelValueSet(DIALOGNAME, InputPanelName, Label);

    status = ProUIInputpanelFocusoutActionSet(DIALOGNAME, InputPanelName, (ProUIAction)_uiTableInputPanelEditLoseIPfocus, NULL);
    status = ProUIInputpanelActivateActionSet(DIALOGNAME, InputPanelName, (ProUIAction)_uiTableInputPanelEditLoseIPfocus, NULL);

    status = ProUIDialogFocusSet(DIALOGNAME, InputPanelName);
}

void _uiTableEditActivateCell(char *dialog, char *component, ProAppData data)
{
    ProError status;
    int count, i, width;
    char **values;
    char *row;
    wchar_t *label;
    char inputPanel[20];
    char optionMenu[20];

    status = ProUITableFocusCellGet(DIALOGNAME, TABLENAME, &_ipRow, &_ipCol);

    if (status == PRO_TK_NO_ERROR)
    {
        status = ProUITableCellLabelGet(DIALOGNAME, TABLENAME, _ipRow, _ipCol, &label);
        status = ProUITableColumnWidthGet(DIALOGNAME, TABLENAME, _ipCol, &width);

        if (strcmp(column_names[0], _ipCol) == 0)
        {
            ProTKSprintf(inputPanel, "%s_%s", _ipRow, _ipCol);
            status = ProUITableCellComponentCopy(DIALOGNAME, TABLENAME, _ipRow, _ipCol, DIALOGNAME, INPUTPANENAME, inputPanel);
            if (status == PRO_TK_NO_ERROR)
            {
                _addUIInputPanel(inputPanel, width, label);
            }
            else
            {
                status = ProStringFree(_ipRow);
                status = ProStringFree(_ipCol);
            }
        }
        else if (strcmp(column_names[1], _ipCol) == 0)
        {
            // status = ProTKSprintf(optionMenu, "%s_%s", _ipRow, _ipCol);
            // status = ProUITableCellComponentCopy(DIALOGNAME, TABLENAME, _ipRow, _ipCol, DIALOGNAME, OPTIONMENUNAME, optionMenu);
            // if (status == PRO_TK_NO_ERROR)
            // {
            //     _addUIOptionMenu(optionMenu, width, label);
            // }
            // else
            // {
            //     status = ProStringFree(_ipRow);
            //     status = ProStringFree(_ipCol);
            // }
        }
        else
        {
        }

        status = ProWstringFree(label);
    }
}

void ShowParamsModDialog()
{
    ProError status;
    int i, nFiles;
    ProPath *fileList, *dirList, rPath;
    ProName rFileName, rExtension, mdlName;
    ProFileName fFileName;
    int nRversion;
    ProMdl mdl;
    ProMaterial material;
    ProUnitsystem unitSystem;

    char **row_names;
    wchar_t **row_labels;

    char checkbutton[20];
    char optionMenu[20];

    status = ProUIDialogCreate(DIALOGNAME, DIALOGNAME);

    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitOK", (ProUIAction)_commitOK, NULL);
    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitCancel", (ProUIAction)_commitCancel, NULL);

    status = ProUITableColumnnamesSet(DIALOGNAME, TABLENAME, column_size, column_names);
    status = ProUITableColumnlabelsSet(DIALOGNAME, TABLENAME, column_size, column_labels);

    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&fileList);
    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dirList);
    status = ProFilesList(NULL, L"*.prt", PRO_FILE_LIST_LATEST, &fileList, &dirList);

    if (status == PRO_TK_NO_ERROR)
    {
        status = ProArraySizeGet((ProArray)fileList, &nFiles);
        row_names = (char **)calloc(nFiles, sizeof(char *));
        row_labels = (wchar_t **)calloc(nFiles, sizeof(wchar_t *));

        if (nFiles > 0)
        {
            for (i = 0; i < nFiles; i++)
            {
                row_names[i] = (char *)calloc(10, sizeof(char));
                row_labels[i] = (wchar_t *)calloc(10, sizeof(wchar_t));
                sprintf(row_names[i], "%d", i + 1);
                wsprintfW(row_labels[i], L"%d", i + 1);
            }
            status = ProUITableRownamesSet(DIALOGNAME, TABLENAME, nFiles, row_names);
            status = ProUITableRowlabelsSet(DIALOGNAME, TABLENAME, nFiles, row_labels);

            for (i = 0; i < nFiles; i++)
            {
                memset(fFileName, '\0', sizeof(ProFileName));
                status = ProFilenameParse(fileList[i], rPath, rFileName, rExtension, &nRversion);
                status = ProWstringConcatenate(rFileName, fFileName, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(L".", fFileName, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(rExtension, fFileName, PRO_VALUE_UNUSED);
                status = ProMdlLoad(fFileName, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
                status = ProMdlNameGet(mdl, mdlName);
                status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], column_names[0], mdlName);

                // status = ProMaterialCurrentGet(ProMdlToSolid(mdl), &material);
                // if (status == PRO_TK_NO_ERROR)
                //     status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], column_names[1], material.matl_name);
                // else
                //     status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], column_names[1], L"未设置");

                ProTKSprintf(optionMenu, "%s_%s", row_names[i], column_names[1]);
                status = ProUITableCellComponentCopy(DIALOGNAME, TABLENAME, row_names[i], column_names[1], DIALOGNAME, OPTIONMENUNAME, optionMenu);

                // status = ProMdlPrincipalunitsystemGet(mdl, &unitSystem);
                // if (status == PRO_TK_NO_ERROR)
                //     status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], column_names[2], unitSystem.name);
                // else
                //     status = ProUITableCellLabelSet(DIALOGNAME, TABLENAME, row_names[i], column_names[2], L"未知");
                ProTKSprintf(checkbutton, "%s_%s", row_names[i], column_names[2]);
                status = ProUITableCellComponentCopy(DIALOGNAME, TABLENAME, row_names[i], column_names[2], DIALOGNAME, CHECKBUTTONNAME, checkbutton);

                free(row_names[i]);
                free(row_labels[i]);
            }
            free(row_names);
            free(row_labels);
        }
    }

    status = ProUITableSelectActionSet(DIALOGNAME, TABLENAME, (ProUIAction)_uiTableEditActivateCell, NULL);

    status = ProArrayFree((ProArray *)&fileList);
    status = ProArrayFree((ProArray *)&dirList);

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
