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

ProUIMessageButton ShowMessageDialog(int style, wchar_t *Message)
{
    ProUIMessageButton *buttons;
    ProUIMessageButton user_choice;
    ProError status;
    if (style == 1)
    {
        status = ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
        buttons[0] = PRO_UI_MESSAGE_YES;
    }
    else
    {
        ProArrayAlloc(2, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
        buttons[0] = PRO_UI_MESSAGE_YES;
        buttons[1] = PRO_UI_MESSAGE_NO;
    }

    status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"ÌבÊ¾", Message, buttons, buttons[0], &user_choice);
    ProArrayFree((ProArray *)&buttons);
    return user_choice;
}

ProError ProRibbonTabSwitchNotification(char *from_tab, char *to_tab)
{
    ProError status;
    if (LastRibbonTab != NULL)
    {
        status = ProStringFree(LastRibbonTab);
    }
    status = ProStringCopy(to_tab, &LastRibbonTab);
    return PRO_TK_NO_ERROR;
}

ProError ProDirectoryChangeNotification(ProPath new_path)
{
    ProError status;
    int i, pathLength, cmpResult;

    status = ProArraySizeGet(CurrentWorkDirectoryList, &pathLength);

    for (i = 0; i < pathLength; i++)
    {
        status = ProWstringCompare(new_path, CurrentWorkDirectoryList[i], PRO_VALUE_UNUSED, &cmpResult);
        if (cmpResult == 0)
            return PRO_TK_NO_ERROR;
    }

    status = ProArrayObjectAdd((ProArray)&CurrentWorkDirectoryList, PRO_VALUE_UNUSED, 1, new_path);
    return PRO_TK_NO_ERROR;
}

ProError ProMdlPathGet(ProMdldata mdldata, ProPath mdlpath)
{
    ProError status;
    memset(mdlpath, '\0', sizeof(ProPath));
    status = ProWstringCopy(mdldata.device, mdlpath, PRO_VALUE_UNUSED);
    if (status != PRO_TK_NO_ERROR)
        return status;
    status = ProWstringConcatenate(L":", mdlpath, PRO_VALUE_UNUSED);
    if (status != PRO_TK_NO_ERROR)
        return status;
    status = ProWstringConcatenate(mdldata.path, mdlpath, PRO_VALUE_UNUSED);
    return status;
}

uiCmdAccessState ComponentInASMTreeAccess(uiCmdAccessMode mode)
{
    uiCmdAccessState access_result;
    ProError status;
    ProSelection *sels;
    int size;

    access_result = ACCESS_REMOVE;

    status = ProSelbufferSelectionsGet(&sels);
    if (status != PRO_TK_NO_ERROR)
        return access_result;

    status = ProArraySizeGet(sels, &size);
    if (status != PRO_TK_NO_ERROR)
        return access_result;

    if (size == 1)
    {
        ProAsmcomp asmcomp;
        status = ProSelectionModelitemGet(sels[0], &asmcomp);
        if (asmcomp.type == PRO_FEATURE)
        {
            ProFeattype ftype;
            status = ProFeatureTypeGet(&asmcomp, &ftype);
            if (ftype == PRO_FEAT_COMPONENT)
            {
                access_result = ACCESS_AVAILABLE;
            }
        }

        if (asmcomp.type == PRO_PART || asmcomp.type == PRO_ASSEMBLY)
        {
            ProAsmcomppath path;
            status = ProSelectionAsmcomppathGet(sels[0], &path);
            if (path.table_num > 0)
            {
                access_result = ACCESS_AVAILABLE;
            }
        }
    }
    ProSelectionarrayFree(sels);
    return access_result;
}


ProError ProUserWindowChangePost()
{
	ChangeDir();
	return PRO_TK_NO_ERROR;
}

ProError ProUserMdlSavePost(ProMdldata *p_mdldata)
{
	ChangeDir();
	return PRO_TK_NO_ERROR;
}

ProError ProUserMdlRetrievePost(ProModel *p_mdldata)
{
	ChangeDir();
	return PRO_TK_NO_ERROR;
}

