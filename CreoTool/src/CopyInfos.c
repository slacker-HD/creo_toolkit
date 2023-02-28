#include "./includes/CopyInfos.h"
#include "CopyInfos.h"

#define FULLNAMELENGTH PRO_PATH_SIZE + PRO_NAME_SIZE + PRO_TYPE_SIZE + 2

void _wCopyToClipBoard(wchar_t *Content)
{
    HGLOBAL hGlobal;
    wchar_t *pGlobal = NULL;
    hGlobal = GlobalAlloc(GHND, (lstrlenW(Content) + 1) * sizeof(wchar_t));
    pGlobal = (wchar_t *)GlobalLock(hGlobal);
    lstrcpyW(pGlobal, Content);
    GlobalUnlock(hGlobal);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hGlobal);
    CloseClipboard();
}

void CopyCurrentFileName()
{
    ProError status;
    ProMdl mdl, genericMdl;
    wchar_t mdlName[PRO_PATH_SIZE];
    ProMdldata mdlData;
    ProFamtable famtab;

    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return;

    status = ProFamtableInit(mdl, &famtab);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProFaminstanceGenericGet(mdl, PRO_B_TRUE, &genericMdl);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProMdlDataGet(genericMdl, &mdlData);
        }
        else
        {
            status = ProMdlDataGet(mdl, &mdlData);
        }
    }
    else
    {
        status = ProMdlDataGet(mdl, &mdlData);
    }
    if (status != PRO_TK_NO_ERROR)
        return;

    wsprintfW(mdlName, L"%s.%s.%d", mdlData.name, mdlData.type, mdlData.version);

    _wCopyToClipBoard(mdlName);
}

void CopyCurrentFilePath()
{
    ProError status;
    ProMdl mdl;
    ProPath path;
    ProMdldata mdlData;

    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return;

    status = ProMdlDataGet(mdl, &mdlData);
    if (status != PRO_TK_NO_ERROR)
        return;

    status = ProMdlPathGet(mdlData, path);
    if (status != PRO_TK_NO_ERROR)
        return;

    _wCopyToClipBoard(path);
}

void CopyCurrentFileFullName()
{
    ProError status;
    ProMdl mdl, genericMdl;
    wchar_t fullName[FULLNAMELENGTH];
    ProMdldata mdlData;
    ProFamtable famtab;
    ProPath path;

    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return;

    status = ProFamtableInit(mdl, &famtab);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProFaminstanceGenericGet(mdl, PRO_B_TRUE, &genericMdl);
        if (status == PRO_TK_NO_ERROR)
        {
            status = ProMdlDataGet(genericMdl, &mdlData);
        }
        else
        {
            status = ProMdlDataGet(mdl, &mdlData);
        }
    }
    else
    {
        status = ProMdlDataGet(mdl, &mdlData);
    }
    if (status != PRO_TK_NO_ERROR)
        return;

    status = ProMdlPathGet(mdlData, path);

    wsprintfW(fullName, L"%s%s.%s.%d", path, mdlData.name, mdlData.type, mdlData.version);

    _wCopyToClipBoard(fullName);
}
