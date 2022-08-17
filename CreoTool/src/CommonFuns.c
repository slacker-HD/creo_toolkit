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

    status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"ÌáÊ¾", Message, buttons, buttons[0], &user_choice);
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
