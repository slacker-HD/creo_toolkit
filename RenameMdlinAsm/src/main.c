#include "./includes/main.h"

ProError renameDrwWithSameName(ProName oldName, ProName newName)
{
    ProError status;
    ProMdlType mdlType = PRO_MDL_DRAWING;
    ProMdl mdl;
    status = ProMdlRetrieve(oldName, mdlType, &mdl);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlRename(mdl, newName);
        status = ProMdlSave(mdl);
    }
    return status;
}

ProError renameMdlinAsm()
{
    ProError status;
    ProSelection *sels;
    ProFeature asmcomp;
    ProMdl asm;
    ProMdldata mdldata;
    ProPath currentPath;
    ProName newName;

    status = ProSelbufferSelectionsGet(&sels);
    status = ProSelectionModelitemGet(sels[0], &asmcomp);

    if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
    {
        status = ProMessageDisplay(MSGFILE, "IMI_Enter_New_Name");
        status = ProMessageStringRead(PRO_NAME_SIZE, newName);

        if (status == PRO_TK_NO_ERROR)
        {
            status = ProMdlCurrentGet(&asm);

            status = ProMdlDataGet(asmcomp.owner, &mdldata);
            status = ProDirectoryCurrentGet(currentPath);
            status = ProDirectoryChange(mdldata.path);

            status = renameDrwWithSameName(mdldata.name, newName);
            status = ProMdlRename(asmcomp.owner, newName);
            status = ProMdlSave(asmcomp.owner);

            status = ProMdlDataGet(asm, &mdldata);
            status = ProDirectoryChange(mdldata.path);
            status = ProMdlSave(asm);

            status = ProDirectoryChange(currentPath);
        }
        ProSelectionarrayFree(sels);
    }
    return PRO_TK_NO_ERROR;
}
uiCmdAccessState renameMdlAccess(uiCmdAccessMode mode)
{
    uiCmdAccessState access_result;
    ProError status;
    ProSelection *sels;
    int size;

    access_result = ACCESS_REMOVE;

    status = ProSelbufferSelectionsGet(&sels);
    if (status != PRO_TK_NO_ERROR)
        return access_result;

    status = ProArraySizeGet(sels, &size);
    if (status != PRO_TK_NO_ERROR)
        return access_result;

    if (size == 1)
    {
        ProAsmcomp asmcomp;
        status = ProSelectionModelitemGet(sels[0], &asmcomp);
        if (asmcomp.type == PRO_FEATURE)
        {
            ProFeattype ftype;
            status = ProFeatureTypeGet(&asmcomp, &ftype);
            if (ftype == PRO_FEAT_COMPONENT)
            {
                access_result = ACCESS_AVAILABLE;
            }
        }

        if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
        {
            ProAsmcomppath path;
            status = ProSelectionAsmcomppathGet(sels[0], &path);
            if (path.table_num > 0)
            {
                access_result = ACCESS_AVAILABLE;
            }
        }
    }
    ProSelectionarrayFree(sels);
    return access_result;
}

ProError AsmTreePrtinAsmRenamePopupmenusSetup()
{
    ProError status;
    uiCmdCmdId rename_cmd_id;
    status = ProCmdActionAdd("IMI_MdlinAsmRename", (uiCmdCmdActFn)renameMdlinAsm, uiProe2ndImmediate, renameMdlAccess, PRO_B_FALSE, PRO_B_FALSE, &rename_cmd_id);
    status = ProMenubarmenuPushbuttonAdd("ActionMenu", "IMI_MdlinAsmRename_Act", "IMI_RenameMdlinAsm", "IMI_RenameMdlinAsmtips", NULL, PRO_B_TRUE, rename_cmd_id, MSGFILE);
    return PRO_TK_NO_ERROR;
}

int user_initialize()
{
    ProError status;
    status = AsmTreePrtinAsmRenamePopupmenusSetup();
    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
