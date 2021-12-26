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