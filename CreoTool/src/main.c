#include "./includes/main.h"
#include "./includes/ShowDirectory.h"
#include "./includes/OpenSamenameDrw.h"
#include "./includes/CommonFuns.h"
#include "./includes/CleanWorkDirectory.h"
#include "./includes/TimeSave.h"
#include "./includes/PaintColor.h"
#include "./includes/AlignSymDim.h"
#include "./includes/RenameMdl.h"

char *LastRibbonTab = NULL;

HINT hint;

void ShowAboutDialog()
{

    if (hint == paintColor)
    {
        PaintColor_AfterMacro();
    }
    else
    {
        ShowMessageDialog(1, L"仅供测试，请勿用于商业用途,更不要放到CSDN等处收费下载。\n访问我的博客获得更多信息：\nhttp://www.hudi.site");
    }
    hint = About;
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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_ASSEMBLY)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_DRAWING)
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
    uiCmdCmdId IMI_VerticalAlignmenuID;
    uiCmdCmdId IMI_HorizonAlignmenuID;
    uiCmdCmdId IMI_PurgeWorkDirmenuID;
    uiCmdCmdId IMI_TimeSavemenuID;
    uiCmdCmdId IMI_AboutID;
    uiCmdCmdId IMI_PaintColormenuID;
    uiCmdCmdId IMI_ClearColormenuID;
    uiCmdCmdId IMI_MdlRenamemenuID;
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

    status = ProCmdActionAdd("IMI_VerticalAlign_Act", (uiCmdCmdActFn)VerticalAlign, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_VerticalAlignmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirDRWsubmenu", "IMI_VerticalAlignmenu", "IMI_VerticalAlignmenu", "IMI_VerticalAlignmenuTips", NULL, PRO_B_TRUE, IMI_VerticalAlignmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_HorizonAlign_Act", (uiCmdCmdActFn)HorizonAlign, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_HorizonAlignmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirDRWsubmenu", "IMI_HorizonAlignmenu", "IMI_HorizonAlignmenu", "IMI_HorizonAlignmenuTips", NULL, PRO_B_TRUE, IMI_HorizonAlignmenuID, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Filesubmenu", "IMI_Filesubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PurgeWorkDir_Act", (uiCmdCmdActFn)ProMdlPurgeAll, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_PurgeWorkDirmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenuTips", NULL, PRO_B_TRUE, IMI_PurgeWorkDirmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_MdlRenamemenu_Act", (uiCmdCmdActFn)RenamePrtandDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_MdlRenamemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_MdlRenamemenu", "IMI_MdlRenamemenu", "IMI_MdlRenamemenuTips", NULL, PRO_B_TRUE, IMI_MdlRenamemenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_TimeSave_Act", (uiCmdCmdActFn)ShowTimeSaveDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_TimeSavemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_TimeSavemenu", "IMI_TimeSavemenu", "IMI_TimeSavemenuTips", NULL, PRO_B_TRUE, IMI_TimeSavemenuID, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_PaintColorsubmenu", "IMI_PaintColorsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PaintColor_Act", (uiCmdCmdActFn)PaintColor, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_AboutID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PaintColorsubmenu", "IMI_PaintColormenu", "IMI_PaintColormenu", "IMI_PaintColormenuTips", NULL, PRO_B_TRUE, IMI_AboutID, MSGFILE);

    status = ProCmdActionAdd("IMI_ClearColor_Act", (uiCmdCmdActFn)ClearColor, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_PaintColormenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PaintColorsubmenu", "IMI_ClearColormenu", "IMI_ClearColormenu", "IMI_ClearColormenuTips", NULL, PRO_B_TRUE, IMI_PaintColormenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_About_Act", (uiCmdCmdActFn)ShowAboutDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_PaintColormenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_Aboutmenu", "IMI_Aboutmenu", "IMI_AboutmenuTips", NULL, PRO_B_TRUE, IMI_PaintColormenuID, MSGFILE);

    status = ProNotificationSet(PRO_RIBBON_TAB_SWITCH, (ProFunction)ProRibbonTabSwitchNotification);

    hint = About;

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
    ProError status;
    if (LastRibbonTab != NULL)
        status = ProStringFree(LastRibbonTab);
    status = ProNotificationUnset(PRO_RIBBON_TAB_SWITCH);
}
