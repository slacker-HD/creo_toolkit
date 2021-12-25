#include "includes/ShowDirectory.h"

void ShowDirectory()
{
    ProError status;
    ProMdl mdl;
    ProMdldata data;
    wchar_t path[TRUEPATHSIZE];
    status = ProMdlCurrentGet(&mdl);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProMdlDataGet(mdl, &data);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProWstringCopy(data.device, path, PRO_VALUE_UNUSED);
            status = ProWstringConcatenate(L":",path,PRO_VALUE_UNUSED);
            status = ProWstringConcatenate(data.path,path,PRO_VALUE_UNUSED);
            ShellExecuteW(NULL, L"open", L"explorer.exe", path, NULL, SW_SHOWNORMAL);
        }
    }
}