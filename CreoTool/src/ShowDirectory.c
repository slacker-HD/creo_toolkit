#include "includes/ShowDirectory.h"

void ShowDirectory()
{
    ProError status;
    ProMdl mdl;
    ProMdldata data;
    wchar_t path[TRUEPATHSIZE];
    status = ProMdlCurrentGet(&mdl);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlDataGet(mdl, &data);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProWstringCopy(data.device, path, PRO_VALUE_UNUSED);
            status = ProWstringConcatenate(L":", path, PRO_VALUE_UNUSED);
            status = ProWstringConcatenate(data.path, path, PRO_VALUE_UNUSED);
            ShellExecuteW(NULL, L"open", L"explorer.exe", path, NULL, SW_SHOWNORMAL);
        }
    }
}

void ShowWorkDirectory()
{
    ProError status;
    ProPath path;
    status = ProDirectoryCurrentGet(path);
    ShellExecuteW(NULL, L"open", L"explorer.exe", path, NULL, SW_SHOWNORMAL);
}

void ShowComponentDirectory()
{

    ProError status;
    ProSelection *sels;
    ProFeature asmcomp;
    ProMdldata mdldata;

    status = ProSelbufferSelectionsGet(&sels);
    status = ProSelectionModelitemGet(sels[0], &asmcomp);

    if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
    {

        status = ProMdlDataGet(asmcomp.owner, &mdldata);
        ShellExecuteW(NULL, L"open", L"explorer.exe", mdldata.path, NULL, SW_SHOWNORMAL);
        status = ProSelectionarrayFree(sels);
    }
}

ProError AsmTreeComponentDirectoryOpenPopupmenusSetup()
{
    ProError status;
    uiCmdCmdId opencomponentdirectory_cmd_id;
    status = ProCmdActionAdd("IMI_ComponentDirectoryOpen", (uiCmdCmdActFn)ShowComponentDirectory, uiProe2ndImmediate, ComponentInASMTreeAccess, PRO_B_FALSE, PRO_B_FALSE, &opencomponentdirectory_cmd_id);
    status = ProMenubarmenuPushbuttonAdd("ActionMenu", "IMI_ComponentDirectoryOpen_Act", "IMI_OpenComponentDirectory", "IMI_OpenComponentDirectorytips", NULL, PRO_B_TRUE, opencomponentdirectory_cmd_id, MSGFILE);
    return PRO_TK_NO_ERROR;
}