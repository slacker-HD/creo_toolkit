// LuaTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LuaTest.h"

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

// CLuaTestApp

BEGIN_MESSAGE_MAP(CLuaTestApp, CWinApp)
END_MESSAGE_MAP()

// CLuaTestApp 构造

CLuaTestApp::CLuaTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CLuaTestApp 对象

CLuaTestApp theApp;

// CLuaTestApp 初始化

BOOL CLuaTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

ProError ShowDialog(wchar_t *Message)
{
	ProUIMessageButton *buttons;
	ProUIMessageButton user_choice;
	ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
	buttons[0] = PRO_UI_MESSAGE_OK;
	ProUIMessageDialogDisplay(PROUIMESSAGE_INFO, L"Warning", Message, buttons, PRO_UI_MESSAGE_OK, &user_choice);
	ProArrayFree((ProArray *)&buttons);
	return PRO_TK_NO_ERROR;
}

lua_State *L;
int luaAdd(int x, int y)
{
	int sum;
	lua_getglobal(L, "add");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_call(L, 2, 1);
	sum = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return sum;
}

extern "C" int user_initialize()
{
	ProError status;
	status = ShowDialog(L"利用lua计算2+3");

	L = luaL_newstate();
	luaopen_base(L);
	luaL_openlibs(L);
	luaL_loadfile(L, "D:\\mydoc\\creo_toolkit\\LuaTest\\add.lua");
	lua_pcall(L, 0, LUA_MULTRET, 0);
	int sum = luaAdd(2, 3);
	lua_close(L);

	WCHAR buf[5];
	wsprintfW(buf, L"%d", sum);
	status = ShowDialog(buf);

	return PRO_TK_NO_ERROR;
}
extern "C" void user_terminate()
{
	ProError status;
	status = ShowDialog(L"程序已退出");
}