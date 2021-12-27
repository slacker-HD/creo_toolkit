#ifndef __COMMONFUNS__
#define __COMMONFUNS__
#include "main.h"

int CurrentMdlType();
ProError ShowMessageDialog(int style, wchar_t *Message);
ProError ProRibbonTabSwitchNotification(char *from_tab, char *to_tab);
extern char *LastRibbonTab;

#endif
