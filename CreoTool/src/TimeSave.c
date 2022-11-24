#include "includes/TimeSave.h"

int minute = 10;
ProBoolean saveAll = PRO_B_FALSE;
ProBoolean autoSave = PRO_B_FALSE;

ProError _proCollect2ParamDBVisitAction(void *P_object, ProAppData AppData)
{
	ProError status;
	ProArray *p_array;
	p_array = (ProArray *)((void **)AppData)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, P_object);
	return (status);
}

ProError _proCollect2ParamIdVisitAction(int id, ProAppData AppData)
{
	return (_proCollect2ParamDBVisitAction((void *)&id, AppData));
}

ProError _proCollectWindowIds(int **p_winid)
{
	ProError status;
	if (p_winid != NULL)
	{
		status = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)p_winid);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProWindowsVisit((ProWindowVisitAction)_proCollect2ParamIdVisitAction, (ProAppData)&p_winid);
			if (status != PRO_TK_NO_ERROR)
			{
				ProArrayFree((ProArray *)p_winid);
				*p_winid = NULL;
			}
		}
	}
	else
		status = PRO_TK_BAD_INPUTS;
	return status;
}

void _saveMdl(BOOL saveall)
{
	ProError status;
	int *p_array = NULL;
	int i, ArraySize = 0;
	ProMdl mdl;
	int CurrentWid;
	ProBoolean save_allowed;
	ProBoolean is_modified;
	status = ProWindowCurrentGet(&CurrentWid);
	if (saveall)
	{
		status = _proCollectWindowIds(&p_array);
		status = ProArraySizeGet(p_array, &ArraySize);
		for (i = 0; i < ArraySize; i++)
		{
			if (CurrentWid != p_array[i])
			{
				status = ProWindowMdlGet(p_array[i], &mdl);
				if (status != PRO_TK_NO_ERROR)
					return;
				status = ProMdlIsSaveAllowed(mdl, PRO_B_TRUE, &save_allowed);
				if (status != PRO_TK_NO_ERROR)
				{
					status = ProArrayFree((ProArray *)&p_array);
					return;
				}
				if (save_allowed == PRO_B_TRUE)
				{
					status = ProMdlModificationVerify(mdl, &is_modified);
					if (is_modified == PRO_B_TRUE)
					{
						status = ProMdlSave(mdl);
					}
				}
			}
		}
		status = ProArrayFree((ProArray *)&p_array);
	}
	status = ProMdlCurrentGet(&mdl);
	if (status == PRO_TK_NO_ERROR)
	{
		status = ProMdlModificationVerify(mdl, &is_modified);
		if (is_modified == PRO_B_TRUE)
		{
			ProMacroLoad(L"~ Command `ProCmdModelSave` ;~ Activate `file_saveas` `OK`;");
		}
	}
	status = ProMessageDisplay(MSGFILE, "IMI_MESSAGE_AutoSaved");
}

static VOID CALLBACK _saveProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (autoSave)
	{
		_saveMdl(saveAll);
	}
}

void _timeSavecommitOK()
{
	ProError status;
	wchar_t *value;
	int ntime;

	status = ProUIInputpanelValueGet("TimeSave", "InputTime", &value);
	ntime = _wtoi(value);
	if (ntime >= 5 && ntime <= 60)
	{
		minute = ntime;
		status = ProUICheckbuttonGetState("TimeSave", "CBAutoSave", &autoSave);
		status = ProUICheckbuttonGetState("TimeSave", "CBSaveAll", &saveAll);
	}
	else
	{
		ShowMessageDialog(1, L"时间范围必须设定在5~60分钟以内。");
		return;
	}
	status = ProWstringFree(value);
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)_saveProc);
	status = ProUIDialogExit("TimeSave", 1);
}

void _timeSavecommitCancel()
{
	ProError status;
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)_saveProc);
	status = ProUIDialogExit("TimeSave", 0);
}

void ShowTimeSaveDialog()
{
	ProError status;
	wchar_t text[5];
	KillTimer(NULL, TIMERID);

	status = ProUIDialogCreate("TimeSave", "TimeSave");
	status = ProUIInputpanelInputtypeSet("TimeSave", "InputTime", PROUIINPUTTYPE_INTEGER);
	if (autoSave)
		status = ProUICheckbuttonSet("TimeSave", "CBAutoSave");
	if (saveAll)
		status = ProUICheckbuttonSet("TimeSave", "CBSaveAll");
	_itow_s(minute, text, 5, 10);
	status = ProUIInputpanelValueSet("TimeSave", "InputTime", text);

	status = ProUIPushbuttonActivateActionSet("TimeSave", "TimeSaveCommitOK", (ProUIAction)_timeSavecommitOK, NULL);
	status = ProUIPushbuttonActivateActionSet("TimeSave", "TimeSaveCommitCancel", (ProUIAction)_timeSavecommitCancel, NULL);

	status = ProUIDialogActivate("TimeSave", NULL);
	status = ProUIDialogDestroy("TimeSave");
}
