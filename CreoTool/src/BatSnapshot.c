#include "./includes/BatSnapshot.h"

void _snapshot(int wid, ProName name)
{
	ProError status;
	ProPath currentpath;
	double width, height;
	ProName jpgName = L"";

	status = ProDirectoryCurrentGet(currentpath);
	status = ProWstringConcatenate(name, jpgName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L".jpg", jpgName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(jpgName, currentpath, PRO_VALUE_UNUSED);

	status = ProViewReset(NULL, NULL);
	status = ProViewRefit(NULL, NULL);
	status = ProMacroLoad(L"~ Command `ProCmdEnvDtmDisp` 1; ~ Command `ProCmdEnvAxisDisp` 1; ~ Command `ProCmdViewSpinCntr` 1; ~ Command `ProCmdEnvPntsDisp`  1;~ Command `ProCmdEnvCsysDisp`  1;");
	status = ProMacroLoad(L"~ Command `ProCmdEnvDtmDisp` 0; ~ Command `ProCmdEnvAxisDisp` 0; ~ Command `ProCmdViewSpinCntr` 0; ~ Command `ProCmdEnvPntsDisp`  0;~ Command `ProCmdEnvCsysDisp`  0;");
	status = ProMacroExecute();
	status = ProWindowRefresh(PRO_VALUE_UNUSED);
	status = ProGraphicWindowSizeGet(wid, &width, &height);
	status = ProRasterFileWrite(wid, PRORASTERDEPTH_24, width * 10, height * 10, PRORASTERDPI_600, PRORASTERTYPE_JPEG, currentpath);
}

void TakeSnapShot()
{
	ProError status;
	ProPath *file_list, *dir_list, r_path;
	int i, n_files;
	ProMdl mdl;
	int winid;
	ProName r_file_name, r_extension;
	int r_version;
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, L"*.prt", PRO_FILE_LIST_LATEST, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 0)
		{
			for (i = 0; i < n_files; i++)
			{
				status = ProFilenameParse(file_list[i], r_path, r_file_name, r_extension, &r_version);
				status = ProMdlRetrieve(r_file_name, PRO_PART, &mdl);
				status = ProWindowCurrentGet(&winid);
				status = ProMdlDisplay(mdl);
				status = ProWindowActivate(winid);
				_snapshot(winid, r_file_name);
				status = ProMdlErase(mdl);
			}
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}