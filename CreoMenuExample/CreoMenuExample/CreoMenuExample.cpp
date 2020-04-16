// CreoMenuExample.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CreoMenuExample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCreoMenuExampleApp

BEGIN_MESSAGE_MAP(CCreoMenuExampleApp, CWinApp)
END_MESSAGE_MAP()

// CCreoMenuExampleApp 构造

CCreoMenuExampleApp::CCreoMenuExampleApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CCreoMenuExampleApp 对象

CCreoMenuExampleApp theApp;

// CCreoMenuExampleApp 初始化

BOOL CCreoMenuExampleApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"提示", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

void MainMenuAct()
{
	ShowDialog(L"点击了主菜单项。工程图或者模型中选中特征右键也会添加右键菜单项，功能与点击主菜单一致。本例功能仅只弹出此对话框。");
}


//static ProMenuItemName check_group_items[] = {"CheckButtonMenu"};
//static ProMenuItemLabel check_group_labels[] = {"CheckButtonMenuItem"};
//static ProMenuLineHelp check_group_help[] = {"CheckButtonMenuItemtips"};
//static ProCmdItemIcon check_group_icons[]={"Icon.png"};

typedef struct procheckbuttonstruct
{
	uiCmdCmdId command;
	ProBoolean state;
} ProCheckButton;

static ProCheckButton _checkbutton[1];

int CheckButtonActfn(uiCmdCmdId command, uiCmdValue *p_value, void *p_push_command_data)
{
	//应该做循环对应多个，这里测试只有一个菜单项所以简化了
	if (command == _checkbutton[0].command)
	{
		if (_checkbutton[0].state == PRO_B_FALSE)
		{
			_checkbutton[0].state = PRO_B_TRUE;
			ShowDialog(L"CheckButton已按下。本例功能仅只弹出此对话框。");
		}
		else
		{
			_checkbutton[0].state = PRO_B_FALSE;
			ShowDialog(L"CheckButton已松开。本例功能仅只弹出此对话框。");
		}
	}
	return 0;
}

int CheckButtonValFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProBoolean value;
	//应该做循环检测对应多个，这里测试只有一个菜单项所以简化了
	if (_checkbutton[0].command == command)
	{
		status = ProMenubarmenuChkbuttonValueGet(p_value, &value);
		if (value == _checkbutton[0].state)
		{
			return 0;
		}
		status = ProMenubarmenuChkbuttonValueSet(p_value, _checkbutton[0].state);
		return 0;
	}
	return 0;
}

static ProMenuItemName radio_group_items[] = {"RadioButtonMenu1", "RadioButtonMenu2","RadioButtonMenu3", "RadioButtonMenu4"};
static ProMenuItemLabel radio_group_labels[] = {"RadioButtonMenuItem1", "RadioButtonMenuItem2","RadioButtonMenuItem3","RadioButtonMenuItem4"};
static ProMenuLineHelp radio_group_help[] = {"RadioButtonMenuItem1tips", "RadioButtonMenuItem2tips","RadioButtonMenuItem3tips","RadioButtonMenuItem4tips"};
static ProCmdItemIcon radio_group_icons[]={"Icon.png", "Icon.png","Icon.png", "Icon.png"};

int RadioButtonValFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProMenuItemName name;

	status = ProMenubarMenuRadiogrpValueGet (p_value, name);
	status = ProMenubarMenuRadiogrpValueSet(p_value, name);
	return 0;
}

int RadioButtonActFn(uiCmdCmdId command, uiCmdValue *p_value)
{
	ProError status;
	ProMenuItemName  name;
	status = ProMenubarMenuRadiogrpValueGet(p_value, name);
	if(strcmp(name,"RadioButtonMenu1") ==0)
	{
		ShowDialog(L"选择了RadioButton项1。本例功能仅只弹出此对话框。");
	}
	else if(strcmp(name,"RadioButtonMenu2") ==0)
	{
		ShowDialog(L"选择了RadioButton项2。本例功能仅只弹出此对话框。");
	}
	else if(strcmp(name,"RadioButtonMenu3") ==0)
	{
		ShowDialog(L"选择了RadioButton项3。本例功能仅只弹出此对话框。");
	}
	else if(strcmp(name,"RadioButtonMenu4") ==0)
	{
		ShowDialog(L"选择了RadioButton项4。本例功能仅只弹出此对话框。");
	}
	else
	{
		ShowDialog(L"出现了一个奇怪项，不应该存在的。");
	}
	return 0;
}

