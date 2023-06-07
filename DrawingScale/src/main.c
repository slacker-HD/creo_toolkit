#include "./includes/main.h"

int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdltype;
    ProError status;
    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(mdl, &mdltype);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return mdltype;
}

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_DRAWING)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

void SetDrawingScale()
{
    ProError status;
    ProMdl drawing;
    double scale = 0.5;
    status = ProMdlCurrentGet(&drawing);
    status = ProMessageDisplay(MSGFILE, "IMI_PrompSetScaling", &scale);
    status = ProMessageDoubleRead(NULL, &scale);
    if (status != PRO_TK_NO_ERROR && status != PRO_TK_GENERAL_ERROR)
    {
        return;
    }
    status = ProDrawingScaleSet(drawing, NULL, -1, scale);
    status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel`;");
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_DrawingScalingmenuID;

    status = ProMenubarMenuAdd("IMI_DrawingScalemenu", "IMI_DrawingScalemenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_DrawingScaling_Act", (uiCmdCmdActFn)SetDrawingScale, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_DrawingScalingmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DrawingScalemenu", "IMI_DrawingScalingmenu", "IMI_DrawingScalingmenu", "IMI_DrawingScalingmenutips", NULL, PRO_B_TRUE, IMI_DrawingScalingmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
