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

int main(int argc, char *argv[])
{

	initialize();
	mvprintw(LINES / 2, (COLS - (int)strlen("启动CREO会话\xE4\xB8\xad...")) / 2, "启动CREO会话\xE4\xB8\xad...");
	refresh();
	ProEngineerStart("C:\\PTC\\Creo 2.0\\Parametric\\bin\\parametric.exe", "");
	mvprintw(LINES / 2 + 1, (COLS - (int)strlen("会话已启动，按任意键关闭会话")) / 2, "会话已启动，按任意键关闭会话");
	refresh();
	getch();
	ProEngineerEnd();
	mvprintw(LINES / 2 + 3, (COLS - (int)strlen("会话结束。按任意键退出 ")) / 2, "会话结束。按任意键退出 ");
	refresh();
	getch();
	endwin();
	return 0;
}

