#include "./includes/main.h"

ProSimprep simp_rep, simp_rep_orig;
ProBoolean simp_rep_Inited = PRO_B_FALSE;
int CurrentMdlType()
{
    ProMdl mdl;
    ProMdlType mdlType;
    ProError status;
    status = ProMdlCurrentGet(&mdl);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    status = ProMdlTypeGet(mdl, &mdlType);
    if (status != PRO_TK_NO_ERROR)
        return -1;
    else
        return mdlType;
}

static uiCmdAccessState AccessASM(uiCmdAccessMode access_mode)
{
    if (CurrentMdlType() == PRO_ASSEMBLY)
        return ACCESS_AVAILABLE;
    else
        return ACCESS_INVISIBLE;
}

void HidePart(ProBoolean SelectedPart)
{
    ProError status;
    ProSelection *sel_array;
    int i, n_size;
    ProMdl mdl;
    ProSolid asmSolid;
    ProSimprepAction simprepaction;
    ProSimprepdata *simprepdata;
    ProAsmcomppath comppath;
    ProModelitem modelitem;
    ProSimprepitem simprepitem;
    ProBoolean isselfrombuffer = PRO_B_TRUE;
    if (CurrentMdlType() == PRO_ASSEMBLY)
    {
        status = ProMdlCurrentGet(&mdl);
        asmSolid = ProMdlToSolid(mdl);

        status = ProSelbufferSelectionsGet(&sel_array);
        status = ProArraySizeGet((ProArray *)sel_array, &n_size);
        if (status != PRO_TK_NO_ERROR || n_size < 1)
        {
            status = ProSelectionarrayFree(sel_array);
            status = ProSelect((char *)"prt_or_asm", -1, NULL, NULL, NULL, NULL, &sel_array, &n_size);
            if (status != PRO_TK_NO_ERROR)
            {
                return;
            }
            else
            {
                isselfrombuffer = PRO_B_FALSE;
            }
        }
        if (simp_rep_Inited == PRO_B_TRUE)
        {
            status = ProSimprepDelete(&simp_rep);
        }
        status = SelectedPart == PRO_B_TRUE ? ProSimprepdataAlloc(L"IMI_SIMPVIEW", PRO_B_FALSE, PRO_SIMPREP_INCLUDE, &simprepdata) : ProSimprepdataAlloc(L"IMI_SIMPVIEW", PRO_B_FALSE, PRO_SIMPREP_EXCLUDE, &simprepdata);
        simp_rep_Inited = PRO_B_TRUE;
        status = ProSimprepActiveGet(asmSolid, &simp_rep_orig);
        status = ProSimprepActionInit(PRO_SIMPREP_REVERSE, NULL, &simprepaction);
        if (status == PRO_TK_NO_ERROR)
        {
            for (i = 0; i < n_size; i++)
            {
                status = ProSelectionAsmcomppathGet(sel_array[i], &comppath);
                status = ProSelectionModelitemGet(sel_array[i], &modelitem);
                status = ProSimprepdataitemInit(comppath.comp_id_table, comppath.table_num, modelitem.id, &simprepaction, &simprepitem);
                if (status == PRO_TK_NO_ERROR)
                {
                    status = ProSimprepdataitemAdd(simprepdata, &simprepitem);
                }
            }

            status = ProSimprepCreate(asmSolid, simprepdata, &simp_rep);
        }

        if (status == PRO_TK_NO_ERROR)
        {
            status = ProSimprepActivate(asmSolid, &simp_rep);
        }
        status = ProSolidDisplay(asmSolid);
    }

    status = ProSimprepdataFree(&simprepdata);
    if (isselfrombuffer == PRO_B_TRUE)
    {
        status = ProSelectionarrayFree(sel_array);
    }
}

void HideSelectedPart()
{
    HidePart(PRO_B_TRUE);
}

void HideUnselectedPart()
{
    HidePart(PRO_B_FALSE);
}

void Restore()
{
    ProError status;
    ProSimprep simprep;
    ProMdl mdl;
    ProSolid asmSolid;
    ProSimprepType type, active_type;

    status = ProMdlCurrentGet(&mdl);
    asmSolid = ProMdlToSolid(mdl);
    status = ProSimprepActivate(asmSolid, &simp_rep_orig);
    status = ProSimprepDelete(&simp_rep);
    status = ProSolidDisplay(asmSolid);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_HideSelectedPartID;
    uiCmdCmdId IMI_HideUnselectedPartID;
    uiCmdCmdId IMI_ShowAllPartID;

    status = ProMenubarMenuAdd("IMI_Mainmenu", "IMI_Mainmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_HideSelectedPart_Act", (uiCmdCmdActFn)HideSelectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideSelectedPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenu", "IMI_HideSelectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideSelectedPartID, MSGFILE);

    status = ProCmdActionAdd("IMI_HideUnselectedPart_Act", (uiCmdCmdActFn)HideUnselectedPart, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_HideUnselectedPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenu", "IMI_HideUnselectedPartmenutips", NULL, PRO_B_TRUE, IMI_HideUnselectedPartID, MSGFILE);

    status = ProCmdActionAdd("IMI_ShowAllPart_Act", (uiCmdCmdActFn)Restore, uiProeImmediate, AccessASM, PRO_B_TRUE, PRO_B_TRUE, &IMI_ShowAllPartID);
    status = ProMenubarmenuPushbuttonAdd("IMI_Mainmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenu", "IMI_ShowAllPartmenutips", NULL, PRO_B_TRUE, IMI_ShowAllPartID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
