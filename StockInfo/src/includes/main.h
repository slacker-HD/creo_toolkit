#ifndef __MAIN__
#define __MAIN__

#define UNICODE
#define _UNICODE

#define MSGFILE L"StockInfo.txt"

#include "stdio.h"
#include <ProUICmd.h>
#include <ProUtil.h>
#include <ProMenu.h>
#include <ProToolkitDll.h>
#include <ProMenubar.h>
#include <ProWstring.h>
#include <ProMessage.h>

// 初始化演示代码列表，参数为相对或绝对路径，返回读取到的条目数（失败返回 -1）
int InitDemoCodes(const wchar_t *filepath);
void FreeDemoCodes(void);

#endif
