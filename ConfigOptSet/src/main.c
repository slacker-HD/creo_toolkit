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

void GetProtks()
{
    ProError status;
    ProPath *protkValues;
    int i, size;
    FILE *file;
    wchar_t *prefix = L"当前已设置为自动加载的程序(config.pro中protkdat选项记录的内容)有:\n";
    _wfopen_s(&file, L"IMI_protkdat.txt", L"wt+,ccs=UTF-16LE");
    fwrite(prefix, sizeof(wchar_t), wcslen(prefix), file);

    status = ProConfigoptArrayGet(L"protkdat", &protkValues);
    status = ProArraySizeGet(protkValues, &size);
    for (i = 0; i < size; i++)
    {
        fwrite(protkValues[i], sizeof(wchar_t), wcslen(protkValues[i]), file);
        fwrite(L"\n",sizeof(wchar_t), wcslen(L"\n"), file);
    }
    fclose(file);
    status = ProInfoWindowDisplay(L"b.txt", NULL, NULL);
    status = ProArrayFree(&protkValues);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_GetConfigmenuID, IMI_SetConfigmenuID, IMI_GetConfigArraymenuID;

    status = ProMenubarMenuAdd("IMI_ConfigOptSetmenu", "IMI_ConfigOptSetmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_GetConfig_Act", (uiCmdCmdActFn)GetHomePageOption, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_GetConfigmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConfigOptSetmenu", "IMI_GetConfigMenu", "IMI_GetConfigMenu", "IMI_GetConfigMenutips", NULL, PRO_B_TRUE, IMI_GetConfigmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_SetConfig_Act", (uiCmdCmdActFn)SetHomePageOption, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_SetConfigmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConfigOptSetmenu", "IMI_SetConfigMenu", "IMI_SetConfigMenu", "IMI_SetConfigMenutips", NULL, PRO_B_TRUE, IMI_SetConfigmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_GetConfigArray_Act", (uiCmdCmdActFn)GetProtks, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_GetConfigArraymenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConfigOptSetmenu", "IMI_GetConfigArrayMenu", "IMI_GetConfigArrayMenu", "IMI_SetConfigMenutips", NULL, PRO_B_TRUE, IMI_GetConfigArraymenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