static uiCmdAccessState AccessPopupmenu(uiCmdCmdId command,ProAppData appdata, ProSelection* sel_buffer)
{
	//应该根据选择的对象确定右键菜单是否出现，这里默认都出现
	return ACCESS_AVAILABLE;
}

ProError ProPopupMenuNotification(ProMenuName name)
{
	ProError status;
	uiCmdCmdId MainMenuID;
	ProPopupMenuId PopupMenuID;
	ProLine label;
	ProLine help;

	status = ProPopupmenuIdGet(name, &PopupMenuID);
	status = ProCmdCmdIdFind("MainMenu_Act", &MainMenuID);
	status = ProMessageToBuffer(label, MSGFILE,"MainMenuItem");
	status = ProMessageToBuffer(help, MSGFILE,"MainMenuItemtips");
	status = ProPopupmenuButtonAdd(PopupMenuID, PRO_VALUE_UNUSED, "HFUTGDM.MainMenu_Act",label, help,MainMenuID,AccessPopupmenu,NULL);

	return PRO_TK_NO_ERROR;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId MainMenuID;
	uiCmdCmdId RadioMenuID;

	status = ProMenubarMenuAdd("CreoMenuExample", "CreoMenuExample", "Help", PRO_B_TRUE, MSGFILE);

	//普通菜单+ribbon按钮
	status = ProMenubarmenuMenuAdd("CreoMenuExample", "MainMenu", "MainMenu", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdActionAdd("MainMenu_Act", (uiCmdCmdActFn)MainMenuAct, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &MainMenuID);
	status = ProMenubarmenuPushbuttonAdd("MainMenu", "MainMenuItem", "MainMenuItem", "MainMenuItemtips", NULL, PRO_B_TRUE, MainMenuID, MSGFILE);
	status = ProCmdIconSet(MainMenuID, "Icon.png");
	status = ProCmdDesignate(MainMenuID, "MainMenuItem", "MainMenuItem", "MainMenuItemtips", MSGFILE);

	//带radio的菜单+ribbon按钮
	status = ProMenubarmenuMenuAdd("CreoMenuExample", "RadioButtonMenu", "RadioButtonMenu", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdOptionAdd("RadioButtonMenu_Act", (uiCmdCmdActFn)RadioButtonActFn, PRO_B_FALSE, (uiCmdCmdValFn)RadioButtonValFn, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &RadioMenuID);
	status = ProMenubarmenuRadiogrpAdd("RadioButtonMenu", "RadioButtonGroup",2, radio_group_items, radio_group_labels, radio_group_help,NULL, PRO_B_FALSE,RadioMenuID,MSGFILE);
	status = ProCmdRadiogrpDesignate(RadioMenuID, 4, radio_group_items, radio_group_labels, radio_group_help,radio_group_icons,"RadioButtonGroupDescription",MSGFILE);

	//带checkbox的菜单+ribbon按钮；添加图标方法未知
	status = ProMenubarmenuMenuAdd("CreoMenuExample", "CheckButtonMenu", "CheckButtonMenu", NULL, PRO_B_TRUE, MSGFILE);
	status = ProCmdOptionAdd("CheckButtonMenu_Act", (uiCmdCmdActFn)CheckButtonActfn, PRO_B_TRUE, (uiCmdCmdValFn)CheckButtonValFn, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &(_checkbutton[0].command));
	status = ProMenubarmenuChkbuttonAdd("CheckButtonMenu", "CheckButtonMenuItem", "CheckButtonMenuItem", "CheckButtonMenuItemtips", NULL, PRO_B_TRUE, _checkbutton[0].command, MSGFILE);
	status = ProCmdDesignate(_checkbutton[0].command, "CheckButtonMenuItem", "CheckButtonMenuItem", "CheckButtonMenuItemtips", MSGFILE);
	//status = ProCmdRadiogrpDesignate(_checkbutton[0].command,1,check_group_items,check_group_labels,check_group_help,check_group_icons,"CheckButtonGroupDescription",MSGFILE);
	//status = ProCmdIconSet(_checkbutton[0].command, "Icon.png");

	//注册右键菜单监听事件，功能与普通菜单一样
	status = ProNotificationSet(PRO_POPUPMENU_CREATE_POST,  (ProFunction)ProPopupMenuNotification);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
	ProError status;
	status = ProNotificationUnset(PRO_POPUPMENU_CREATE_POST);
}