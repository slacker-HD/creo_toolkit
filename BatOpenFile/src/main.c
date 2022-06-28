#include "./includes/main.h"

static uiCmdAccessState AccessDefault(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

void _openFilesByType(wchar_t *filter, ProMdlType mdltype)
{
    ProError status;
    ProPath *file_list, *dir_list, r_path;
    int i, n_files;
    ProMdl mdl;
    int new_win_id;
    ProName r_file_name, r_extension;
    ProFileName f_file_name;
    int r_version;
    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
    status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
    status = ProFilesList(NULL, filter, PRO_FILE_LIST_LATEST, &file_list, &dir_list);
    if (status == PRO_TK_NO_ERROR)
    {
        status = ProArraySizeGet((ProArray)file_list, &n_files);
        if (n_files > 0)
        {
            for (i = 0; i < n_files; i++)
            {
                memset(f_file_name, '\0', sizeof(ProFileName));
                status = ProFilenameParse(file_list[i], r_path, r_file_name, r_extension, &r_version);
                status = ProWstringConcatenate(r_file_name, f_file_name, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(L".", f_file_name, PRO_VALUE_UNUSED);
                status = ProWstringConcatenate(r_extension, f_file_name, PRO_VALUE_UNUSED);
                status = ProMdlLoad(f_file_name, PRO_MDL_UNUSED, PRO_B_FALSE, &mdl);
                status = ProObjectwindowCreate(f_file_name, mdltype, &new_win_id);
                status = ProMdlDisplay(mdl);
            }
        }
    }
    status = ProArrayFree((ProArray *)&file_list);
    status = ProArrayFree((ProArray *)&dir_list);
}

void OpenPrts()
{
    _openFilesByType(L"*.prt", PRO_PART);
}

void OpenDrws()
{
    _openFilesByType(L"*.drw", PRO_DRAWING);
}

int user_initialize()
{
    ProError status;
    uiCmdCmdId IMI_BatOPenPrtmenuID;
    uiCmdCmdId IMI_BatOPenDrwmenuID;

    status = ProMenubarMenuAdd("IMI_BatOPenmenu", "IMI_BatOPenmenu", "About", PRO_B_TRUE, MSGFILE);

    status = ProCmdActionAdd("IMI_BatOPenPrt_Act", (uiCmdCmdActFn)OpenPrts, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatOPenPrtmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatOPenmenu", "IMI_BatOPenPrtmenu", "IMI_BatOPenPrtmenu", "IMI_BatOPenPrtmenutips", NULL, PRO_B_TRUE, IMI_BatOPenPrtmenuID, MSGFILE);

    status = ProCmdActionAdd("IMI_BatOPenDrw_Act", (uiCmdCmdActFn)OpenDrws, uiProeImmediate, AccessDefault, PRO_B_TRUE, PRO_B_TRUE, &IMI_BatOPenDrwmenuID);
    status = ProMenubarmenuPushbuttonAdd("IMI_BatOPenmenu", "IMI_BatOPenDrwmenu", "IMI_BatOPenDrwmenu", "IMI_BatOPenDrwmenutips", NULL, PRO_B_TRUE, IMI_BatOPenDrwmenuID, MSGFILE);

    return PRO_TK_NO_ERROR;
}

void user_terminate()
{
}
