#include "./includes/BatConvertUnit.h"

#define BATCONVERTUNITDIALOG (char *)"BatConvertUnit"
#define TABLEUNIT (char *)"TableUnit"
#define OPTIONMENUMETHOD (char *)"OptionMenuMethod"
#define BATCONVERTUNITCOMMITOK (char *)"BatConvertUnitCommitOK"
#define BATCONVERTUNITCOMMITCANCEL (char *)"BatConvertUnitCommitCancel"
#define BATCONVERTUNITCOMMITAPPLY (char *)"BatConvertUnitCommitApply"
#define PROGRESSBARUNIT (char *)"ProgressBarUnit"

static char *_ipRow;
static char *_ipCol;
int column_size = 3;
char *column_names[] = {"name", "unit", "method"};
wchar_t *column_labels[] = {L"零件", L"当前单位", L"转换方式"};
wchar_t *convert_methods——labels[] = {L"不转换", L"解释尺寸", L"转换尺寸"};

void _convertUnit(ProMdl Mdl, ProUnitConvertType ConVertType)
{
    ProError status;
    ProUnitsystem unitSystem;
    ProUnitsystemType type;
    wchar_t *p = NULL;
    ProUnitsystem *unitSystem_array;
    int num_unitSystem, i;

    status = ProMdlPrincipalunitsystemGet(Mdl, &unitSystem);
    p = wcsstr(unitSystem.name, L"mmNs");
    if (!p)
    {
        status = ProMdlUnitsystemsCollect(Mdl, &unitSystem_array);
        if (status != PRO_TK_NO_ERROR)
            return;
        status = ProArraySizeGet(unitSystem_array, &num_unitSystem);
        for (i = 0; i < num_unitSystem; i++)
        {
            p = wcsstr(unitSystem_array[i].name, L"mmNs");
            if (p)
            {
                status = ProMdlPrincipalunitsystemSet(Mdl, &unitSystem_array[i], ConVertType, PRO_B_TRUE, PRO_VALUE_UNUSED);
                status = ProArrayFree((ProArray *)&unitSystem_array);
                return;
            }
        }
        ShowMessageDialog(1, L"模型中未包含mmNs单位。\n请检查模型设置。");
        status = ProArrayFree((ProArray *)&unitSystem_array);
    }
}

void InterPretUnit(ProMdl Mdl)
{
    _convertUnit(Mdl, PRO_UNITCONVERT_SAME_DIMS);
}

void ConvertUnit(ProMdl Mdl)
{
    _convertUnit(Mdl, PRO_UNITCONVERT_SAME_SIZE);
}

void _commitOK()
{
    ProError status;
    status = ProUIDialogExit(BATCONVERTUNITDIALOG, 1);
}

void _commitCancel()
{
    ProError status;
    status = ProUIDialogExit(BATCONVERTUNITDIALOG, 0);
}

void _commitApply()
{
    ProError status;
    int i;
    int length;
    char **rows;
    wchar_t *mdlName;
    char *opmenuName;
    wchar_t *method;
    status = ProUITableRownamesGet(BATCONVERTUNITDIALOG, TABLEUNIT, &length, &rows);
    for (i = 0; i < length; i++)
    {
        status = ProUITableCellLabelGet(BATCONVERTUNITDIALOG, TABLEUNIT, rows[i], column_names[0], &mdlName);
        status = ProUITableCellComponentNameGet(BATCONVERTUNITDIALOG, TABLEUNIT, rows[i], column_names[2], &opmenuName);
        status = ProUIOptionmenuValueGet(BATCONVERTUNITDIALOG, opmenuName, &method);

        // status = ProMdlLoad(mdl,)

        status = ProWstringFree(mdlName);
        status = ProStringFree(opmenuName);
        status = ProWstringFree(method);
    }
    status = ProStringarrayFree(rows, length);
}

void ShowBatConvertUnitDialogDialog()
{
    ProError status;
    int i, nFiles;
    ProPath *fileList, *dirList, rPath;
    ProName rFileName, rExtension, mdlName;
    ProFileName fFileName;
    int nRversion;
    ProMdl mdl;
    ProUnitsystem unitSystem;
    char **row_names;
    wchar_t **row_labels;
    char optionMenu[20];
    wchar_t *p = NULL;

    status = ProUIDialogCreate(BATCONVERTUNITDIALOG, BATCONVERTUNITDIALOG);

    status = ProUIPushbuttonActivateActionSet(BATCONVERTUNITDIALOG, BATCONVERTUNITCOMMITOK, (ProUIAction)_commitOK, NULL);
    status = ProUIPushbuttonActivateActionSet(BATCONVERTUNITDIALOG, BATCONVERTUNITCOMMITCANCEL, (ProUIAction)_commitCancel, NULL);
    status = ProUIPushbuttonActivateActionSet(BATCONVERTUNITDIALOG, BATCONVERTUNITCOMMITAPPLY, (ProUIAction)_commitApply, NULL);

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
            status = ProUITableRownamesSet(BATCONVERTUNITDIALOG, TABLEUNIT, nFiles, row_names);
            status = ProUITableRowlabelsSet(BATCONVERTUNITDIALOG, TABLEUNIT, nFiles, row_labels);

            for (i = 0; i < nFiles; i++)
            {
                memset(fFileName, '\0', sizeof(ProFileName));
                status = ProFilenameParse(fileList[i], rPath, rFileName, rExtension, &nRversion);
                status = ProWstringConcatenate(rFileName, fFileName, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(L".", fFileName, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(rExtension, fFileName, PRO_VALUE_UNUSED);
                status = ProMdlLoad(fFileName, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
                status = ProMdlNameGet(mdl, mdlName);
                status = ProUITableCellLabelSet(BATCONVERTUNITDIALOG, TABLEUNIT, row_names[i], column_names[0], mdlName);

                status = ProMdlPrincipalunitsystemGet(mdl, &unitSystem);
                if (status == PRO_TK_NO_ERROR)
                    status = ProUITableCellLabelSet(BATCONVERTUNITDIALOG, TABLEUNIT, row_names[i], column_names[1], unitSystem.name);
                else
                    status = ProUITableCellLabelSet(BATCONVERTUNITDIALOG, TABLEUNIT, row_names[i], column_names[1], L"未知");

                ProTKSprintf(optionMenu, "%s_%s", row_names[i], column_names[2]);
                status = ProUITableCellComponentCopy(BATCONVERTUNITDIALOG, TABLEUNIT, row_names[i], column_names[2], BATCONVERTUNITDIALOG, OPTIONMENUMETHOD, optionMenu);

                p = wcsstr(unitSystem.name, L"mmNs");
                if (p)
                {
                    status = ProUIOptionmenuDisable(BATCONVERTUNITDIALOG, optionMenu);
                }

                free(row_names[i]);
                free(row_labels[i]);
            }
            free(row_names);
            free(row_labels);
        }
        status = ProArrayFree((ProArray *)&fileList);
        status = ProArrayFree((ProArray *)&dirList);

        status = ProUIDialogActivate(BATCONVERTUNITDIALOG, NULL);
        status = ProUIDialogDestroy(BATCONVERTUNITDIALOG);
    }
}
