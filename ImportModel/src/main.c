#include <windows.h>

#include "./includes/main.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}
void ImportModel()
{
    ProError status;
    ProPath selected_file;
    int new_win_id;
    ProMdl mdl;
    ProName name = L"stpfile";
    if (ProFileOpen(L"选择STP文件", L"*.stp,*.step", NULL, NULL, NULL, NULL, selected_file) == PRO_TK_NO_ERROR)
    {
        status = ProIntfimportModelCreate(selected_file, PRO_INTF_IMPORT_STEP, PRO_MDL_PART, name, NULL, NULL, &mdl);
        status = ProObjectwindowCreate(name, PRO_MDL_PART, &new_win_id);
        status = ProMdlDisplay(mdl);
        
        status = ProWindowActivate(new_win_id);
    }
}
int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ImportSTPModelID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProCmdActionAdd("IMI_ImportSTPModel_Act", (uiCmdCmdActFn)ImportModel, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ImportSTPModelID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_ImportSTPModelmenu", "IMI_ImportSTPModelmenu", "IMI_ImportSTPModelSTPmenutips", NULL, PRO_B_TRUE, IMI_ImportSTPModelID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}