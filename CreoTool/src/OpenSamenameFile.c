#include "./includes/OpenSamenameFile.h"

void OpenSamenameFile()
{
    ProError status;
    ProMdl mdl;
    ProDrawing drawing;
    ProName name;
    ProPath selectedFile;
    ProMdlType mdlType;
    int newModelWinID, newDrwWinID;
    status = ProFileOpen(NULL, L"*.prt,*.asm", NULL, NULL, NULL, NULL, selectedFile);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlLoad(selectedFile, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
        status = ProMdlNameGet(mdl, name);
        status = ProMdlTypeGet(mdl, &mdlType);
        status = ProObjectwindowCreate(name, mdlType, &newModelWinID);
        status = ProMdlDisplay(mdl);

        status = ProMdlLoad(name, PRO_MDL_DRAWING, PRO_B_FALSE, (ProMdl *)&drawing);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProObjectwindowCreate(name, PRO_DRAWING, &newDrwWinID);
            status = ProMdlDisplay(drawing);
            status = ProWindowActivate(newDrwWinID);
        }
        status = ProWindowActivate(newModelWinID);
    }
}