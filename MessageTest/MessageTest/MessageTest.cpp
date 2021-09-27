// MessageTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MessageTest.h"

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

// CMessageTestApp

BEGIN_MESSAGE_MAP(CMessageTestApp, CWinApp)
END_MESSAGE_MAP()

// CMessageTestApp 构造

CMessageTestApp::CMessageTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMessageTestApp 对象

CMessageTestApp theApp;

// CMessageTestApp 初始化

BOOL CMessageTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
	return ACCESS_AVAILABLE;
}

void ShowMessageText()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ProError status;
	AfxMessageBox(_T("显示一般类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_TestMessage");

	AfxMessageBox(_T("显示提示类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_Prompt");

	AfxMessageBox(_T("显示信息类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_Info");

	AfxMessageBox(_T("显示警告类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_Warning");

	AfxMessageBox(_T("显示错误类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_Error");

	AfxMessageBox(_T("显示严重错误类型的消息"));
	status = ProMessageDisplay(MSGFILE, "IMI_Critical");
}

void InputMessage()
{
	ProError status;
	wchar_t wcharString[MAXMESSAGESTRINGLENGTH] = L"测试用宽字符串";
	int intRange[2], intValue = 14;
	intRange[0] = 1;
	intRange[1] = 100;
	double doubleValue = 3.1415926;

	status = ProMessageDisplay(MSGFILE, "IMI_WCharStringPrompt|||%0w", wcharString);
	status = ProMessageStringRead(MAXMESSAGESTRINGLENGTH, wcharString);
	if (status != PRO_TK_NO_ERROR)
	{
		if (status != PRO_TK_MSG_USER_QUIT)
		{
			status = ProWstringCopy(L"测试用宽字符串", wcharString, PRO_VALUE_UNUSED);
		}
		else
			return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_IntegerPrompt|||%0d", &intValue);
	status = ProMessageIntegerRead(intRange, &intValue);
	if (status != PRO_TK_NO_ERROR && status != PRO_TK_GENERAL_ERROR)
	{
		return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_DoublePrompt|||%0f", &doubleValue);
	status = ProMessageDoubleRead(NULL, &doubleValue);
	if (status != PRO_TK_NO_ERROR && status != PRO_TK_GENERAL_ERROR)
	{
		return;
	}

	status = ProMessageDisplay(MSGFILE, "IMI_ResultPrompt", wcharString, &intValue, &doubleValue);
}

void ClearMessageText()
{
	ProMessageClear();
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId ShowMessageTextID, ClearMessageID, InputMessageID;

	status = ProMenubarMenuAdd("IMI_MessageTest", "IMI_MessageTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("IMI_MessageTest", "IMI_MessageTest", "IMI_MessageTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("ShowMessage_Act", (uiCmdCmdActFn)ShowMessageText, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ShowMessageTextID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_ShowMessage", "IMI_ShowMessage", "IMI_ShowMessageTips", NULL, PRO_B_TRUE, ShowMessageTextID, MSGFILE);

	status = ProCmdActionAdd("InputMessage_Act", (uiCmdCmdActFn)InputMessage, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &InputMessageID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_InputMessage", "IMI_InputMessage", "IMI_InputMessageTips", NULL, PRO_B_TRUE, InputMessageID, MSGFILE);

	status = ProCmdActionAdd("ClearMessage_Act", (uiCmdCmdActFn)ClearMessageText, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &ClearMessageID);
	status = ProMenubarmenuPushbuttonAdd("IMI_MessageTest", "IMI_ClearMessage", "IMI_ClearMessage", "IMI_ClearMessageTips", NULL, PRO_B_TRUE, ClearMessageID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
