#include "includes/TimeSave.h"

int minute = 10;
ProBoolean saveAll = PRO_B_FALSE;
ProBoolean autoSave = PRO_B_FALSE;

ProError ProCollect2ParamDBVisitAction(void *p_object, ProAppData app_data)
{
	ProError status;
	ProArray *p_array;
	p_array = (ProArray *)((void **)app_data)[0];
	status = ProArrayObjectAdd(p_array, PRO_VALUE_UNUSED, 1, p_object);
	return (status);
}

ProError ProCollect2ParamIdVisitAction(int id, ProAppData app_data)
{
	return (ProCollect2ParamDBVisitAction((void *)&id, app_data));
}

ProError ProCollectWindowIds(int **p_winid)
{
	ProError status;
	if (p_winid != NULL)
	{
		status = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)p_winid);
		if (status == PRO_TK_NO_ERROR)
		{
			status = ProWindowsVisit((ProWindowVisitAction)ProCollect2ParamIdVisitAction, (ProAppData)&p_winid);
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

void SaveMdl(BOOL saveall)
{
	ProError status;
	int *p_array = NULL;
	int i, ArraySize = 0;
	ProMdl mdl;
	int CurrentWid;
	ProBoolean save_allowed;
	status = ProWindowCurrentGet(&CurrentWid);
	if (saveall)
	{
		status = ProCollectWindowIds(&p_array);
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
					status = ProMdlSave(mdl);
			}
		}
		status = ProArrayFree((ProArray *)&p_array);
	}
	ProMacroLoad(L"~ Command `ProCmdModelSave` ;~ Activate `file_saveas` `OK`;");
	status = ProMessageDisplay(MSGFILE, "AutoSaved");
}

static VOID CALLBACK SaveProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if (autoSave)
	{
		SaveMdl(saveAll);
	}
}

void _commitOK()
{
	ProError status;
	wchar_t *value;
	int ntime;

	status = ProUIInputpanelValueGet("TimerSaver", "InputTime", &value);
	ntime = _wtoi(value);
	if (ntime >= 5 && ntime <= 60)
	{
		minute = ntime;
		status = ProUICheckbuttonGetState("TimerSaver", "CBAutoSave", &autoSave);
		status = ProUICheckbuttonGetState("TimerSaver", "CBSaveAll", &saveAll);
	}
	else
	{
		ShowDialog(_T("时间范围必须设定在5~60分钟以内。"));
		return;
	}
	status = ProWstringFree(value);
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)SaveProc);
	status = ProUIDialogExit("TimerSaver", 1);
}

void _commitCancel()
{
	ProError status;
	SetTimer(NULL, TIMERID, minute * 60000, (TIMERPROC)SaveProc);
	status = ProUIDialogExit("TimerSaver", 0);
}


void ShowTimeSaveDialog()
{
	ProError status;
	wchar_t text[5];
	// KillTimer(NULL, TIMERID);

	
	status = ProUIDialogCreate("TimeSave", "TimeSave");
	status = ProUIInputpanelInputtypeSet("TimeSave", "InputTime", PROUIINPUTTYPE_INTEGER);
	// if (autoSave)
	// 	status = ProUICheckbuttonSet("TimeSave", "CBAutoSave");
	// if (saveAll)
	// 	status = ProUICheckbuttonSet("TimeSave", "CBSaveAll");
	// _itow_s(minute, text, 5, 10);
	// status = ProUIInputpanelValueSet("TimeSave", "InputTime", text);

	status = ProUIPushbuttonActivateActionSet("TimeSave", "CommitOK", ProUIAction(_commitOK), NULL);
	status = ProUIPushbuttonActivateActionSet("TimeSave", "CommitCancel", ProUIAction(_commitCancel), NULL);

	status = ProUIDialogActivate("TimeSave", NULL);
	status = ProUIDialogDestroy("TimeSave");
}
