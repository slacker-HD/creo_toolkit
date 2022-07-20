#include "./includes/BattoDwg.h"

void _toDwg(ProDrawing Drawing, ProName name)
{
	ProError status;
	ProName dwgName = L"";
	ProPath currentpath;

	status = ProDirectoryCurrentGet(currentpath);
	status = ProWstringConcatenate(name, dwgName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L".dwg", dwgName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(dwgName, currentpath, PRO_VALUE_UNUSED);

	status = ProOutputFileWrite(Drawing, dwgName, PRO_DWG_FILE, NULL, NULL, NULL, NULL);
}

void BatToDwg()
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
	status = ProFilesList(NULL, L"*.drw", PRO_FILE_LIST_LATEST, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 0)
		{
			for (i = 0; i < n_files; i++)
			{
				status = ProFilenameParse(file_list[i], r_path, r_file_name, r_extension, &r_version);
				status = ProMdlRetrieve(r_file_name, PRO_DRAWING, &mdl);
				status = ProWindowCurrentGet(&winid);
				status = ProMdlDisplay(mdl);
				status = ProWindowActivate(winid);
				_toDwg((ProDrawing)mdl, r_file_name);
				status = ProMdlErase(mdl);
			}
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}
