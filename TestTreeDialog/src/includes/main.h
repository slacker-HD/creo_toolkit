#ifndef __MAIN__
#define __MAIN__

#define UNICODE
#define _UNICODE

#define MSGFILE L"TestTreeDialog.txt"

#include "stdio.h"

#include <ProUICmd.h>
#include <ProUtil.h>
#include <ProMenu.h>
#include <ProToolkitDll.h>
#include <ProMenubar.h>
#include <ProMdl.h>
#include <ProArray.h>
#include <ProUITree.h>
#include <ProMaterial.h>
#include <ProMdlUnits.h>
#include <ProUIMessage.h>
#include <ProUIInputpanel.h>

PRO_BEGIN_C_DECLS
extern ProError ProUITreeNodeColumntextGet(char *dialog, char *component, char *name, char *column, wchar_t **text);
PRO_END_C_DECLS

#endif
