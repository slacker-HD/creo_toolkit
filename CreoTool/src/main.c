#include "./includes/main.h"
#include "./includes/ShowDirectory.h"
#include "./includes/OpenSamenameDrw.h"
#include "./includes/CommonFuns.h"
#include "./includes/CleanWorkDirectory.h"
#include "./includes/TimeSave.h"

void ShowAboutDialog()
{
    ShowMessageDialog(1, L"仅供测试，请勿用于商业用途,更不要放到CSDN等处收费下载。\n访问我的博客获得更多信息：\nhttp://www.hudi.site");
}

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
    uiCmdCmdId IMI_PurgeWorkDirmenuID;
    uiCmdCmdId IMI_TimeSaveSettingmenuID;
    uiCmdCmdId IMI_AboutID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "IMI_Mainmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_DirToolsubmenu", "IMI_DirToolsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowDirectory_Act", (uiCmdCmdActFn)ShowDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowDirectoryID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirToolsubmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenuTips", NULL, PRO_B_TRUE, IMI_ShowDirectoryID, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowWorkDirectory_Act", (uiCmdCmdActFn)ShowWorkDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowWorkDirmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirToolsubmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenuTips", NULL, PRO_B_TRUE, IMI_ShowWorkDirmenuID, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_DirDRWsubmenu", "IMI_DirDRWsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_OpenSamenameDrw_Act", (uiCmdCmdActFn)OpenSamenameDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_OpenSamenameDrwmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirDRWsubmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenuTips", NULL, PRO_B_TRUE, IMI_OpenSamenameDrwmenuID, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Filesubmenu", "IMI_Filesubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PurgeWorkDir_Act", (uiCmdCmdActFn)ProMdlPurgeAll, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_AboutID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenuTips", NULL, PRO_B_TRUE, IMI_AboutID, MSGFILE);

    status = ProCmdActionAdd("IMI_TimeSaveSetting_Act", (uiCmdCmdActFn)ShowTimeSaveDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_TimeSaveSettingmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_TimeSaveSettingmenu", "IMI_TimeSaveSettingmenu", "IMI_TimeSaveSettingmenuTips", NULL, PRO_B_TRUE, IMI_TimeSaveSettingmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_About_Act", (uiCmdCmdActFn)ShowAboutDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowDirectoryID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_Aboutmenu", "IMI_Aboutmenu", "IMI_AboutmenuTips", NULL, PRO_B_TRUE, IMI_ShowDirectoryID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
