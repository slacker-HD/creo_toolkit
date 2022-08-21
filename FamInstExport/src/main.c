#include "./includes/main.h"

int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdlType;
    ProError status;
    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(mdl, &mdlType);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return mdlType;
}

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

ProError famtableInstanceAction(ProFaminstance *instance, ProError status, ProAppData app_data)
{
    ProMdl mdl, newMdl;
    ProName name;
    status = ProFaminstanceRetrieve(instance, &mdl);
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

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ExportFamInstsID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProCmdActionAdd("IMI_ExportFamInsts_Act", (uiCmdCmdActFn)ExportFamInsts, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_ExportFamInstsID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_ExportFamInstsmenu", "IMI_ExportFamInstsmenu", "IMI_ExportFamInstsmenutips", NULL, PRO_B_TRUE, IMI_ExportFamInstsID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
