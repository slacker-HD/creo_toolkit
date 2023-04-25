#include "./includes/main.h"

#define DIALOGNAME (char *)"MyTreeDialog"
#define TREENAME (char *)"MyTree"
#define INPUTPANELNAME (char *)"InputPanel1"

ProUIMessageButton ShowMessageDialog(int style, wchar_t *Message)
{
    ProUIMessageButton *buttons;
    ProUIMessageButton user_choice;
    ProError status;
    if (style == 1)
    {
        status = ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
        buttons[0] = PRO_UI_MESSAGE_YES;
    }
    else
    {
        ProArrayAlloc(2, sizeof(ProUIMessageButton), 1, (ProArray *)&buttons);
        buttons[0] = PRO_UI_MESSAGE_YES;
        buttons[1] = PRO_UI_MESSAGE_NO;
    }

    status = ProUIMessageDialogDisplay(PROUIMESSAGE_QUESTION, L"?��??", Message, buttons, buttons[0], &user_choice);
    ProArrayFree((ProArray *)&buttons);
    return user_choice;
}

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void _commitOK()
{
    ProError status;
    status = ProUIDialogExit(DIALOGNAME, 1);
}

void _commitCancel()
{
    ProError status;
    status = ProUIDialogExit(DIALOGNAME, 0);
}

void OnNodeActivate(char *dialog, char *component, ProAppData data)
{
    ProError status;
    ProBoolean isExpanded;
    int i;
    char *label;
    int num_sels;
    char **selnames;
    status = ProUITreeSelectednamesGet(DIALOGNAME, TREENAME, &num_sels, &selnames);
    if (num_sels == 1)
    {
        status = ProUITreeNodeIsExpanded(DIALOGNAME, TREENAME, selnames[0], &isExpanded);
        if (isExpanded == PRO_B_TRUE)
        {
            status = ProUITreeNodeCollapse(DIALOGNAME, TREENAME, selnames[0]);
            status = ProUITreeAllnodesDeselect(DIALOGNAME, TREENAME);
        }
        else
        {
            status = ProUITreeNodeExpand(DIALOGNAME, TREENAME, selnames[0], PRO_B_FALSE);
            status = ProUITreeAllnodesDeselect(DIALOGNAME, TREENAME);
        }
    }
    ProStringarrayFree(selnames, num_sels);
}

void OnCellActivate(char *dialog, char *component, ProAppData data)
{
    ProError status;
    char *cellSelCol;
    char *cellSelNode;

    status = ProUITreeTreecellselcolGet(dialog, component, &cellSelCol);
    status = ProUITreeTreecellselnodeGet(dialog, component, &cellSelNode);
    status = ProUIInputpanelShow(dialog, INPUTPANELNAME);
    status = ProUITreeNodeComponentSet(dialog, component, cellSelNode, INPUTPANELNAME, cellSelCol);

    status = ProStringFree(cellSelCol);
    status = ProStringFree(cellSelNode);
}

void OnInputPanelFocusin(char *dialog, char *component, ProAppData data)
{
    ProError status;
    char *cellSelCol;
    char *cellSelNode;
    wchar_t *text;
    status = ProUITreeTreecellselcolGet(DIALOGNAME, TREENAME, &cellSelCol);
    status = ProUITreeTreecellselnodeGet(DIALOGNAME, TREENAME, &cellSelNode);

    // status = ProUITreeNodeColumntextGet(DIALOGNAME, TREENAME, "Node5", "column2", &text);

    status = ProWstringFree(text);
}

void OnInputPanelFocusout(char *dialog, char *component, ProAppData data)
{
    ProError status;
    char *cellSelCol;
    char *cellSelNode;
    wchar_t *text;

    status = ProUIInputpanelHide(dialog, INPUTPANELNAME);
    status = ProUITreeTreecellselcolGet(DIALOGNAME, TREENAME, &cellSelCol);
    status = ProUITreeTreecellselnodeGet(DIALOGNAME, TREENAME, &cellSelNode);
    status = ProUIInputpanelWidestringGet(dialog, INPUTPANELNAME, &text);
    status = ProUITreeNodeColumntextSet(DIALOGNAME, TREENAME, cellSelNode, text, cellSelCol);
    status = ProWstringFree(text);
}

void OnInputPanelActivate(char *dialog, char *component, ProAppData data)
{
}

