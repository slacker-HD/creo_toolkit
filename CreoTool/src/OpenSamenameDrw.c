#include "./includes/OpenSamenameDrw.h"

void OpenSamenameDrw()
{
    ProError status;
    ProMdl mdl;
    ProDrawing drawing;
    ProName name;
    int new_win_id;
    status = ProMdlCurrentGet(&mdl);
    status = ProMdlNameGet(mdl, name);
    status = ProMdlLoad(name, PRO_MDL_DRAWING, PRO_B_FALSE, (ProMdl *)&drawing);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProObjectwindowCreate(name, PRO_DRAWING, &new_win_id);
        status = ProMdlDisplay(drawing);
        status = ProWindowActivate(new_win_id);
    }
    else
    {
        ShowMessageDialog(1, L"当前工作目录下未发现同名工程图");
    }
}

void openDrwinAsm()
{
    ProError status;
    ProSelection *sels;
    ProFeature asmcomp;
    ProMdl asm;
    ProMdldata mdldata;
    ProPath currentPath;
    ProName newName;
    ProDrawing drawing;
    int new_win_id;

    status = ProSelbufferSelectionsGet(&sels);
    status = ProSelectionModelitemGet(sels[0], &asmcomp);

    if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
    {

        status = ProMdlCurrentGet(&asm);

        status = ProMdlDataGet(asmcomp.owner, &mdldata);
        status = ProDirectoryCurrentGet(currentPath);
        status = ProDirectoryChange(mdldata.path);

        status = ProMdlLoad(mdldata.name, PRO_MDL_DRAWING, PRO_B_FALSE, (ProMdl *)&drawing);

        if (status == PRO_TK_NO_ERROR)
        {
            status = ProObjectwindowCreate(mdldata.name, PRO_DRAWING, &new_win_id);
            status = ProMdlDisplay(drawing);
            status = ProWindowActivate(new_win_id);
        }
        else
        {
            ShowMessageDialog(1, L"模型目录下未发现同名工程图");
        }

        status = ProDirectoryChange(currentPath);
        status = ProSelectionarrayFree(sels);
    }
}

ProError AsmTreeDrwOpenPopupmenusSetup()
{
    ProError status;
    uiCmdCmdId opendrw_cmd_id;
    status = ProCmdActionAdd("IMI_DrwinAsmOpen", (uiCmdCmdActFn)openDrwinAsm, uiProe2ndImmediate, ComponentInASMTreeAccess, PRO_B_FALSE, PRO_B_FALSE, &opendrw_cmd_id);
    status = ProMenubarmenuPushbuttonAdd("ActionMenu", "IMI_DRWinAsmOpen_Act", "IMI_OpenDrwinAsm", "IMI_OpenDrwinAsmtips", NULL, PRO_B_TRUE, opendrw_cmd_id, MSGFILE);
    return PRO_TK_NO_ERROR;
}
