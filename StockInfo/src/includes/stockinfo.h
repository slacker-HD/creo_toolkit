#ifndef __STOCKINFO__
#define __STOCKINFO__

#define UNICODE
#define _UNICODE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <wininet.h>

wchar_t *GetStockInfoStringByCode(const wchar_t *stockCode);

#endif
