#include "stdafx.h"
#include "QRCodeGen.h"
#include "CommonToolkitFun.h"
#include "ToolkitSymFun.h"

#pragma region MFC
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CQRCodeGenApp, CWinApp)
END_MESSAGE_MAP()

CQRCodeGenApp::CQRCodeGenApp()
{
}
CQRCodeGenApp theApp;
BOOL CQRCodeGenApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
#pragma endregion

static uiCmdAccessState AccessDRW(uiCmdAccessMode access_mode)
{
	if (CurrentMdlType() == PRO_DRAWING)
		return ACCESS_AVAILABLE;
	else
		return ACCESS_INVISIBLE;
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId QRCodeGenID;

	status = ProMenubarMenuAdd("QRCodeGen", "QRCodeGen", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("QRCodeGen", "QRCodeGen", "QRCodeGen", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("QRCodeGen1_Act", (uiCmdCmdActFn)PlaceQrCode, uiProeImmediate, AccessDRW, PRO_B_TRUE, PRO_B_TRUE, &QRCodeGenID);
	status = ProMenubarmenuPushbuttonAdd("QRCodeGen", "QRCodeGenmenu", "QRCodeGenmenu", "QRCodeGenmenutips", NULL, PRO_B_TRUE, QRCodeGenID, MSGFILE);

	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}