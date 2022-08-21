#include "./includes/OpenSamenameDrw.h"

void OpenSamenameDrw()
{
    ProError status;
    ProMdl mdl;
    ProDrawing drawing;
    ProName name;
    int new_win_id;
    status = ProMdlCurrentGet(&mdl);
    status = ProMdlNameGet(mdl, name);
    status = ProMdlLoad(name, PRO_MDL_DRAWING, PRO_B_FALSE, (ProMdl *)&drawing);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProObjectwindowCreate(name, PRO_DRAWING, &new_win_id);
        status = ProMdlDisplay(drawing);
        status = ProWindowActivate(new_win_id);
    }
    else
    {
        ShowMessageDialog(1, L"当前工作目录下未发现同名工程图");
    }
}