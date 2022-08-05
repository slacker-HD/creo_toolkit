#ifndef __CFG_H__
#define __CFG_H__

#include "main.h"
#include "CommonFuns.h"

int WriteOrUpdateConfig(wchar_t *FileName /*in*/, wchar_t *Key /*in*/, wchar_t *Value /*in*/);
int ReadConfig(wchar_t *FileName /*in*/, wchar_t *Key/*in*/, wchar_t *Value /*out*/, int *ValueLen /*out*/);

#endif