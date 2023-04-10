#include "./includes/FLTKTest.h"

FLTKTest ::FLTKTest()
{
}

void b3_cb(Fl_Widget *w, void *data)
{
    ((Fl_Button *)w)->label((char *)data);
    fl_alert("测试事件");
}

void FLTKTest::ShowDialog()
{
    Fl::scheme("plastic");
    Fl_Double_Window w(100, 200, 460, 320, "Fltk布局");
    w.size_range(w.w(), w.h(), 0, 0);

    Fl_Group group1(10, 10, w.w() - 20, 30);
    Fl_Input input1(80, 10, w.w() - 205, 30, "名字:");
    Fl_Button *b1 = new Fl_Button(w.w() - 110, 10, 100, 30, "确定");
    group1.end();
    group1.resizable(input1);

    Fl_Group group2(10, 50, w.w() - 20, 30);
    Fl_Input input2(80, 50, w.w() - 205, 30, "邮件:");
    Fl_Button b2(w.w() - 110, 50, 100, 30, "确定");
    group2.end();
    group2.resizable(input2);

    Fl_Multiline_Input comments(80, 100, w.w() - 90, w.h() - 150, "详细说明:");
    Fl_Group group3(10, w.h() - 10 - 30, w.w() - 20, 30);
    Fl_Box b(10, w.h() - 10 - 30, group3.w() - 100, 30); // Fl_Box是默认不可见的
    Fl_Button b3(w.w() - 10 - 100, w.h() - 10 - 30, 100, 30, "测试事件");
    group3.end();
    group3.resizable(b);

    b3.callback((Fl_Callback *)b3_cb, "测试事件");

    w.resizable(comments);
    w.show();
    Fl::run();
}