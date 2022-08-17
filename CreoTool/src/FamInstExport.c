#include "./includes/FamInstExport.h"

ProError famtableInstanceAction(ProFaminstance *instance, ProError status, ProAppData app_data)
{
    ProMdl mdl, newMdl;
    ProName name;
    status = ProFaminstanceRetrieve(instance, &mdl);
    if (status != PRO_TK_NO_ERROR)
    {
        ShowMessageDialog(1, L"当前族表有错，请确认。");
    }
    status = ProMdlNameGet(mdl, name);
    status = ProMdlCopy(mdl, name, &newMdl);
    status = ProMdlErase(mdl);
    return PRO_TK_NO_ERROR;
}

void ExportFamInsts()
{
    ProError status;
    ProPath famFile;
    ProMdl mdl, newMdl;
    ProName name;
    ProFamtable famtab;
    int i, n_size;

    status = ProFileOpen(NULL, L"*.prt", NULL, NULL, NULL, NULL, famFile);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlEraseNotDisplayed();
        status = ProMdlLoad(famFile, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
        status = ProMdlNameGet(mdl, name);

        status = ProFamtableInit(mdl, &famtab);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProFamtableInstanceVisit(&famtab, (ProFamtableInstanceAction)famtableInstanceAction, NULL, NULL);
            status = ProFamtableErase(&famtab);

            status = ProWstringConcatenate(L"_orig", name, PRO_VALUE_UNUSED);
            status = ProMdlCopy(mdl, name, &newMdl);
            status = ProMdlErase(mdl);
        }
    }
}
