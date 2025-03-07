#include "./includes/AutoWorkDirSetting.h"

int AutoWorkDirSettingFn(uiCmdCmdId command, uiCmdValue *p_value, void *p_push_command_data)
{
	ProError status;
	ProPath exePath, cfgPath;
	status = ProToolkitApplTextPathGet(exePath);
	status = ProWstringCopy(exePath, cfgPath, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L"\\CreoTool.ini", cfgPath, PRO_VALUE_UNUSED);
	
	if (command == check_but[1].command)
	{
		if (check_but[1].state == PRO_B_FALSE)
		{
			check_but[1].state = PRO_B_TRUE;
			status = ProNotificationSet(PRO_WINDOW_CHANGE_POST, ProUserWindowChangePost); //切换窗口导致的当前模型变化
			status = ProNotificationSet(PRO_MDL_SAVE_POST, ProUserMdlSavePost);			  //另存为导致的路径变化
			status = ProNotificationSet(PRO_MDL_RETRIEVE_POST, ProUserMdlRetrievePost);	  //打开新模型导致的路径变化
			WriteOrUpdateConfig(cfgPath, L"AutoChangeWorkDir", L"True");
		}
		else
		{
			check_but[1].state = PRO_B_FALSE;
			status = ProNotificationUnset(PRO_WINDOW_CHANGE_POST);
			status = ProNotificationUnset(PRO_MDL_SAVE_POST);
			status = ProNotificationUnset(PRO_MDL_RETRIEVE_POST);
			WriteOrUpdateConfig(cfgPath, L"AutoChangeWorkDir", L"False");
		}
	}
	return 0;
}

int AutoWorkDirSettingValueFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProBoolean value;
	if (check_but[1].command == command)
	{
		status = ProMenubarmenuChkbuttonValueGet(p_value, &value);
		if (value == check_but[1].state)
			return 0;
		status = ProMenubarmenuChkbuttonValueSet(p_value, check_but[1].state);
		return 0;
	}
	return 0;
}