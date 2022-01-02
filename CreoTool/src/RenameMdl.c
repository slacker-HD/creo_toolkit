#include "./includes/RenameMdl.h"

void RenamePrtandDrw()
{
	ProError status;
	ProMdl mdl, drw;
	ProMdldata mdldata;
	ProName newName;
	ProPath currentPath, realPath;
	boolean drwFinded;

	status = ProMdlCurrentGet(&mdl);
	status = ProDirectoryCurrentGet(currentPath);
	status = ProMessageDisplay(MSGFILE, "IMI_Enter_New_Name");
	status = ProMessageStringRead(PRO_NAME_SIZE, newName);

	if (status == PRO_TK_NO_ERROR)
	{
		status = ProMdlDataGet(mdl, &mdldata);

		status = ProWstringCopy(mdldata.device, realPath, PRO_VALUE_UNUSED);
		status = ProWstringConcatenate(L":", realPath, PRO_VALUE_UNUSED);
		status = ProWstringConcatenate(mdldata.path, realPath, PRO_VALUE_UNUSED);


		status = ProDirectoryChange(realPath);
		status = ProMdlRename(mdl, newName);
		status = ProMdlSave(mdl);

		drwFinded = ProMdlLoad(mdldata.name, PRO_MDL_DRAWING, PRO_B_FALSE, &drw) == PRO_TK_NO_ERROR ? TRUE : FALSE;

		if (drwFinded == TRUE)
		{
			status = ProMdlRename(drw, newName);
			status = ProMdlSave(drw);
		}

		status = ProDirectoryChange(currentPath);
	}
}