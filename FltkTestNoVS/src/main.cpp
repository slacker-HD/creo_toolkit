#include "./includes/main.h"
#include "./includes/FLTKTest.h"


FLTKTest _fltk;
static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void ShowDialog()
{
    _fltk.ShowDialog();
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FltkTestID;

	status = ProMenubarMenuAdd("FltkTest", "FltkTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FltkTest", "FltkTest", "FltkTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FltkTest_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &FltkTestID);
	status = ProMenubarmenuPushbuttonAdd("FltkTest", "FltkTestmenu", "FltkTestmenu", "FltkTestmenutips", NULL, PRO_B_TRUE, FltkTestID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
