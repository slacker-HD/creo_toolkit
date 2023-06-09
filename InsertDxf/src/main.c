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

void SetInsertDxf()
{
    ProError status;
    ProMdl mdl;
    ProPath dxfFile;
    status = ProMdlCurrentGet(&mdl);
    status = ProFileOpen(NULL, L"*.dxf", NULL, NULL, NULL, NULL, dxfFile);
    if (status == PRO_TK_NO_ERROR)
    {
        status = Pro2dImportAppend(PRO_DXF_FILE, L"D:\\Test.dxf", mdl, PRO_B_TRUE, PRO_B_TRUE);
        status = ProMacroLoad(L"~ Command `ProCmdDwgRegenModel`;");
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_InsertDxfmenuID;

    status = ProMenubarMenuAdd("IMI_DxfInsertmenu", "IMI_DxfInsertmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_InsertDxf_Act", (uiCmdCmdActFn)SetInsertDxf, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_InsertDxfmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DxfInsertmenu", "IMI_InsertDxfmenu", "IMI_InsertDxfmenu", "IMI_InsertDxfmenutips", NULL, PRO_B_TRUE, IMI_InsertDxfmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
