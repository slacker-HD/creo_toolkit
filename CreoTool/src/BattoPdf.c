#include "./includes/BattoPdf.h"

void _toPdf(ProDrawing Drawing)
{
	ProName w_name;
	ProError status;
	ProPath currentpath;
	int wid;
	status = ProMdlNameGet(Drawing, w_name);
	status = ProDirectoryCurrentGet(currentpath);
	if (!PathFileExists(Cpath + _T("pdf\\")))
	{
		if (!CreateDirectory(Cpath + _T("pdf\\"), NULL))
		{
			AfxMessageBox(_T("无法创建目录!"));
			return;
		}
	}
	CString a;
	a = Cpath + _T("pdf\\") + CString(w_name) + _T(".pdf");
	FreeP(p);
	p = a.AllocSysString();
	ProPDFOptions options;
	status = ProPDFoptionsAlloc(&options);
	status = ProPDFoptionsIntpropertySet(options, PRO_PDFOPT_EXPORT_MODE, PRO_PDF_2D_DRAWING);
	status = ProPDFExport(Drawing, p, options);
	status = ProPDFoptionsFree(options);
	status = ProMdlEraseNotDisplayed();
	status = ProDirectoryChange(currentpath);
}
void BatToPdf()
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
				status = ProMdlRetrieve(r_file_name, PRO_PART, &mdl);
				status = ProWindowCurrentGet(&winid);
				status = ProMdlDisplay(mdl);
				status = ProWindowActivate(winid);
				_toPdf(mdl);
			}
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}
