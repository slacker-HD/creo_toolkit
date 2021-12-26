#include "./includes/CleanWorkDirectory.h"

void _recycle(wchar_t *FilePath, BOOL bDelete)
{
	LPSHFILEOPSTRUCTW shDelFile;
	shDelFile = malloc(sizeof(LPSHFILEOPSTRUCTW));
	memset(shDelFile, 0, sizeof(LPSHFILEOPSTRUCTW));
	shDelFile->fFlags |= FOF_SILENT;
	shDelFile->fFlags |= FOF_NOERRORUI;
	shDelFile->fFlags |= FOF_NOCONFIRMATION;
	shDelFile->wFunc = FO_DELETE;
	shDelFile->pFrom = FilePath;
	shDelFile->pTo = NULL;
	if (bDelete)
	{
		shDelFile->fFlags &= ~FOF_ALLOWUNDO;
	}
	else
	{
		shDelFile->fFlags |= FOF_ALLOWUNDO;
	}
	SHFileOperation(shDelFile);
	free(shDelFile);
}

int _compareVersion(ProPath p1, ProPath p2)
{
	int ver1 = 0, ver2 = 0;
	ProError status;
	status = ProFilenameParse(p1, NULL, NULL, NULL, &ver1);
	status = ProFilenameParse(p2, NULL, NULL, NULL, &ver2);
	return (ver1 - ver2);
}

ProError _getRealFileName(ProPath FileFullName, ProPath RealName)
{
	ProError status;
	ProName r_file_name, r_extension;

	status = ProFilenameParse(FileFullName, NULL, r_file_name, r_extension, NULL);
	status = ProWstringCopy(r_file_name, RealName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(L".", RealName, PRO_VALUE_UNUSED);
	status = ProWstringConcatenate(r_extension, RealName, PRO_VALUE_UNUSED);
	return PRO_TK_NO_ERROR;
}

void _proMdlPurge(ProPath W_name)
{
	ProError status;
	ProPath *file_list, *dir_list;
	ProPath rfilename;
	int i;
	unsigned int n_files;

	status = _getRealFileName(W_name, rfilename);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, rfilename, PRO_FILE_LIST_ALL, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 1)
		{
			qsort(file_list, n_files, sizeof(file_list[0]), (int (*)(const void *, const void *))_compareVersion);
		}
		for (i = 0; i < n_files - 1; i++)
		{
			_recycle(file_list[i], FALSE);
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}

void _proMdlPurgeFileType(wchar_t *Type)
{
	ProError status;
	ProPath *file_list, *dir_list;
	int i, n_files;
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&file_list);
	status = ProArrayAlloc(0, sizeof(ProPath), 1, (ProArray *)&dir_list);
	status = ProFilesList(NULL, Type, PRO_FILE_LIST_LATEST, &file_list, &dir_list);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProArraySizeGet((ProArray)file_list, &n_files);
		if (n_files > 0)
		{
			for (i = 0; i < n_files; i++)
			{
				_proMdlPurge(file_list[i]);
			}
		}
	}
	status = ProArrayFree((ProArray *)&file_list);
	status = ProArrayFree((ProArray *)&dir_list);
}

void ProMdlPurgeAll()
{
	_proMdlPurgeFileType(L"*.drw");
	_proMdlPurgeFileType(L"*.prt");
	_proMdlPurgeFileType(L"*.asm");
	ShowMessageDialog(1, L"当前工作目录已清理。");
}