#include "./includes/main.h"

#define DIALOGNAME (char *)"MyTreeDialog"
#define TreeNAME (char *)"MyTree"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void _commitOK()
{
    ProError status;
    // status = ProUITreeNodeTypeFree(_nodetypedata);

    status = ProUIDialogExit(DIALOGNAME, 1);
}

void _commitCancel()
{
    ProError status;
    // status = ProUITreeNodeTypeFree(_nodetypedata);

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
    status = ProUITreeSelectednamesGet(DIALOGNAME, TreeNAME, &num_sels, &selnames);

    if (num_sels == 1)
    {

        status = ProUITreeNodeIsExpanded(DIALOGNAME, TreeNAME, selnames[0], &isExpanded);
        if (isExpanded == PRO_B_TRUE)
        {
            status = ProUITreeNodeCollapse(DIALOGNAME, TreeNAME, selnames[0]);
            status = ProUITreeAllnodesDeselect(DIALOGNAME, TreeNAME);
        }
        else
        {
            status = ProUITreeNodeExpand(DIALOGNAME, TreeNAME, selnames[0], PRO_B_TRUE);
            status = ProUITreeAllnodesDeselect(DIALOGNAME, TreeNAME);
        }

        ProStringarrayFree(selnames, num_sels);
    }
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
    status = ProUITreeNodetypeAdd(DIALOGNAME, TreeNAME, nodetypedataRoot);

    status = ProUITreeNodeTypeAlloc("Branch", &nodetypedataBranch);
    status = ProUITreeNodeTypeCollapseImageSet(nodetypedataBranch, "UI ScrollBar right image");
    status = ProUITreeNodeTypeExpandImageSet(nodetypedataBranch, "UI ScrollBar down image");
    status = ProUITreeNodetypeAdd(DIALOGNAME, TreeNAME, nodetypedataBranch);

    status = ProUITreeNodeTypeAlloc("Leaf", &nodetypedataLeaf);
    status = ProUITreeNodeTypeCollapseImageSet(nodetypedataLeaf, "UI small info image");
    status = ProUITreeNodeTypeExpandImageSet(nodetypedataLeaf, "UI small info image");
    status = ProUITreeNodetypeAdd(DIALOGNAME, TreeNAME, nodetypedataLeaf);

    status = ProUITreeNodeAdd(DIALOGNAME, TreeNAME, "Node1", L"…Ë÷√", NULL, "Root");
    status = ProUITreeNodeAdd(DIALOGNAME, TreeNAME, "Node2", L"Node2", "Node1", "Branch");
    status = ProUITreeNodeAdd(DIALOGNAME, TreeNAME, "Node3", L"Node3", "Node2", "Branch");
    status = ProUITreeNodeAdd(DIALOGNAME, TreeNAME, "Node4", L"Node4", "Node3", "Leaf");

    status = ProUITreeNodeExpand(DIALOGNAME, TreeNAME, "Node1", PRO_B_TRUE);
    status = ProUITreeActivateActionSet(DIALOGNAME, TreeNAME, OnNodeActivate, NULL);
    status = ProUITreeAllnodesDeselect(DIALOGNAME, TreeNAME);

    status = ProUITreeColumnCreate(DIALOGNAME, TreeNAME, "column1", L"labe1", 10, PROUITREECOL_LEFT);
    status = ProUITreeColumnCreate(DIALOGNAME, TreeNAME, "column2", L"labe2", 10, PROUITREECOL_LEFT);

    status = ProUITreeUseScrollbarswhenNeeded(DIALOGNAME, TreeNAME);

    status = ProUITreeTreeredrawSet(DIALOGNAME, TreeNAME, PRO_B_TRUE);

    status = ProUITreeSelectionpolicySet(DIALOGNAME, TreeNAME, PROUISELPOLICY_SINGLE);
    status = ProUITreeEnable(DIALOGNAME, TreeNAME);

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
