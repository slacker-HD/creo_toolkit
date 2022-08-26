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

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_ASSEMBLY)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

void _hideSelectedPart()
{
    ProError status;
    status = ProMacroLoad(L"~ Command `ProCmdViewExclude`");
    status = ProMacroExecute();
}

void _hideUnselectedPart()
{
    ProError status;
    status = ProMacroLoad(L"~ Command `ProCmdViewNormalMaster`");
    status = ProMacroExecute();
}

void HidePart(ProBoolean SelectedPart)
{
    ProError status;
    ProSelection *sel_array;
    int n_size;
    ProBoolean isSelBuf = PRO_B_TRUE;
    if (CurrentMdlType() == PRO_ASSEMBLY)
    {
        status = ProSelbufferSelectionsGet(&sel_array);
        status = ProArraySizeGet((ProArray *)sel_array, &n_size);
        if (status != PRO_TK_NO_ERROR || n_size < 1)
        {
            status = ProSelectionarrayFree(sel_array);
            status = ProSelect((char *)"prt_or_asm", -1, NULL, NULL, NULL, NULL, &sel_array, &n_size);
            if (status != PRO_TK_NO_ERROR)
            {
                return;
            }
            isSelBuf = PRO_B_FALSE;
        }

        SelectedPart == PRO_B_TRUE ? _hideSelectedPart() : _hideUnselectedPart();
        if (isSelBuf == PRO_B_TRUE)
        {
            status = ProSelectionarrayFree(sel_array);
        }
    }
}

void HideSelectedPart()
{
    HidePart(PRO_B_TRUE);
}

void HideUnselectedPart()
{
    HidePart(PRO_B_FALSE);
}

void Restore()
{
    ProError status;
    ProSimprep simprep;
    ProMdl mdl;
    ProSolid asmSolid;

    status = ProMdlCurrentGet(&mdl);
    asmSolid = ProMdlToSolid(mdl);
    status = ProSimprepActiveGet(asmSolid, &simprep);
    status = ProSimprepDelete(&simprep);
    status = ProMacroLoad(L"~ Command `ProCmdViewVisTool` ;~ Activate `visual_dlg0` `CloseBtn`;");
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_HideSelectedPartID;
    uiCmdCmdId IMI_HideUnselectedPartID;
    uiCmdCmdId IMI_ShowAllPartID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_HideSelectedPart_Act", (uiCmdCmdActFn)HideSelectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideSelectedPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideSelectedPartID, MSGFILE);

    status = ProCmdActionAdd("IMI_HideUnselectedPart_Act", (uiCmdCmdActFn)HideUnselectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideUnselectedPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideUnselectedPartID, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowAllPart_Act", (uiCmdCmdActFn)Restore, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowAllPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenutips", NULL, PRO_B_TRUE, IMI_ShowAllPartID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
