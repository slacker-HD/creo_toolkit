#include "./includes/CommonFuns.h"

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
