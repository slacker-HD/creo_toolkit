#ifndef __MAIN__
#define __MAIN__

#define UNICODE
#define _UNICODE

#define MSGFILE L"CreoTool.txt"
#define TRUEPATHSIZE PRO_PATH_SIZE + 20 + 1

#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include <ProPDF.h>
#include <ProFaminstance.h>
#include <ProFamtable.h>
#include <ProSimprep.h>
#include <ProSelbuffer.h>
#include <ProSimprepdata.h>
#include <ProLayer.h>
#include <ProDwgtable.h>
#include <ProDtlnote.h>
#include <ProAnnotation.h>
#include <ProSurfFinish.h>
#include <ProUITextarea.h>
#include <ProRelSet.h>
#include <ProUITable.h>
#include <ProUIOptionmenu.h>


extern char *LastRibbonTab;
typedef enum _hint
{
    paintColor = 0,
    About = 1,
} HINT;
extern enum hint;

typedef struct _user_check_but
{
    uiCmdCmdId command;
    ProBoolean state;
} UserCheckBut;
extern UserCheckBut check_but[2];

extern ProRelfuncArg* Args_StrToDouble;
extern ProRelfuncArg* Args_DoubleToStr;
extern ProRelfuncArg* Args_IntToStr;
extern ProRelfuncArg* Args_StrToInt;

#endif
