#include "./includes/Test.h"
#include "./includes/curses.h"
#include "./includes/curspriv.h"
#include "./includes/panel.h"

void initcolor()
{
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_YELLOW);
	init_pair(3, COLOR_RED, COLOR_GREEN);
}

void initialize()
{
	initscr();
	start_color();
	initcolor();
	raw();
	cbreak();
	noecho();
	curs_set(0);
}

int main(int argc, char *argv[])
{
	initialize();
	attron(COLOR_PAIR(1));
	mvprintw(LINES / 2, (COLS - (int)strlen("启动CREO会话\xE4\xB8\xad...")) / 2, "启动CREO会话\xE4\xB8\xad...");
	refresh();
	ProEngineerStart("C:\\PTC\\Creo 2.0\\Parametric\\bin\\parametric.exe", "");
	attron(COLOR_PAIR(2));
	mvprintw(LINES / 2 + 1, (COLS - (int)strlen("会话已启动，按任意键关闭会话")) / 2, "会话已启动，按任意键关闭会话");
	refresh();
	getch();
	ProEngineerEnd();
	attron(COLOR_PAIR(3));
	mvprintw(LINES / 2 + 3, (COLS - (int)strlen("会话结束。按任意键退\xE5\x87\xBA...")) / 2, "会话结束。按任意键退\xE5\x87\xBA...");
	refresh();
	getch();
	endwin();
	return 0;
}
