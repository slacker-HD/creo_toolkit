#include "./includes/main.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void ShowDialog()
{
    Fl_Double_Window w(100, 200, 460, 320, "Fltk layout");
    w.size_range(w.w(), w.h(), 0, 0);

    Fl_Group group1(10, 10, w.w() - 20, 30);
    Fl_Input input1(80, 10, w.w() - 205, 30, "Name:");
    Fl_Button *b1 = new Fl_Button(w.w() - 110, 10, 100, 30, "Check!");
    group1.end();
    group1.resizable(input1);

    Fl_Group group2(10, 50, w.w() - 20, 30);
    Fl_Input input2(80, 50, w.w() - 205, 30, "Mail:");
    Fl_Button b2(w.w() - 110, 50, 100, 30, "Check!");
    group2.end();
    group2.resizable(input2);

    Fl_Multiline_Input comments(80, 100, w.w() - 90, w.h() - 150, "Comments:");
    Fl_Group group3(10, w.h() - 10 - 30, w.w() - 20, 30);
    Fl_Box b(10, w.h() - 10 - 30, group3.w() - 100, 30); // Fl_Box是默认不可见的
    Fl_Button b3(w.w() - 10 - 100, w.h() - 10 - 30, 100, 30, "OK");
    group3.end();
    group3.resizable(b);

    w.resizable(comments);
    w.show();
    Fl::run();
}

extern "C" int user_initialize()
{
	ProError status;
	uiCmdCmdId FltkTestID;

	Fl::scheme("plastic");

	status = ProMenubarMenuAdd("FltkTest", "FltkTest", "About", PRO_B_TRUE, MSGFILE);
	status = ProMenubarmenuMenuAdd("FltkTest", "FltkTest", "FltkTest", NULL, PRO_B_TRUE, MSGFILE);

	status = ProCmdActionAdd("FltkTest_Act", (uiCmdCmdActFn)ShowDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &FltkTestID);
	status = ProMenubarmenuPushbuttonAdd("FltkTest", "FltkTestmenu", "FltkTestmenu", "FltkTestmenutips", NULL, PRO_B_TRUE, FltkTestID, MSGFILE);
	return PRO_TK_NO_ERROR;
}

extern "C" void user_terminate()
{
}