void ShowTestTreeDialogDialog()
{
    ProError status;
    ProUITreeNodeType nodetypedataRoot, nodetypedataBranch, nodetypedataLeaf;

    status = ProUIDialogCreate(DIALOGNAME, DIALOGNAME);
    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitOK", (ProUIAction)_commitOK, NULL);
    status = ProUIPushbuttonActivateActionSet(DIALOGNAME, "CommitCancel", (ProUIAction)_commitCancel, NULL);

    status = ProUITreeNodeTypeAlloc("Root", &nodetypedataRoot);
    status = ProUITreeNodeTypeCollapseImageSet(nodetypedataRoot, "PTC small logo image");
    status = ProUITreeNodeTypeExpandImageSet(nodetypedataRoot, "PTC small logo image");
    status = ProUITreeNodetypeAdd(DIALOGNAME, TREENAME, nodetypedataRoot);

    status = ProUITreeNodeTypeAlloc("Branch", &nodetypedataBranch);
    status = ProUITreeNodeTypeCollapseImageSet(nodetypedataBranch, "UI ScrollBar right image");
    status = ProUITreeNodeTypeExpandImageSet(nodetypedataBranch, "UI ScrollBar down image");
    status = ProUITreeNodetypeAdd(DIALOGNAME, TREENAME, nodetypedataBranch);

    status = ProUITreeNodeTypeAlloc("Leaf", &nodetypedataLeaf);
    status = ProUITreeNodeTypeCollapseImageSet(nodetypedataLeaf, "UI small info image");
    status = ProUITreeNodeTypeExpandImageSet(nodetypedataLeaf, "UI small info image");
    status = ProUITreeNodetypeAdd(DIALOGNAME, TREENAME, nodetypedataLeaf);

    status = ProUITreeNodeAdd(DIALOGNAME, TREENAME, "Node1", L"setting", NULL, "Root");
    status = ProUITreeNodeAdd(DIALOGNAME, TREENAME, "Node2", L"Node2", "Node1", "Branch");
    status = ProUITreeNodeAdd(DIALOGNAME, TREENAME, "Node3", L"Node3", "Node2", "Branch");
    status = ProUITreeNodeAdd(DIALOGNAME, TREENAME, "Node4", L"Node4", "Node3", "Leaf");
    status = ProUITreeNodeAdd(DIALOGNAME, TREENAME, "Node5", L"Node5", "Node2", "Leaf");

    status = ProUITreeNodeExpand(DIALOGNAME, TREENAME, "Node1", PRO_B_TRUE);
    status = ProUITreeActivateActionSet(DIALOGNAME, TREENAME, OnNodeActivate, NULL);
    status = ProUITreeAllnodesDeselect(DIALOGNAME, TREENAME);

    status = ProUITreeColumnCreate(DIALOGNAME, TREENAME, "column1", L"labe1", 10, PROUITREECOL_LEFT);
    status = ProUITreeColumnCreate(DIALOGNAME, TREENAME, "column2", L"labe2", 10, PROUITREECOL_LEFT);

    status = ProUITreeUseScrollbarswhenNeeded(DIALOGNAME, TREENAME);

    status = ProUITreeTreeredrawSet(DIALOGNAME, TREENAME, PRO_B_TRUE);

    status = ProUITreeSelectionpolicySet(DIALOGNAME, TREENAME, PROUISELPOLICY_SINGLE);
    status = ProUITreeEnable(DIALOGNAME, TREENAME);

    status = ProUITreeTreecellactivateActionSet(DIALOGNAME, TREENAME, OnCellActivate, NULL);

    status = ProUIInputpanelFocusinActionSet(DIALOGNAME, INPUTPANELNAME, OnInputPanelFocusin, NULL);
    status = ProUIInputpanelFocusoutActionSet(DIALOGNAME, INPUTPANELNAME, OnInputPanelFocusout, NULL);
    status = ProUIInputpanelActivateActionSet(DIALOGNAME, INPUTPANELNAME, OnInputPanelActivate, NULL);

    status = ProUIDialogActivate(DIALOGNAME, NULL);
    status = ProUIDialogDestroy(DIALOGNAME);

    status = ProUITreeNodeTypeFree(nodetypedataRoot);
    status = ProUITreeNodeTypeFree(nodetypedataBranch);
    status = ProUITreeNodeTypeFree(nodetypedataLeaf);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_TestTreeDialogmenuID;

    status = ProMenubarMenuAdd("IMI_Paramsmenu", "IMI_Paramsmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_UnitInterPret_Act", (uiCmdCmdActFn)ShowTestTreeDialogDialog, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_TestTreeDialogmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Paramsmenu", "IMI_TestTreeDialogmenu", "IMI_TestTreeDialogmenu", "IMI_TestTreeDialogmenutips", NULL, PRO_B_TRUE, IMI_TestTreeDialogmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
