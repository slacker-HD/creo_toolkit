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
void ChangeDrwtoMdlName()
{
    ProError status;
    ProDrawing drawing;
    ProMdl tmpMdl;
    ProSolid solid;
    ProName mdlName, drawingName;
    ProPath drawingPath, currentPath;
    ProMdldata drawingData;
    int compResult;
    status = ProMdlCurrentGet(&drawing);
    status = ProMdlDataGet(drawing, &drawingData);
    status = ProDrawingCurrentsolidGet(drawing, &solid);
    status = ProDirectoryCurrentGet(currentPath);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlNameGet(solid, mdlName);
        status = ProMdlNameGet(drawing, drawingName);
        status = ProWstringCompare(mdlName, drawingName, PRO_VALUE_UNUSED, &compResult);
        if (compResult != 0)
        {
            status = ProDirectoryChange(drawingPath);
            status = ProMdlRetrieve(mdlName, PRO_MDL_DRAWING, &tmpMdl);
            if (status != PRO_TK_NO_ERROR)
            {
                status = ProMdlRename(drawing, mdlName);
                status = ProMdlSave(drawing);
            }
            status = ProDirectoryChange(currentPath);
        }
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_ChangeDrwtoMdlNamemenuID;

    status = ProMenubarMenuAdd("IMI_DrwRenamemenu", "IMI_DrwRenamemenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_ChangeDrwtoMdlName_Act", (uiCmdCmdActFn)ChangeDrwtoMdlName, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &IMI_ChangeDrwtoMdlNamemenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_DrwRenamemenu", "IMI_ChangeDrwtoMdlNamemenu", "IMI_ChangeDrwtoMdlNamemenu", "IMI_ChangeDrwtoMdlNamemenutips", NULL, PRO_B_TRUE, IMI_ChangeDrwtoMdlNamemenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
