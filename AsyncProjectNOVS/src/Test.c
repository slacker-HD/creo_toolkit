#include "./includes/Test.h"

int main(int argc,char *argv[])
{
	printf("启动会话中...\n");
	ProEngineerStart("C:\\PTC\\Creo 2.0\\Parametric\\bin\\parametric.exe","");
	printf("会话已启动，按任意键关闭会话并退出本程序...\n");
	_getch();
	printf("结束会话。程序退出...\n");
	ProEngineerEnd();
	return 0;
}
 