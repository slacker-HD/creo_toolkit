#include "./includes/main.h"
#include "./includes/ShowDirectory.h"
#include "./includes/OpenSamenameDrw.h"
#include "./includes/CommonFuns.h"
#include "./includes/CleanWorkDirectory.h"
#include "./includes/TimeSave.h"
#include "./includes/PaintColor.h"
#include "./includes/AlignSymDim.h"
#include "./includes/RenameMdl.h"
#include "./includes/ChangeWorkDirectory.h"
#include "./includes/PrtinAsmRename.h"
#include "./includes/BatOpenFiles.h"
#include "./includes/BatSnapshot.h"
#include "./includes/BattoPdf.h"
#include "./includes/BattoDwg.h"
#include "./includes/AutoDirSetting.h"
#include "./includes/cfg.h"
#include "./includes/FamInstExport.h"
#include "./includes/PartShow.h"
#include "./includes/AutoWorkDirSetting.h"
#include "./includes/LayerSetting.h"
#include "./includes/qrcodegen.h"
#include "./includes/InsertQRCode.h"
#include "./includes/RelFunctions.h"
#include "./includes/OpenSamenameFile.h"
#include "./includes/BatConvertUnit.h"
#include "./includes/CopyInfos.h"
#include "./includes/TextFormat.h"
#include "./includes/TableCellWidth.h"

char *LastRibbonTab = NULL;
ProPath *CurrentWorkDirectoryList;
HINT hint;

UserCheckBut check_but[2]; // 0.定时保存，1.自动切换工作目录为打开模型位置

ProRelfuncArg *Args_StrToDouble = NULL;
ProRelfuncArg *Args_DoubleToStr = NULL;
ProRelfuncArg *Args_StrToInt = NULL;
ProRelfuncArg *Args_IntToStr = NULL;

