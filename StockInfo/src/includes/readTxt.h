#ifndef __READTXT__
#define __READTXT__

#define UNICODE
#define _UNICODE

#define MAX_LINES 1000   // 最多读取1000行
#define MAX_LINE_LEN 256 // 每行最多256个字符

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ProToolkit.h>
#include <ProUtil.h>

int read_stock_file(char lines[][MAX_LINE_LEN], int maxLines, int maxLineLen, const char *filePath);
int get_full_file_path(char *filePath, size_t bufferSize);

#endif
