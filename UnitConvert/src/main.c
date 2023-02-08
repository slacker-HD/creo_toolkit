#include "./includes/main.h"

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

    status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"提示", Message, buttons, buttons[0], &user_choice);
    ProArrayFree((ProArray *)&buttons);
    return user_choice;
}

int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdltype;
    ProError status;
    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(mdl, &mdltype);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return mdltype;
}

static uiCmdAccessState AccessPRT(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_PART)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

void _convertUnit(ProUnitConvertType ConVertType)
{
    ProError status;
    ProMdl mdl;
    ProUnitsystem unitSystem;
    ProUnitsystemType type;
    wchar_t *p = NULL;
    ProUnitsystem *unitSystem_array;
    int num_unitSystem, i;
    status = ProMdlCurrentGet(&mdl);
    status = ProMdlPrincipalunitsystemGet(mdl, &unitSystem);
    p = wcsstr(unitSystem.name, L"mmNs");
    if (!p)
    {
        status = ProMdlUnitsystemsCollect(mdl, &unitSystem_array);
        if (status != PRO_TK_NO_ERROR)
            return;
        status = ProArraySizeGet(unitSystem_array, &num_unitSystem);
        for (i = 0; i < num_unitSystem; i++)
        {
            p = wcsstr(unitSystem_array[i].name, L"mmNs");
            if (p)
            {
                status = ProMdlPrincipalunitsystemSet(mdl, &unitSystem_array[i], ConVertType, PRO_B_TRUE, PRO_VALUE_UNUSED);
                status = ProArrayFree((ProArray *)&unitSystem_array);
                return;
            }
        }
        ShowMessageDialog(1, L"模型中未包含mmNs单位。\n请检查模型设置。");
        status = ProArrayFree((ProArray *)&unitSystem_array);
    }
}

void InterPretUnit()
{
    _convertUnit(PRO_UNITCONVERT_SAME_DIMS);
}

void ConvertUnit()
{
    _convertUnit(PRO_UNITCONVERT_SAME_SIZE);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_UnitInterPretmenuID;
    uiCmdCmdId IMI_UnitConvertmenuID;

    status = ProMenubarMenuAdd("IMI_ConvertUnitmenu", "IMI_ConvertUnitmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitInterPret_Act", (uiCmdCmdActFn)InterPretUnit, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_UnitInterPretmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConvertUnitmenu", "IMI_UnitInterPretmenu", "IMI_UnitInterPretmenu", "IMI_UnitInterPretmenutips", NULL, PRO_B_TRUE, IMI_UnitInterPretmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitConvert_Act", (uiCmdCmdActFn)ConvertUnit, uiProeImmediate, AccessPRT, PRO_B_TRUE, PRO_B_TRUE, &IMI_UnitConvertmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_ConvertUnitmenu", "IMI_UnitConvertmenu", "IMI_UnitConvertmenu", "IMI_UnitConvertmenutips", NULL, PRO_B_TRUE, IMI_UnitConvertmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
