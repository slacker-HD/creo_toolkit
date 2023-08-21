#ifndef __COMMONFUNS__
#define __COMMONFUNS__
#include "main.h"

int CurrentMdlType();
ProError ShowMessageDialog(int style, wchar_t *Message);
ProError ProRibbonTabSwitchNotification(char *from_tab, char *to_tab);
ProError ProDirectoryChangeNotification(ProPath new_path);
ProError ProMdlPathGet(ProMdldata mdldata, ProPath mdlPath);
uiCmdAccessState ComponentInASMTreeAccess(uiCmdAccessMode mode);

ProError ProUserWindowChangePost();
ProError ProUserMdlSavePost(ProMdldata *p_mdldata);
ProError ProUserMdlRetrievePost(ProModel *p_mdldata);
ProError ProUserRefreshAll();

extern char *LastRibbonTab;
extern ProPath *CurrentWorkDirectoryList;
#endif
