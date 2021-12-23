#include "./includes/Test.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void ShowDialog()
{
    ProUIMessageButton *buttons;
    ProUIMessageButton user_choice;
    ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
    buttons[0] = PRO_UI_MESSAGE_OK;
    ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", L"测试消息", buttons, PRO_UI_MESSAGE_OK, &user_choice);
    ProArrayFree((ProArray *)&buttons);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId ShowDialogID;

    status = ProMenubarMenuAdd("ShowDialog", "ShowDialog", "About", PRO_B_TRUE, MSGFILE);
    status = ProMenubarmenuMenuAdd("ShowDialog", "ShowDialog", "ShowDialog", NULL, PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("ShowDialog_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ShowDialogID);
    status = ProMenubarmenuPushbuttonAdd("ShowDialog", "ShowDialogmenu", "ShowDialogmenu", "ShowDialogmenutips", NULL, PRO_B_TRUE, ShowDialogID, MSGFILE);
    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
