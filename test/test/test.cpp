// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
int _tmain(int argc, _TCHAR* argv[])
{
	FILE *fp;
    char line[100]; //应该最多只有12个的，乘以2以备以后扩展
    int i = 1;
	int _col = _wtoi(argv[2]);
    int _row = _wtoi(argv[3]);
    fp = fopen("D:\\mydoc\\creo_toolkit\\test\\Debug\\ReadMe.txt", "r");
    if (fp == NULL)
    {
        printf("Can not load the print file.");
        return -1;
    }

    while (!feof(fp))
    {
        printf("input line %d:",i);
        i++;
        fgets(line, 100, fp);
        printf("%s", line);
		printf("   %d\n", strlen(line));
    }
    fclose(fp);
    return 0;
}