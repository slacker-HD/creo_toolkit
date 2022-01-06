#ifndef __MAIN__
#define __MAIN__

#define UNICODE
#define _UNICODE

#define MSGFILE L"CreoTool.txt"
#define TRUEPATHSIZE PRO_PATH_SIZE + 20 + 1

#include "windows.h"
#include "shellapi.h"
#include "stdio.h"

#include <ProUICmd.h>
#include <ProUtil.h>
#include <ProMenu.h>
#include <ProToolkitDll.h>
#include <ProMenubar.h>
#include <ProMdl.h>
#include <ProWstring.h>
#include <ProUIMessage.h>
#include <ProArray.h>
#include <ProWindows.h>
#include <ProUI.h>
#include <ProNotify.h>
#include <ProSurface.h>
#include <ProFeature.h>
#include <ProFeatType.h>
#include <ProSolid.h>
#include <ProAsmcomp.h>
#include <ProGraphic.h>
#include <ProDtlsyminst.h>
#include <ProDrawing.h>
#include <ProUIList.h>

extern char *LastRibbonTab;
typedef enum _hint
{
    paintColor = 0,
    About = 1,
} HINT;
extern enum hint;
#endif
