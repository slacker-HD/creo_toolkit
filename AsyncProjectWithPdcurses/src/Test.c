#include "./includes/Test.h"
#include "./includes/curses.h"
#include "./includes/curspriv.h"
#include "./includes/panel.h"

void initialize()
{
	initscr();
	start_color();
	raw();
	cbreak();
	noecho();
	curs_set(0);
}

void shutdown()
{
	endwin();
}

int main(int argc, char *argv[])
{

	initialize();
	mvprintw(LINES / 2, (COLS - (int)strlen("启动会话中...")) / 2, "启动会话中...");
	refresh();
	ProEngineerStart("C:\\PTC\\Creo 2.0\\Parametric\\bin\\parametric.exe", "");
	mvprintw(LINES / 2, (COLS - (int)strlen("会话已启动，按任意键关闭会话并退出本程序...")) / 2, "会话已启动，按任意键关闭会话并退出本程序...");
	refresh();
	getch();
	mvprintw(LINES / 2, (COLS - (int)strlen("结束会话。程序退出...")) / 2, "结束会话。程序退出...");
	refresh();
	ProEngineerEnd();
	shutdown();
	return 0;
}
