#include "./includes/main.h"
#include "./includes/ShowDirectory.h"
#include "./includes/OpenSamenameDrw.h"
#include "./includes/CommonFuns.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

static uiCmdAccessState AccessPRTorASM(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_ASSEMBLY || CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ShowDirectoryID;
    uiCmdCmdId IMI_ShowWorkDirmenuID;
    uiCmdCmdId IMI_OpenSamenameDrwmenuID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "IMI_Mainmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowDirectory_Act", (uiCmdCmdActFn)ShowDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowDirectoryID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenuTips", NULL, PRO_B_TRUE, IMI_ShowDirectoryID, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowWorkDirectory_Act", (uiCmdCmdActFn)ShowWorkDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowWorkDirmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenuTips", NULL, PRO_B_TRUE, IMI_ShowWorkDirmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_OpenSamenameDrw_Act", (uiCmdCmdActFn)OpenSamenameDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_OpenSamenameDrwmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenuTips", NULL, PRO_B_TRUE, IMI_OpenSamenameDrwmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
