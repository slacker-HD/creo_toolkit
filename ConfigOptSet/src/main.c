#include "./includes/main.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void GetHomePageOption()
{
    ProError status;
    ProLine Value;
    status = ProConfigoptGet(L"web_browser_homepage", Value);
    status = ProMessageDisplay(MSGFILE, "IMI_OptionResult", Value);
}

void SetHomePageOption()
{
    ProError status;
    wchar_t Value[MAXMESSAGESTRINGLENGTH] = L"about:blank";
    status = ProMessageDisplay(MSGFILE, "IMI_OptionPromt", Value);
    status = ProMessageStringRead(MAXMESSAGESTRINGLENGTH, Value);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProConfigoptSet(L"web_browser_homepage", Value);
        status = ProMessageDisplay(MSGFILE, "IMI_OptionFinPrompt");
    }
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_GetConfigmenuID, IMI_SetConfigmenuID;

    status = ProMenubarMenuAdd("IMI_ConfigOptSetmenu", "IMI_ConfigOptSetmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_GetConfig_Act", (uiCmdCmdActFn)GetHomePageOption, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_GetConfigmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConfigOptSetmenu", "IMI_GetConfigMenu", "IMI_GetConfigMenu", "IMI_GetConfigMenutips", NULL, PRO_B_TRUE, IMI_GetConfigmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_SetConfig_Act", (uiCmdCmdActFn)SetHomePageOption, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_SetConfigmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConfigOptSetmenu", "IMI_SetConfigMenu", "IMI_SetConfigMenu", "IMI_SetConfigMenutips", NULL, PRO_B_TRUE, IMI_SetConfigmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
