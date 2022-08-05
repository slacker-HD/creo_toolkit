#include "./includes/AutoDirSetting.h"

int AutoDirSettingFn(uiCmdCmdId command, uiCmdValue *p_value, void *p_push_command_data)
{
	ProError status;
	ProPath exePath, cfgPath;
	status = ProToolkitApplTextPathGet(exePath);
	status = ProWstringCopy(exePath, cfgPath, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L"\\CreoTool.ini", cfgPath, PRO_VALUE_UNUSED);
	if (command == check_but[0].command)
	{
		if (check_but[0].state == PRO_B_FALSE)
		{
			check_but[0].state = PRO_B_TRUE;
			WriteOrUpdateConfig(cfgPath, L"AutoChangePath", L"True");
		}
		else
		{
			check_but[0].state = PRO_B_FALSE;
			WriteOrUpdateConfig(cfgPath, L"AutoChangePath", L"False");
		}
	}
	return 0;
}
int AutoDirSettingValueFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProBoolean value;
	if (check_but[0].command == command)
	{
		status = ProMenubarmenuChkbuttonValueGet(p_value, &value);
		if (value == check_but[0].state)
			return 0;
		status = ProMenubarmenuChkbuttonValueSet(p_value, check_but[0].state);
		return 0;
	}
	return 0;
}