void ShowAboutDialog()
{
    if (hint == AsmPaintColor)
    {
        AsmPaintColor_AfterMacro();
    }
    else if (hint == PrtPaintColor)
    {
        PrtPaintColor_AfterMacro();
    }
    else
    {
        ShowMessageDialog(1, L"本项目遵循BSD 3-clause许可证发布。\n二维码生成部分修改自https://github.com/nayuki/QR-Code-generator。该项目基于MIT协议发布。\n请不要放到CSDN等处收费下载。\n访问项目主页获得更多信息：\nhttps://github.com/slacker-HD/creo_toolkit/tree/master/CreoTool");
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

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ShowDirectorymenuID;
    uiCmdCmdId IMI_ShowWorkDirmenuID;
    uiCmdCmdId IMI_ChangeWorkDirectorymenuID;
    uiCmdCmdId IMI_OpenSamenameDrwmenuID;
    uiCmdCmdId IMI_VerticalAlignmenuID;
    uiCmdCmdId IMI_HorizonAlignmenuID;
    uiCmdCmdId IMI_PurgeWorkDirmenuID;
    uiCmdCmdId IMI_TimeSavemenuID;
    uiCmdCmdId IMI_AboutmenuID;
    uiCmdCmdId IMI_PaintColormenuID;
    uiCmdCmdId IMI_ClearColormenuID;
    uiCmdCmdId IMI_MdlRenamemenuID;
    uiCmdCmdId IMI_OpenSamenameFilemenuID;

    uiCmdCmdId IMI_BatOPenPrtmenuID;
    uiCmdCmdId IMI_BatOPenDrwmenuID;
    uiCmdCmdId IMI_BatSnapShotmenuID;
    uiCmdCmdId IMI_BatExportPdfmenuID;
    uiCmdCmdId IMI_BatExportDwgmenuID;
    uiCmdCmdId IMI_ExportFamInstsmenuID;
    uiCmdCmdId IMI_BatConvertUnitID;

    uiCmdCmdId IMI_HideSelectedPartmenuID;
    uiCmdCmdId IMI_HideUnselectedPartmenuID;
    uiCmdCmdId IMI_ShowAllPartmenuID;

    uiCmdCmdId IMI_LayerSetmenuID;
    uiCmdCmdId IMI_InsertQRCodemenuID;
    uiCmdCmdId IMI_FormatTextmenuID;

    uiCmdCmdId IMI_CopyNamemenuID;
    uiCmdCmdId IMI_CopyPathmenuID;
    uiCmdCmdId IMI_CopyFullNamemenuID;

    uiCmdCmdId IMI_PrtPaintColormenuID;
    uiCmdCmdId IMI_PrtSettransSurfacemenuID;

    uiCmdCmdId IMI_TextWrapmenuID;

    ProPath currentPath;
    ProPath exePath;
    ProPath cfgPath;
    int n_size;

    wchar_t ifMemPath[256] = L"True";
    wchar_t ifAutoChangeWorkDir[256] = L"False";
    wchar_t lastPath[256] = L"";
    int valueLength;
    int compResult;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "IMI_Mainmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_DirToolsubmenu", "IMI_DirToolsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowDirectory_Act", (uiCmdCmdActFn)ShowDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowDirectorymenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirToolsubmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenu", "IMI_ShowDirmenutips", NULL, PRO_B_TRUE, IMI_ShowDirectorymenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ShowDirectorymenuID, "IMI_ShowDirectorymenuID.png");

    status = ProCmdActionAdd("IMI_ShowWorkDirectory_Act", (uiCmdCmdActFn)ShowWorkDirectory, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowWorkDirmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirToolsubmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenu", "IMI_ShowWorkDirmenutips", NULL, PRO_B_TRUE, IMI_ShowWorkDirmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ShowWorkDirmenuID, "IMI_ShowWorkDirectoryID.png");

    status = ProCmdActionAdd("IMI_ChangeWorkDirectory_Act", (uiCmdCmdActFn)ShowChangeWorkDirectoryDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_ChangeWorkDirectorymenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DirToolsubmenu", "IMI_ChangeWorkDirectorymenu", "IMI_ChangeWorkDirectorymenu", "IMI_ChangeWorkDirectorymenutips", NULL, PRO_B_TRUE, IMI_ChangeWorkDirectorymenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ChangeWorkDirectorymenuID, "IMI_ChangeWorkDirectorymenuID.png");

    status = ProCmdOptionAdd("IMI_AutoDirSettingChkMenu_Act", (uiCmdCmdActFn)AutoDirSettingFn, PRO_B_TRUE, (uiCmdCmdValFn)AutoDirSettingValueFn, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &(check_but[0].command));
    status = ProMenubarmenuChkbuttonAdd("IMI_DirToolsubmenu", "IMI_AutoDirSettingChkMenu", "IMI_AutoDirSettingChkMenu", "IMI_AutoDirSettingChkMenu", "IMI_TimeSave_Act", PRO_B_TRUE, check_but[0].command, MSGFILE);

    status = ProCmdOptionAdd("IMI_AutoWorkDirSettingChkMenu_Act", (uiCmdCmdActFn)AutoWorkDirSettingFn, PRO_B_TRUE, (uiCmdCmdValFn)AutoWorkDirSettingValueFn, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &(check_but[1].command));
    status = ProMenubarmenuChkbuttonAdd("IMI_DirToolsubmenu", "IMI_AutoWorkDirSettingChkMenu", "IMI_AutoWorkDirSettingChkMenu", "IMI_AutoWorkDirSettingChkMenu", "IMI_AutoWorkDirSetting_Act", PRO_B_TRUE, check_but[1].command, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_DRWsubmenu", "IMI_DRWsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_OpenSamenameDrw_Act", (uiCmdCmdActFn)OpenSamenameDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_OpenSamenameDrwmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenu", "IMI_OpenSamenameDrwmenutips", NULL, PRO_B_TRUE, IMI_OpenSamenameDrwmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_OpenSamenameDrwmenuID, "IMI_OpenSamenameDrwmenuID.png");

    status = ProCmdActionAdd("IMI_VerticalAlign_Act", (uiCmdCmdActFn)VerticalAlign, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_VerticalAlignmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_VerticalAlignmenu", "IMI_VerticalAlignmenu", "IMI_VerticalAlignmenutips", NULL, PRO_B_TRUE, IMI_VerticalAlignmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_VerticalAlignmenuID, "IMI_VerticalAlignmenuID.png");

    status = ProCmdActionAdd("IMI_HorizonAlign_Act", (uiCmdCmdActFn)HorizonAlign, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_HorizonAlignmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_HorizonAlignmenu", "IMI_HorizonAlignmenu", "IMI_HorizonAlignmenutips", NULL, PRO_B_TRUE, IMI_HorizonAlignmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_HorizonAlignmenuID, "IMI_HorizonAlignmenuID.png");

    status = ProCmdActionAdd("IMI_LayerSet_Act", (uiCmdCmdActFn)CreateLayers, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_LayerSetmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_LayerSetmenu", "IMI_LayerSetmenu", "IMI_LayerSetmenutips", NULL, PRO_B_TRUE, IMI_LayerSetmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_LayerSetmenuID, "IMI_LayerSetmenuID.png");

    status = ProCmdActionAdd("IMI_InsertQRCode_Act", (uiCmdCmdActFn)ShowQRCodeDialog, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_InsertQRCodemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_InsertQRCodemenu", "IMI_InsertQRCodemenu", "IMI_InsertQRCodemenutips", NULL, PRO_B_TRUE, IMI_InsertQRCodemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_InsertQRCodemenuID, "IMI_InsertQRCodemenuID.png");

    status = ProCmdActionAdd("IMI_FormatText_Act", (uiCmdCmdActFn)FormatText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_FormatTextmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenu", "IMI_FormatTextmenutips", NULL, PRO_B_TRUE, IMI_FormatTextmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_FormatTextmenuID, "IMI_FormatTextmenuID.png");

    status = ProCmdActionAdd("IMI_TextWrap_Act", (uiCmdCmdActFn)WrapText, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_TextWrapmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DRWsubmenu", "IMI_TextWrapmenu", "IMI_TextWrapmenu", "IMI_TextWrapmenutips", NULL, PRO_B_TRUE, IMI_TextWrapmenuID, MSGFILE);

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_Filesubmenu", "IMI_Filesubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PurgeWorkDir_Act", (uiCmdCmdActFn)ProMdlPurgeAll, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_PurgeWorkDirmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenu", "IMI_PurgeWorkDirmenutips", NULL, PRO_B_TRUE, IMI_PurgeWorkDirmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_PurgeWorkDirmenuID, "IMI_PurgeWorkDirmenuID.png");

    status = ProCmdActionAdd("IMI_MdlRenamemenu_Act", (uiCmdCmdActFn)RenamePrtandDrw, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_MdlRenamemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_MdlRenamemenu", "IMI_MdlRenamemenu", "IMI_MdlRenamemenutips", NULL, PRO_B_TRUE, IMI_MdlRenamemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_MdlRenamemenuID, "IMI_MdlRenamemenuID.png");

    status = ProCmdActionAdd("IMI_TimeSave_Act", (uiCmdCmdActFn)ShowTimeSaveDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_TimeSavemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_TimeSavemenu", "IMI_TimeSavemenu", "IMI_TimeSavemenutips", NULL, PRO_B_TRUE, IMI_TimeSavemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_TimeSavemenuID, "IMI_TimeSavemenuID.png");

    status = ProCmdActionAdd("IMI_OpenSamenameFile_Act", (uiCmdCmdActFn)OpenSamenameFile, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_OpenSamenameFilemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_OpenSamenameFilemenu", "IMI_OpenSamenameFilemenu", "IMI_OpenSamenameFilemenutips", NULL, PRO_B_TRUE, IMI_OpenSamenameFilemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_OpenSamenameFilemenuID, "IMI_OpenSamenameFilemenuID.png");

    status = ProCmdActionAdd("IMI_CopyName_Act", (uiCmdCmdActFn)CopyCurrentFileName, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_CopyNamemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_CopyNamemenu", "IMI_CopyNamemenu", "IMI_CopyNamemenutips", NULL, PRO_B_TRUE, IMI_CopyNamemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_CopyNamemenuID, "IMI_CopyNamemenuID.png");

    status = ProCmdActionAdd("IMI_CopyPath_Act", (uiCmdCmdActFn)CopyCurrentFilePath, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_CopyPathmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_CopyPathmenu", "IMI_CopyPathmenu", "IMI_CopyPathmenutips", NULL, PRO_B_TRUE, IMI_CopyPathmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_CopyPathmenuID, "IMI_CopyPathmenuID.png");

    status = ProCmdActionAdd("IMI_CopyFullName_Act", (uiCmdCmdActFn)CopyCurrentFileFullName, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_CopyFullNamemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Filesubmenu", "IMI_CopyFullNamemenu", "IMI_CopyFullNamemenu", "IMI_CopyFullNamemenutips", NULL, PRO_B_TRUE, IMI_CopyFullNamemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_CopyFullNamemenuID, "IMI_CopyFullNamemenuID.png");

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_PaintColorsubmenu", "IMI_PaintColorsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PaintColor_Act", (uiCmdCmdActFn)PaintColorAsm, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_PaintColormenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PaintColorsubmenu", "IMI_PaintColormenu", "IMI_PaintColormenu", "IMI_PaintColormenutips", NULL, PRO_B_TRUE, IMI_PaintColormenuID, MSGFILE);
    status = ProCmdIconSet(IMI_PaintColormenuID, "IMI_PaintColormenuID.png");

    status = ProCmdActionAdd("IMI_ClearColor_Act", (uiCmdCmdActFn)ClearColor, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_ClearColormenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PaintColorsubmenu", "IMI_ClearColormenu", "IMI_ClearColormenu", "IMI_ClearColormenutips", NULL, PRO_B_TRUE, IMI_ClearColormenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ClearColormenuID, "IMI_ClearColormenuID.png");

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_BatOPensubmenu", "IMI_BatOPensubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BatOPenPrt_Act", (uiCmdCmdActFn)OpenPrts, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatOPenPrtmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatOPensubmenu", "IMI_BatOPenPrtmenu", "IMI_BatOPenPrtmenu", "IMI_BatOPenPrtmenutips", NULL, PRO_B_TRUE, IMI_BatOPenPrtmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_BatOPenPrtmenuID, "IMI_BatOPenPrtmenuID.png");

    status = ProCmdActionAdd("IMI_BatOPenDrw_Act", (uiCmdCmdActFn)OpenDrws, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatOPenDrwmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatOPensubmenu", "IMI_BatOPenDrwmenu", "IMI_BatOPenDrwmenu", "IMI_BatOPenDrwmenutips", NULL, PRO_B_TRUE, IMI_BatOPenDrwmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_BatOPenDrwmenuID, "IMI_BatOPenDrwmenuID.png");

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_BatExportmenu", "IMI_BatExportmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BatSnapShot_Act", (uiCmdCmdActFn)BatSnapShot, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatSnapShotmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_BatSnapShotmenu", "IMI_BatSnapShotmenu", "IMI_BatSnapShotmenutips", NULL, PRO_B_TRUE, IMI_BatSnapShotmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_BatSnapShotmenuID, "IMI_BatSnapShotmenuID.png");

    status = ProCmdActionAdd("IMI_BatExportPdf_Act", (uiCmdCmdActFn)BatToPdf, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatExportPdfmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_BatExportPdfmenu", "IMI_BatExportPdfmenu", "IMI_BatExportPdfmenutips", NULL, PRO_B_TRUE, IMI_BatExportPdfmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_BatExportPdfmenuID, "IMI_BatExportPdfmenuID.png");

    status = ProCmdActionAdd("IMI_BatExportDwg_Act", (uiCmdCmdActFn)BatToDwg, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatExportDwgmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_BatExportDwgmenu", "IMI_BatExportDwgmenu", "IMI_BatExportDwgmenutips", NULL, PRO_B_TRUE, IMI_BatExportDwgmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_ExportFamInsts_Act", (uiCmdCmdActFn)ExportFamInsts, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_ExportFamInstsmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_ExportFamInstsmenu", "IMI_ExportFamInstsmenu", "IMI_ExportFamInstsmenutips", NULL, PRO_B_TRUE, IMI_ExportFamInstsmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ExportFamInstsmenuID, "IMI_ExportFamInstsmenuID.png");

    status = ProCmdActionAdd("IMI_BatConvertUnit_Act", (uiCmdCmdActFn)ShowBatConvertUnitDialogDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatConvertUnitID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatExportmenu", "IMI_BatConvertUnitmenu", "IMI_BatConvertUnitmenu", "IMI_BatConvertUnitmenutips", NULL, PRO_B_TRUE, IMI_BatConvertUnitID, MSGFILE);
    status = ProCmdIconSet(IMI_ExportFamInstsmenuID, "IMI_ExportFamInstsmenuID.png");

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_PartShowmenu", "IMI_PartShowmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_HideSelectedPart_Act", (uiCmdCmdActFn)HideSelectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideSelectedPartmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PartShowmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideSelectedPartmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_HideSelectedPartmenuID, "IMI_HideSelectedPartmenuID.png");

    status = ProCmdActionAdd("IMI_HideUnselectedPart_Act", (uiCmdCmdActFn)HideUnselectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideUnselectedPartmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PartShowmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideUnselectedPartmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_HideUnselectedPartmenuID, "IMI_HideUnselectedPartmenuID.png");

    status = ProCmdActionAdd("IMI_ShowAllPart_Act", (uiCmdCmdActFn)RestoreSimpRep, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowAllPartmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PartShowmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenutips", NULL, PRO_B_TRUE, IMI_ShowAllPartmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_ShowAllPartmenuID, "IMI_ShowAllPartmenuID.png");

    status = ProMenubarmenuMenuAdd("IMI_Mainmenu", "IMI_PrtOrpsubmenu", "IMI_PrtOrpsubmenu", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_PrtPaintColor_Act", (uiCmdCmdActFn)PaintColorPrt, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_PrtPaintColormenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PrtOrpsubmenu", "IMI_PrtPaintColormenu", "IMI_PrtPaintColormenu", "IMI_PrtPaintColormenutips", NULL, PRO_B_TRUE, IMI_PrtPaintColormenuID, MSGFILE);
    status = ProCmdIconSet(IMI_PrtPaintColormenuID, "IMI_PrtPaintColormenuID.png");

    status = ProCmdActionAdd("IMI_PrtSettransSurface_Act", (uiCmdCmdActFn)PrtSettransSurface, uiProeImmediate, AccessPRTorASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_PrtSettransSurfacemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_PrtOrpsubmenu", "IMI_PrtSettransSurfacemenu", "IMI_PrtSettransSurfacemenu", "IMI_PrtSettransSurfacemenutips", NULL, PRO_B_TRUE, IMI_PrtSettransSurfacemenuID, MSGFILE);
    status = ProCmdIconSet(IMI_PrtSettransSurfacemenuID, "IMI_PrtSettransSurfacemenuID.png");

    status = ProCmdActionAdd("IMI_About_Act", (uiCmdCmdActFn)ShowAboutDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_AboutmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_Aboutmenu", "IMI_Aboutmenu", "IMI_Aboutmenutips", NULL, PRO_B_TRUE, IMI_AboutmenuID, MSGFILE);
    status = ProCmdIconSet(IMI_AboutmenuID, "IMI_AboutmenuID.png");

    status = AsmTreePrtinAsmRenamePopupmenusSetup();
    status = AsmTreePrtinAsmShowOrHidePopupmenusSetup();
    status = AsmTreeDrwOpenPopupmenusSetup();
    status = AsmTreeComponentDirectoryOpenPopupmenusSetup();

    status = ProNotificationSet(PRO_RIBBON_TAB_SWITCH, (ProFunction)ProRibbonTabSwitchNotification);
    status = ProNotificationSet(PRO_DIRECTORY_CHANGE_POST, (ProFunction)ProDirectoryChangeNotification);

    status = ProToolkitApplTextPathGet(exePath);
    status = ProWstringCopy(exePath, cfgPath, PRO_VALUE_UNUSED);
    status = ProWstringConcatenate(L"\\CreoTool.ini", cfgPath, PRO_VALUE_UNUSED);

    status = ProDirectoryCurrentGet(currentPath);

    if (ReadConfig(cfgPath, L"LastPath", currentPath, &valueLength) != 0)
    {
        WriteOrUpdateConfig(cfgPath, L"LastPath", currentPath);
    }

    if (ReadConfig(cfgPath, L"AutoChangePath", ifMemPath, &valueLength) != 0)
    {
        WriteOrUpdateConfig(cfgPath, L"AutoChangePath", L"False");
    }

    status = ProWstringCompare(L"True", ifMemPath, PRO_VALUE_UNUSED, &compResult);
    if (compResult == 0)
    {
        check_but[0].state = PRO_B_TRUE;
        status = ProDirectoryChange(currentPath);
    }
    else
    {
        check_but[0].state = PRO_B_FALSE;
    }

    if (ReadConfig(cfgPath, L"AutoChangeWorkDir", ifAutoChangeWorkDir, &valueLength) != 0)
    {
        WriteOrUpdateConfig(cfgPath, L"AutoChangeWorkDir", L"False");
    }

    status = ProWstringCompare(L"True", ifAutoChangeWorkDir, PRO_VALUE_UNUSED, &compResult);
    if (compResult == 0)
    {
        check_but[1].state = PRO_B_TRUE;
        status = ProNotificationSet(PRO_WINDOW_CHANGE_POST, ProUserWindowChangePost); // 切换窗口导致的当前模型变化
        status = ProNotificationSet(PRO_MDL_SAVE_POST, ProUserMdlSavePost);           // 另存为导致的路径变化
        status = ProNotificationSet(PRO_MDL_RETRIEVE_POST, ProUserMdlRetrievePost);   // 打开新模型导致的路径变化
    }
    else
    {
        check_but[1].state = PRO_B_FALSE;
    }

    status = ProDirectoryCurrentGet(currentPath);
    status = ProWstringLengthGet(currentPath, &n_size);
    currentPath[n_size - 1] = '\0';
    status = ProArrayAlloc(1, sizeof(ProPath), 1, (ProArray *)&CurrentWorkDirectoryList);
    status = ProWstringCopy(currentPath, CurrentWorkDirectoryList[0], PRO_VALUE_UNUSED);
    hint = About;

    status = ProArrayAlloc(1, sizeof(ProRelfuncArg), 1, (ProArray *)&Args_StrToDouble);
    Args_StrToDouble[0].type = PRO_PARAM_STRING;
    Args_StrToDouble[0].attributes = PRO_RELF_ATTR_NONE;

    status = ProArrayAlloc(1, sizeof(ProRelfuncArg), 1, (ProArray *)&Args_DoubleToStr);
    Args_DoubleToStr[0].type = PRO_PARAM_DOUBLE;
    Args_DoubleToStr[0].attributes = PRO_RELF_ATTR_NONE;

    status = ProArrayAlloc(1, sizeof(ProRelfuncArg), 1, (ProArray *)&Args_StrToInt);
    Args_StrToInt[0].type = PRO_PARAM_STRING;
    Args_StrToInt[0].attributes = PRO_RELF_ATTR_NONE;

    status = ProArrayAlloc(1, sizeof(ProRelfuncArg), 1, (ProArray *)&Args_IntToStr);
    Args_IntToStr[0].type = PRO_PARAM_INTEGER;
    Args_IntToStr[0].attributes = PRO_RELF_ATTR_NONE;

    status = ProRelationFunctionRegister("IMI_StrToDouble", Args_StrToDouble, StrToDouble, NULL, NULL, PRO_B_FALSE, NULL);
    status = ProRelationFunctionRegister("IMI_DoubleToStr", Args_DoubleToStr, DoubleToStr, NULL, NULL, PRO_B_FALSE, NULL);
    status = ProRelationFunctionRegister("IMI_StrToInt", Args_StrToInt, StrToInt, NULL, NULL, PRO_B_FALSE, NULL);
    status = ProRelationFunctionRegister("IMI_IntToStr", Args_IntToStr, IntToStr, NULL, NULL, PRO_B_FALSE, NULL);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
    ProError status;
    ProPath currentPath;
    ProPath exePath;
    ProPath cfgPath;
    if (LastRibbonTab != NULL)
    {
        status = ProStringFree(LastRibbonTab);
    }
    status = ProNotificationUnset(PRO_RIBBON_TAB_SWITCH);
    status = ProNotificationUnset(PRO_DIRECTORY_CHANGE_POST);
    if (check_but[1].state == PRO_B_TRUE)
    {
        status = ProNotificationUnset(PRO_WINDOW_CHANGE_POST);
        status = ProNotificationUnset(PRO_MDL_SAVE_POST);
        status = ProNotificationUnset(PRO_MDL_RETRIEVE_POST);
    }

    status = ProArrayFree((ProArray *)&CurrentWorkDirectoryList);
    status = ProToolkitApplTextPathGet(exePath);
    status = ProWstringCopy(exePath, cfgPath, PRO_VALUE_UNUSED);
    status = ProWstringConcatenate(L"\\CreoTool.ini", cfgPath, PRO_VALUE_UNUSED);
    status = ProDirectoryCurrentGet(currentPath);
    if (status == PRO_TK_NO_ERROR)
    {
        WriteOrUpdateConfig(cfgPath, L"LastPath", currentPath);
    }

    status = ProArrayFree((ProArray *)&Args_StrToDouble);
    Args_StrToDouble = NULL;
    status = ProArrayFree((ProArray *)&Args_DoubleToStr);
    Args_DoubleToStr = NULL;
    status = ProArrayFree((ProArray *)&Args_StrToInt);
    Args_StrToInt = NULL;
    status = ProArrayFree((ProArray *)&Args_IntToStr);
    Args_IntToStr = NULL;
}
